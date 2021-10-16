all : main

main : main.o my_mv.o my_ls.o
	gcc my_mv.o main.o my_ls.o -o main

main.o : main.c

my_mv.o : my_mv.c my_mv.h

my_ls.o : my_ls.c my_ls.h

clean:
	rm *.o 
