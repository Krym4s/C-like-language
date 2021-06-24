all: frontend

frontend: main.o IsE_Interpret.o IsE_LinesF.o
	g++ main.o IsE_Interpret.o -o IsE_LinesF.o makeTree

main.o: main.cpp
	g++ -c main.cpp 

IsE_Interpret.o: IsE_Interpret.cpp
	g++ -c IsE_Interpret.cpp

IsE_LinesF.o: IsE_LinesF.cpp
	g++ -c IsE_LinesF.cpp