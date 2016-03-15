sudoku: Sudoku.o Solver.o Box.o Batch.o
	mpiCC -o sudoku Sudoku.o Solver.o Box.o Batch.o

Sudoku.o: Sudoku.cpp
	mpiCC -std=c++11 -g -O0 -c -o Sudoku.o Sudoku.cpp

Solver.o: Solver.h Solver.cpp
	mpiCC -std=c++11 -g -O0 -c -o Solver.o Solver.cpp

Box.o: Box.h Box.cpp
	mpiCC -std=c++11 -g -O0 -c -o Box.o Box.cpp

Batch.o: Batch.h Batch.cpp
	mpiCC -std=c++11 -g -O0 -c -o Batch.o Batch.cpp

clean:
	rm -f sudoku
	rm -f Sudoku.o
	rm -f Solver.o
	rm -f Box.o
	rm -f Batch.o
