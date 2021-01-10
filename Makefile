CC=@g++
FLAGS=-std=c++14 -Wall -O2
SRC=ShellStart.cpp ShellExit.cpp ShellPCB.cpp ShellUtility.cpp ShellProcessAction.cpp
OBJ=$(SRC:.cpp=.o)

shell379: $(OBJ)
	$(CC) $(FLAGS) -o shell379 $(OBJ)

ShellStart.o: ShellStart.cpp
	$(CC) $(FLAGS) -c -o ShellStart.o ShellStart.cpp
ShellExit.o: ShellExit.cpp
	$(CC) $(FLAGS) -c -o ShellExit.o ShellExit.cpp
ShellPCB.o: ShellPCB.cpp
	$(CC) $(FLAGS) -c -o ShellPCB.o ShellPCB.cpp
ShellUtility.o: ShellUtility.cpp
	$(CC) $(FLAGS) -c -o ShellUtility.o ShellUtility.cpp
ShellProcessAction.o: ShellProcessAction.cpp
	$(CC) $(FLAGS) -c -o ShellProcessAction.o ShellProcessAction.cpp

clean:
	@-rm -f *.o shell379
