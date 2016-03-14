sudoku: Sudoku.o Solver.o Box.o
	g++ -o sudoku Sudoku.o Solver.o Box.o

Sudoku.o: Sudoku.cpp
	g++ -std=c++11 -g -O0 -c -o Sudoku.o Sudoku.cpp

Solver.o: Solver.h Solver.cpp
	g++ -std=c++11 -g -O0 -c -o Solver.o Solver.cpp

Box.o: Box.h Box.cpp
	g++ -std=c++11 -g -O0 -c -o Box.o Box.cpp

clean:
	rm -f sudoku
	rm -f Sudoku.o
	rm -f Solver.o
	rm -f Box.o
