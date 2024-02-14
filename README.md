# Compresie-de-imagine-cu-arbori-cuaternari


TNod este tipul de date utilizat pentru implementarea arborelui cuaternar.

TMatrice este tipul de date utilizat pentru matricea de pixeli.

Lista este tipul de date utilizat pentru lista de informatii pentru noduri de la cerinta 3.

Functia AlocaNod aloca si initializeaza un nod din arborele cuaternar.

Functia aloca_element_lista aloca si initializeaza un element din lista de informatii utilizata in cadrul cerintei 3.

Functia divide_matrice primeste ca parametri matricea de pixeli,nodul la care ar putea eventual sa ataseze niste fii,
un indicator pentru fiul corespunzator unei eventuale atasari,factorul de compresie,adresa variabilei in
care se va retine dimensiunea maxima a unui patrat nedivizat si fisierul de output.Aceasta functie construieste arborele de compresie si calculeaza latura maxima a unui patrat nedivizat si
scorurile corespunzatoare culorilor,prezentand doua scenarii:nodul curent este terminal sau intern.Daca
e terminal,nu se mai executa apeluri recursive pentru impartirea matricei de pixeli.Daca nodul este intern,atunci
se calculeaza 2 indici de mijloc pentru linie,respectiv coloana in zona curenta din matrice si se apeleaza recursiv
functia de inca 4 ori,fiecare apel fiind corespunzator unui sfert din zona curenta a matricei de pixeli.

Functia nr_niveluri calculeaza indicele ultimului nivel din arborele de compresie.

Functia nr_noduri_terminale numara cate noduri terminale are arborele de compresie.

Functia completeaza_matrice primeste ca parametri dimensiunea matricei de pixeli,matricea de pixeli,
indicii corespunzatori primei linii,ultimei linii,primei coloane si ultimei coloane din zona curenta a matricei de pixeli si
nodul din arborele de compresie corespunzator acelei zone din matrice,avand ca efect completarea corespunzatoare a matricei de pixeli pentru imagine.Daca nodul pentru care am apelat functia e terminal,
atunci toti pixelii din zona curenta din matrice vor avea pe canalele de culoare aceleasi valori ca si nodul mentionat anterior.Daca acesta este intern,
se imparte recursiv zona curenta in 4 sferturi.

Functia sterge_arbore_cuaternar elibereaza memoria alocata pentru arborele cuaternar.Ca sa nu pierdem legaturile catre fii,mai intai stergem toti cei
4 subarbori ai nodului pentru care am apelat functia si la final il stergem si pe acesta.

Pentru rezolvarea cerintei 1 citim matricea de pixeli,construim arborele de compresie si determinam statisticile cerute.

Pentru rezolvarea cerintei 2 citim matricea de pixeli,construim arborele de compresie si determinam parcurgerea in latime a acestuia.Pentru asta vom construi o matrice de niveluri in care indicele i
contine toate nodurile de pe nivelul i(radacina e pe nivelul 0).Porim de la nindicele 0,pe care inseram radacina.Pe linia de indice 1 se vor afla fiii radacinii.Pe linia de indice 2 se vor afla toti copiii nodurilor de pe nivelul 1 si tot asa. Cu alte cuvinte,
pe nivelul k se afla toti copiii nodurilor de pe nivelul k-1,iar numarul total de noduri de pe nivelul k este 4* numarul nodurilor neterminale de pe nivelul k-1.
La final,parcurgem matricea de niveluri de la stanga la dreapta si de sus in jos si scriem datele nodurilor in fisierul binar cerut.

Pentru rezolvarea cerintei 3 citim datele din fisier si salvam valorile corespunzatoare nodurilor intr-o lista.Daca elementul curent este 0,atunci avem nod intern,il construim si avansam in lista cu o pozitie.
Daca valoarea curenta este 1,atunci avem nod terminal,il construim si avansam in lista cu 4 pozitii,deoarece urmatoarele 3 sunt valorile culorilor pentru nodul respectiv.Repetam procesul pana cand terminam de parcurs lista.
Dupa ce am aflat cate noduri avem,repetam procedeul anterior,dar de data aceasta si construim un vector de noduri,care va contine nodurile arborelui de compresie,dar fara legaturi realizate.
Dupa ce avem si nodurile arborelui de compresie il vom construi,intr-o maniera asemanatoare cu cea utilizata la obtinerea parcurgerii in latime de la cerinta 2:pornim de la radacina,punem fiii acesteia pe urmatorul nivel si repetam procedeul pana cand toate nodurile sunt inserate in arbore.
Singura diferenta este ca de data aceasta obtinem direct arborele in locul unei reprezentari a acestuia printr-o matrice care retine parcurgerea sa in latime.
La final,alocam spatiu pentru matricea de pixeli si apelam functia completeaza_matrice care o sa initializeze pixelii cu valorile corespunzatoare din nodurile arborelui de compresie.


