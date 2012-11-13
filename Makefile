# Dirty Makefile
all:
	gcc -c src/*.c
	gcc *.o -o ttt

clean:
	rm -fv *.o ttt
