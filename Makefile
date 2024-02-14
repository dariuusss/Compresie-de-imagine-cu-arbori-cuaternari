build:
	gcc -g -Wall -std=gnu11 -lm -o quadtree quadtree.c
run:
	./quadtree
clean:
	rm -rf quadtree
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./quadtree