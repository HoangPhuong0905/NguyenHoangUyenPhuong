all: dsa172a1

dsa172a1: dbLib.o requestLib.o main.o processData.o
	g++ -g main.o dbLib.o requestLib.o processData.o -std=c++11 -o dsa172a1
main.o: main.cpp listLib.h dbLib.h
	g++ -g -c main.cpp -std=c++11 $(DEFS)
requestLib.o: requestLib.cpp requestLib.h listLib.h
	g++ -g -c requestLib.cpp -std=c++11 $(DEFS)
dbLib.o: dbLib.cpp dbLib.h listLib.h requestLib.h
	g++ -g -c dbLib.cpp -std=c++11 $(DEFS)
processData.o: processData.cpp listLib.h requestLib.h dbLib.h
	g++ -g -c processData.cpp -std=c++11 $(DEFS)

clean:
	rm *.o dsa172a1*
