all: proc

proc: main.o IsE_Processor.o IsE_Stack.o IsE_text.o
	g++ main.o IsE_Processor.o IsE_Stack.o IsE_text.o -o proc

main.o: main.cpp
	g++ -c main.cpp 

IsE_Processor.o: IsE_Processor.cpp
	g++ -c IsE_Processor.cpp

IsE_Stack.o: IsE_Stack.cpp
	g++ -c IsE_Stack.cpp

IsE_text.o: IsE_text.cpp
	g++ -c IsE_text.cpp