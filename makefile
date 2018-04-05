Project4: pro4.o
	gcc -Wall -g -o pro4 pro4.o

Project_4.0: pro4.c
	gcc -Wall -g -c pro4.c

clean:
	rm -i pro4.o pro4
