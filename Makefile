all: myls shell

CFLAGS=-Wall -g

shell: shell.c stringlist.c stringlist.h
	gcc shell.c stringlist.c ${CFLAGS} -o shell

myls: myls.c
	gcc myls.c ${CFLAGS} -o myls

clean:
	rm -f  shell myls *.o 