mont: tools.o main.o Swombat.o
	g++ -std=c++11 tools.o main.o Swombat.o -o mont
main.o: main.cpp tools.h Swombat.h
	g++ -std=c++11 -g -c main.cpp
tools.o: tools.h tools.cpp
	g++ -std=c++11 -g -c tools.cpp
Swombat.o: Swombat.h tools.h Swombat.cpp
	g++ -std=c++11 -g -c Swombat.cpp
clean:
	rm *.o *.gch
	rm mont
