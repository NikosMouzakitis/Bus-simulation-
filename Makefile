PROG=main.c

all:
	gcc $(PROG) -lpthread
clean:
	rm a.out
