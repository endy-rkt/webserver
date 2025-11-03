test: main.s
	as main.s -o main.o
	ld main.o -o webserver

clean:
	rm -rf main.o

clear: clean
	rm -rf webserver