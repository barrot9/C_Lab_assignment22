myset: myset.o set.o helpset.o
	gcc -ansi -Wall -pedantic myset.o set.o helpset.o -o myset
myset.o: myset.c set.h
	gcc -c -ansi -Wall -pedantic myset.c -o myset.o
set.o: set.c set.h
	gcc -c -ansi -Wall -pedantic set.c -o set.o
helpset.o: helpset.c set.h
	gcc -c -ansi -Wall -pedantic helpset.c -o helpset.o

