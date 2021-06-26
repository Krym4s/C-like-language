all: asm

asm: main.o IsE_text.o IsE_Assembler.o IsE_Stack.o
	g++ -o asm main.o IsE_text.o IsE_Assembler.o IsE_Stack.o

main.o: main.cpp
	g++ -g -c  main.cpp

IsE_Assembler.o: IsE_Assembler.cpp
	g++ -g -c IsE_Assembler.cpp

IsE_Stack.o: IsE_Stack.cpp
	g++ -g -c IsE_Stack.cpp

IsE_text.o: IsE_text.cpp
	g++ -g -c IsE_text.cpp

clean:
	rm *.o 