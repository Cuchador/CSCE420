CC = g++

make: DPLL.o DPLL

DPLL: DPLL.o
	$(CC) -o DPLL DPLL.o

DPLL.o: DPLL.cpp
	$(CC) -c DPLL.cpp

clean:
	$(RM) DPLL DPLL.o