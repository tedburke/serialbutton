all: serialbutton.exe

serialbutton.exe: serialbutton.o
	gcc -o serialbutton.exe serialbutton.o

serialbutton.o: serialbutton.c
	gcc -c serialbutton.c
