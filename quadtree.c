/*Ignatescu Darius Bogdan
Grupa 311CB*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct nod {
    unsigned char tip;
    unsigned char r,g,b;
    struct nod *f1,*f2,*f3,*f4;
}TNod,*TArb;

typedef struct matrice_pixeli{
    unsigned char r,g,b;
}TMatrice;

typedef struct lista
{
    struct lista *urm;
    unsigned char info;
}Lista,*TLista;

TArb AlocaNod(unsigned int tip,unsigned char r,unsigned char g,unsigned char b)
{
    TArb aux = (TArb)malloc(sizeof(TNod));
    if(aux == NULL)
        return NULL;
    aux->tip = tip;
    aux->r = r;
    aux->g = g;
    aux->b = b;
    aux->f1 = aux->f2 = aux->f3 = aux->f4 = NULL;
    return aux;
}

TLista aloca_element_lista(unsigned char x) {
    TLista aux = (TLista)malloc(sizeof(Lista));
    if(aux == NULL)
        return NULL;
    aux->info = x;
    aux->urm = NULL;
    return aux;
}

// functia divide_matrice imparte recursiv matricea de pixeli si simultan construieste arborele de compresie
void divide_matrice(TMatrice **w,int a,int g,int b,int d,TArb radacina,int ramura, unsigned long long int factor,int *p,FILE *f2)
{
    //
    TArb aux; //aux este nodul corespunzator zonei curente
    //ok == 1 inseamna ca nodul corespunzator zonei curente din matrice e nod terminal
    int ok = 0;//presupunem ca nu e terminal
    unsigned long long int red,green,blue,mean;
    unsigned int i,j;
    red = green = blue = mean = 0;
/*  a = indicele primei linii din zona curenta
    g = indicele ultimei linii din zona curenta
    b = indicele primei coloane din zona curenta
    d = indicele ultimei coloane din zona curenta
*/

    //parcurgem matricea de pixeli si calculam scorurile
    for(i = a; i <= g; i++){
        for(j = b; j <=d ;j++) {
            red = red + w[i][j].r;
            green = green + w[i][j].g;
            blue = blue + w[i][j].b;
        }
    }

    red = red / ((g - a  + 1) * (g - a + 1));
    green = green / ((g - a + 1 ) * (g - a + 1));
    blue = blue / ((g - a + 1) * (g - a + 1));

    for(i = a; i <= g; i++){
        for(j = b; j <=d ;j++) {
            mean = mean + (red-w[i][j].r)*(red-w[i][j].r) + (green-w[i][j].g)* (green-w[i][j].g) +
                   (blue-w[i][j].b) * (blue-w[i][j].b);
        }
    }

    //calculam scorul similaritatii si decidem tipul nodului
    mean = mean / (3 * (g - a + 1) * (g - a + 1));
    if(mean <= factor) {
        ok = 1;
    }
    unsigned int m1 = (b + d) / 2; //indicele ultimei coloane din jumatatea stanga a zonei curente
    unsigned int m2 = (a + g) / 2; //indicele ultimei linii din jumatatea superioara a zonei curente
    if(ok == 1)
        aux = AlocaNod('1',red,green,blue);
    else
        aux = AlocaNod('0',red,green,blue); //nu conteaza ce valori se afla aici,
                                                        //deoarece e nod intern si mai trebuie divizat
    if(aux == NULL)
        return ;

    // ramura este un indicator pentru fiu(exemplu : ramura == 1 ==>aux este fiul 1 al nodului pentru care am apelat functia)
        if(ramura == 1)
            radacina->f1=aux;
    else
        if(ramura == 2)
            radacina->f2=aux;
    else
        if(ramura == 3)
            radacina->f3=aux;
    else
        if(ramura == 4)
            radacina->f4 = aux;
    radacina = aux; //nodul curent devine nodul la care vom lega alte noduri dupa urmatoarele apeluri(daca vor exista)

    if(ok == 1) { //daca zona curenta nu se mai imparte in alte 4,cautam o posibila valoare maxima pentru latura de la cerinta 1
        if(g - a + 1 >= *p)
            *p = g - a + 1;
    }

    if(ok == 0){ //daca e nod intern,atunci se divide din nou si apelam functia pentru toti cei 4 fii
        divide_matrice(w, a, m2, b, m1, radacina, 1,factor,p,f2);
        divide_matrice(w, a, m2, m1 + 1, d, radacina, 2,factor,p,f2);
        divide_matrice(w, m2 + 1, g, m1 + 1, d, radacina, 3,factor,p,f2);
        divide_matrice(w, m2 + 1, g, b, m1, radacina, 4,factor,p,f2);
    }

}

