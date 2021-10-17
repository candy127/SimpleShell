all : main

main : main.o 
	gcc main.o -o main

main.o : main.c

clean:
	rm *.o  main
