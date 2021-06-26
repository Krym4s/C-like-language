all: backend

backend: main.o TreeToAsm.o IsE_LinesF.o LogicTree.o
	g++ main.o TreeToAsm.o IsE_LinesF.o LogicTree.o -o  makeAsm

main.o: main.cpp
	g++ -c main.cpp 

TreeToAsm.o: TreeToAsm.cpp
	g++ -c TreeToAsm.cpp

IsE_LinesF.o: IsE_LinesF.cpp
	g++ -c IsE_LinesF.cpp

LogicTree.o: LogicTree.cpp
	g++ -c LogicTree.cpp