//nr_niveluri determina indicele ultimului nivel din arbore(radacina e pe nivelul 0)
void nr_niveluri(TArb radacina,int niv_crt,int *niv_max) {

    if(radacina != NULL) {
        if(*niv_max < 0 || *niv_max < niv_crt)
            *niv_max = niv_crt;
        nr_niveluri(radacina->f1,niv_crt+1,niv_max);
        nr_niveluri(radacina->f2,niv_crt+1,niv_max);
        nr_niveluri(radacina->f3,niv_crt+1,niv_max);
        nr_niveluri(radacina->f4,niv_crt+1,niv_max);
    }

}

int nr_noduri_terminale(TArb radacina) {
    if(radacina == NULL)
        return 0;
    return (radacina->tip == '1') + nr_noduri_terminale(radacina->f1)+nr_noduri_terminale(radacina->f2)+
           nr_noduri_terminale(radacina->f3) + nr_noduri_terminale(radacina->f4);
}

//pornind de la un arbore de compresie cunoscut,functia de mai jos construieste imaginea corespunzatoare arborelui
void completeaza_matrice(int dimensiune,TMatrice **c,int a,int g,int b,int d,TArb radacina)
{
    unsigned int i,j;
    if(radacina->tip == '1') { //daca e nod terminal,toti pixelii zonei curente vor contine valorile culorilor acestui nod
        for(i = a;i <= g;i++)
            for(j = b;j <= d;j++) {
                c[i][j].r = radacina -> r;
                c[i][j].g = radacina -> g;
                c[i][j].b = radacina -> b;
            }
    }
    else
    if(radacina->tip == '0') { //daca e nod intern,zona curenta se divide in alte 4
        unsigned int m1 = (b + d) / 2;
        unsigned int m2 = (a + g) / 2;
        completeaza_matrice(dimensiune,c, a, m2, b, m1, radacina->f1);
        completeaza_matrice(dimensiune,c, a, m2, m1 + 1, d, radacina->f2);
        completeaza_matrice(dimensiune,c, m2 + 1, g, m1 + 1, d, radacina->f3);
        completeaza_matrice(dimensiune,c, m2 + 1, g, b, m1, radacina->f4);
    }

}

void sterge_arbore_cuaternar(TArb radacina) {
    if(radacina != NULL) {
        sterge_arbore_cuaternar(radacina->f1);
        sterge_arbore_cuaternar(radacina->f2);
        sterge_arbore_cuaternar(radacina->f3);
        sterge_arbore_cuaternar(radacina->f4);
        free(radacina);
    }
}


