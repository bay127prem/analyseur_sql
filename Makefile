all: analyseur_sql

analyseur_sql: analex.o anasynt.o main.o
	gcc -o analyseur_sql analex.o anasynt.o main.o

analex.o: analex.c analyseur_sql.h
	gcc -o analex.o -c analex.c -W -Wall

anasynt.o: anasynt.c analyseur_sql.h
	gcc -o anasynt.o -c anasynt.c -W -Wall

main.o: main.c analyseur_sql.h
	gcc -o main.o -c main.c -W -Wall

clean:
	rm -rf *.o

mrproper: clean
	rm -rf analex
