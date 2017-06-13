link: tools.o main.o linker.o
	g++ -std=c++11 tools.o main.o linker.o -o link
main.o: main.cpp tools.h linker.h
	g++ -std=c++11 -g -c main.cpp
tools.o: tools.h tools.cpp
	g++ -std=c++11 -g -c tools.cpp
linker.o: linker.h tools.h linker.cpp
	g++ -std=c++11 -g -c linker.cpp
clean:
	rm *.o *.gch
	rm link