int main(int argc,char *argv[]) {

    char nr_cerinta[7];
    char sir_factor[20]="";
    unsigned long long int factor = 0;
    unsigned int l,k;
    strcpy(nr_cerinta,argv[1]);
    if(strcmp(nr_cerinta,"-c1") == 0 || strcmp(nr_cerinta,"-c2") == 0) {
        strcpy(sir_factor,argv[2]);
        l = strlen(sir_factor)-1;
        k = 0;
        while(k <= l) { //extragem factorul de compresie
            factor = factor*10 + sir_factor[k]-48;
            k++;
        }
    }
    
    //190-195 : calculam factorul de compresie pt primele 2 cerinte

    if(strcmp(nr_cerinta,"-c1") == 0) {

        FILE *f1 = fopen(argv[3],"rb");
        FILE *f2 = fopen(argv[4],"w");
        if(f1 == NULL || f2 == NULL)
            return 0;
        unsigned int latime,inaltime,culoare_maxima;
        unsigned int i,j;
        unsigned char rosu,verde,albastru;
        char sir[10];
        TArb radacina = AlocaNod('0','0','0','0'); //nod santinela pentru arborele cuaternar
                                                                //dupa ce construim arborele,acesta va fi sters
        if(radacina == NULL)
            return 0;
        fscanf(f1,"%s",sir);
        fscanf(f1,"%u",&latime);
        fscanf(f1,"%u",&inaltime);
        fscanf(f1,"%u",&culoare_maxima);
        char newline = '\n';
        fscanf(f1,"%c",&newline);
        TMatrice **a = (TMatrice**)malloc((inaltime+1)*sizeof(TMatrice*));
        if(a == NULL)
            return 0;
        for(i = 0;i<=inaltime;i++) {
            a[i] = (TMatrice*)malloc((latime+1)*sizeof(TMatrice));
            if(a[i] == NULL)
                return 0;
        }
    //folosim indici de la 1 in loc de 0 ca sa evitam eventuale trunchieri la calculul indicilor in functia divide_matrice
    //care sa conduca la valori eronate
        for(i = 1; i <= inaltime; i++) {
            for(j = 1; j<= latime; j++) {
                fread(&rosu,sizeof(char),1,f1);
                fread(&verde,sizeof(char),1,f1);
                fread(&albastru,sizeof(char),1,f1);
                a[i][j].r = rosu;
                a[i][j].g = verde;
                a[i][j].b = albastru;
            }
        }
        int lat_max = -1; //tine minte latura maxima a unei portiuni nedivizate
        divide_matrice( a, 1, inaltime, 1, latime, radacina, 1, factor, &lat_max,f2); //construim arborele de compresie
        TArb aux;
        aux = radacina;
        radacina = radacina->f1;
        free(aux); //in liniile 243-246 scapam de santinela
        int nivel = -1; //tine minte numarul de niveluri
        nr_niveluri(radacina, 0, &nivel);
        nivel++;
        fprintf(f2,"%d\n", nivel);
        fprintf(f2,"%d\n", nr_noduri_terminale(radacina));
        fprintf(f2,"%d\n", lat_max);
        for(i = 0; i <= inaltime;i++)
            free(a[i]);
        free(a); //253-255 : eliberam memoria alocata pentru matricea de pixeli
        sterge_arbore_cuaternar(radacina); //eliberam memoria alocata pentru arborele cuaternar
        fclose(f1);
        fclose(f2);
    }

    if(strcmp(nr_cerinta,"-c2") == 0) {

        FILE *f1 = fopen(argv[3],"rb");
        FILE *f2 = fopen(argv[4],"wb");
        if(f1 == NULL || f2 == NULL)
            return 0;
        unsigned int latime,inaltime,culoare_maxima;
        unsigned int i,j;
        unsigned char rosu,verde,albastru;
        char sir[10];
        TArb radacina = AlocaNod('0','0','0','0'); //santinela pentru arborele cuaternar
        if(radacina == NULL)
            return 0;

        fscanf(f1,"%s",sir);
        fscanf(f1,"%u",&latime);
        fscanf(f1,"%u",&inaltime);
        fscanf(f1,"%u",&culoare_maxima);
        char newline = '\n';
        fscanf(f1,"%c",&newline);
        TMatrice **a = (TMatrice**)malloc((inaltime+1)*sizeof(TMatrice*));
        if(a == NULL)
            return 0;
        for(i = 0;i<=inaltime;i++) {
            a[i] = (TMatrice*)malloc((latime+1)*sizeof(TMatrice));
            if(a[i] == NULL)
                return 0;
        }
        for(i = 1; i <= inaltime; i++) {
            for(j = 1; j<= latime; j++) {
                fread(&rosu,sizeof(char),1,f1);
                fread(&verde,sizeof(char),1,f1);
                fread(&albastru,sizeof(char),1,f1);
                a[i][j].r = rosu;
                a[i][j].g = verde;
                a[i][j].b = albastru;
            }
        }
        int lat_max = 0; //pentru aceasta cerinta nu avem nevoie de determinarea laturii maxime
        divide_matrice(a, 1, inaltime, 1, latime, radacina, 1, factor, &lat_max,f2);
        TArb aux;
        aux = radacina;
        radacina = radacina->f1;
        free(aux); //301-304: stergem santinela
        int nivel = -1;
        nr_niveluri(radacina, 0, &nivel);
        nivel++;
        //306-307:determinam numarul de niveluri

        int *marimi_nivel = (int*)malloc(nivel*sizeof(int));
        if(marimi_nivel == NULL)
            return 0;
        //marimi_nivel[i] semnifica numarul de noduri de pe nivelul de indice i
        marimi_nivel[0] = 1;
        TArb **vector_noduri = (TArb **)malloc(nivel*sizeof(TArb*));
        if(vector_noduri == NULL)
            return 0;
        //vector_noduri[i] tine minte nodurile de pe nivelul de indice i
        vector_noduri[0] = (TArb*)malloc(sizeof(TArb));
        if(vector_noduri[0] == NULL)
            return 0;
        vector_noduri[0][0] = radacina;

        int nr_noduri_de_alocat; //dimensiunea urmatorului nivel

        if(radacina->f1 == NULL)
            nr_noduri_de_alocat = 0;
        else
        if(radacina->f2 == NULL)
            nr_noduri_de_alocat = 1;
        else
        if(radacina->f3 == NULL)
            nr_noduri_de_alocat = 2;
        else
        if(radacina->f4 == NULL)
            nr_noduri_de_alocat = 3;
        else
            nr_noduri_de_alocat = 4;


        for(i = 1;i < nivel;i++) {
            marimi_nivel[i] = nr_noduri_de_alocat;
            vector_noduri[i] = (TArb *)malloc( nr_noduri_de_alocat*sizeof(TArb) );
            if(vector_noduri[i] == NULL)
                return 0;
            for(j = 0;j < nr_noduri_de_alocat;j++)
                vector_noduri[i][j] = NULL;
            int l = 0,contor = 0; //l este indicele ultimului nod de pe nivelul i
            //contor este nr nodurilor neterminale,ne ajuta sa aflam cata memorie trb alocata pe urmatorul nivel
            for(j = 0; j < marimi_nivel[i-1]; j++) { //parcurgem nivelul anterior
                if(vector_noduri[i-1][j] -> tip == '0') {
                    vector_noduri[i][l++] = vector_noduri[i - 1][j]->f1;
                    vector_noduri[i][l++] = vector_noduri[i - 1][j]->f2;
                    vector_noduri[i][l++] = vector_noduri[i - 1][j]->f3;
                    vector_noduri[i][l++] = vector_noduri[i - 1][j]->f4;
                    if(vector_noduri[i - 1][j]->f1->tip == '0')
                        contor++;
                    if(vector_noduri[i - 1][j]->f2->tip == '0')
                        contor++;
                    if(vector_noduri[i - 1][j]->f3->tip == '0')
                        contor++;
                    if(vector_noduri[i - 1][j]->f4->tip == '0')
                        contor++;
                }

            }
            nr_noduri_de_alocat = 4*contor;
        }


        fwrite(&latime,sizeof(unsigned int),1,f2);

        unsigned char tip;

        for(i = 0; i < nivel; i++) {
            for(j = 0; j < marimi_nivel[i];j++) {
                if(vector_noduri[i][j] -> tip == '0') {
                    tip = 0;
                    fwrite(&tip,sizeof(unsigned char),1,f2);
                }

                if(vector_noduri[i][j] -> tip == '1') {
                    tip = 1;
                    fwrite(&tip,sizeof(unsigned char),1,f2);
                    fwrite(&vector_noduri[i][j] ->r,sizeof(unsigned char),1,f2);
                    fwrite(&vector_noduri[i][j] ->g,sizeof(unsigned char),1,f2);
                    fwrite(&vector_noduri[i][j] ->b,sizeof(unsigned char),1,f2);
                }
            }
        }

        for(i = 0; i <= inaltime;i++)
            free(a[i]);
        free(a);
        // 392-394 : eliberam matricea de pixeli
        for(i = 0 ; i < nivel ; i++)
            for(j = 0 ; j < marimi_nivel[i] ; j++)
                free(vector_noduri[i][j]);
        for(i = 0 ; i < nivel ; i++)
            free(vector_noduri[i]);
        free(vector_noduri);
        //396-401: eliberam vectorul de noduri
        free(marimi_nivel); //eliberam vectorul de dimensiuni
        fclose(f1);
        fclose(f2);
    }


    if(strcmp(nr_cerinta,"-d") == 0) {
        FILE *f1 = fopen(argv[2],"rb");
        FILE *f2 = fopen(argv[3],"wb");
        if(f1 == NULL || f2 == NULL)
            return 0;
        unsigned int latura;
        unsigned char x;
        fread(&latura,sizeof(unsigned int),1,f1);
        TLista inceput_lista = aloca_element_lista('9');//santinela pentru lista de informatii
        TLista p = inceput_lista,aux;
        while(!feof(f1))
        {
            fread(&x,sizeof(unsigned char),1,f1);
            aux = aloca_element_lista(x);
            p -> urm = aux;
            p = p -> urm;
        }
        //417-424: construim o lista cu valorile corespunzatoare nodurilor din fisierul binar citit
        aux = inceput_lista;
        inceput_lista = inceput_lista ->urm;
        free(aux);
        //427-429 : stergem santinela

        for(p = inceput_lista;p ->urm->urm!=NULL ;p = p ->urm);
        aux = p->urm;
        p->urm = NULL;
        free(aux);

        int nr_noduri = 0;
        for(p = inceput_lista;p !=NULL ;p = p->urm) {
            if(p->info == 1)
                p = p->urm->urm->urm;
            nr_noduri++;
        }
        //437-441 : numaram cate noduri va avea arborele de compresie

        p = inceput_lista;


        TArb *vector_noduri = (TArb*)malloc(nr_noduri*sizeof(TArb));
        if(vector_noduri == NULL)
            return 0;
        unsigned int i,j;
        for(i = 0;i < nr_noduri;i++) {
            vector_noduri[i] = (TArb)malloc(sizeof(TNod));
            if(vector_noduri[i] == NULL)
                return 0;
        }
        int l = 0; //index-ul curent din lista de noduri
        k = 0;
        while(p) {
            if(p->info == 1) {
                vector_noduri[l]->tip = '1';
                vector_noduri[l]->r = p->urm->info;
                vector_noduri[l]->g = p->urm->urm->info;
                vector_noduri[l]->b = p->urm->urm->urm->info;
                p = p->urm->urm->urm->urm;
            }
            else
            if(p->info == 0) {
                vector_noduri[l]->tip = '0';
                vector_noduri[l]->r = '0';
                vector_noduri[l]->g = '0';
                vector_noduri[l]->b = '0';
                p = p->urm;
            }
            vector_noduri[l]->f1 = NULL;
            vector_noduri[l]->f2 = NULL;
            vector_noduri[l]->f3 = NULL;
            vector_noduri[l]->f4 = NULL;
            l++;
        }


        unsigned short int ultimul_nivel[nr_noduri]; //indicii nodurilor de pe ultimul nivel
        unsigned short int dim_ult_nivel = 1; //dimensiunea ultimului nivel
        unsigned short int numaratoare = 0; //numar cate noduri neterminale am pe ultimul nivel
        unsigned short int nivelul_actual[nr_noduri]; //indicii nodurilor de pe nivelul actual
        unsigned int dim_niv_act; //dimensiunea nivelului actual
        l = 0;
        ultimul_nivel[0] = l;
        l = 1;
        while(l < nr_noduri) //cat timp mai am noduri de adaugat
        {
            for(j = 0; j < dim_ult_nivel; j++)
                if(vector_noduri[ultimul_nivel[j]] ->tip == '0')
                    numaratoare++;
            //494-495: numar cate noduri neterminale am ca sa stiu cate noduri vor fi pe nivelul urmator
            dim_niv_act = 4 * numaratoare;
            i = 0;//indexul curent din vectorul de nivel actual
            for(j = 0;j < dim_ult_nivel;j++)
            {
                if(vector_noduri[ultimul_nivel[j]] ->tip == '0')
                {
                    //leg 4 noduri,fac legaturi si le pun pe cele nou adaugate in vectorul de nivel actual
                    vector_noduri[ultimul_nivel[j]]->f1 = vector_noduri[l];
                    nivelul_actual[i] = l;
                    i++;
                    l++;


                    vector_noduri[ultimul_nivel[j]]->f2 = vector_noduri[l];
                    nivelul_actual[i] = l;
                    i++;
                    l++;


                    vector_noduri[ultimul_nivel[j]]->f3 = vector_noduri[l];
                    nivelul_actual[i] = l;
                    i++;
                    l++;


                    vector_noduri[ultimul_nivel[j]]->f4 = vector_noduri[l];
                    nivelul_actual[i] = l;
                    i++;
                    l++;
                }
            }

            dim_ult_nivel = dim_niv_act;
            int t;
            for(t = 0; t < dim_niv_act;t++) //ultimul nivel devine nivelul actual
                ultimul_nivel[t] = nivelul_actual[t];

            numaratoare = 0;
        }

        TMatrice **c = (TMatrice**)malloc((latura+1)*sizeof(TMatrice*));
        if(c == NULL)
            return 0;
        for(i = 0;i <= latura;i++) {
            c[i] = (TMatrice*)malloc((latura+1)*sizeof(TMatrice));
            if(c[i] == NULL)
                return 0;
        }
        completeaza_matrice(latura,c,1,latura,1,latura,vector_noduri[0]);
        //lista_noduri[0] este radacina arborelui de compresie


        char sir[3]="P6";
        fprintf(f2,"%s\n",sir);
        fprintf(f2,"%u ",latura);
        fprintf(f2,"%u\n",latura);
        unsigned int culoare_maxima = 255;
        fprintf(f2,"%u\n",culoare_maxima);
        for(i = 1; i <= latura; i++) {
            for(j = 1; j<= latura; j++) {
                fwrite(&c[i][j].r,sizeof(unsigned char),1,f2);
                fwrite(&c[i][j].g,sizeof(unsigned char),1,f2);
                fwrite(&c[i][j].b,sizeof(unsigned char),1,f2);
            }
        }
        fclose(f1);
        fclose(f2);

        p = inceput_lista;
        while(p != NULL) {
            aux = p;
            p = p->urm;
            free(aux);
        }

    //565-569: eliberez memeoria pentru lista de informatii

        for(i = 0;i < l;i++)
            free(vector_noduri[i]);
        free(vector_noduri);

        //574-576: eliberez vectorul de noduri
        for(i = 0;i <= latura;i++)
            free(c[i]);
        free(c);

        //579-581 : eliberez spatiul alocat pentru matricea de pixeli
    }

    return 0;
}
