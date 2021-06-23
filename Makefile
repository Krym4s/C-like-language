all: frontend

frontend: main.o IsE_Interpret.o 
	g++ main.o IsE_Interpret.o -o makeTree