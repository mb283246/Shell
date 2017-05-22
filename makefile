shell402: main.o
	gcc main.o -o shell402

main.o: main.c
	gcc -c main.c

clean:
	rm -f *.o core sicxe 
