// Sudoku.cpp : Defines the entry point for the console application.

#include <iostream>
#include <mpi.h>
#include "Solver.h"

namespace crf {
    class mpi_guard {
    public:
    explicit mpi_guard() { MPI_Init( nullptr, nullptr ); }
    ~mpi_guard() { MPI_Finalize(); }

    };
} // namespace crf


void run_master() {
    // Tov�bbi megoldhat� 17 elemet tartalmaz� t�bl�k: http://http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php
    Solver solver("000801000000000043500000000000070800020030000000000100600000075003400000000200600");
    // std::cout << "Problem:" << std::endl << std::endl;
    solver.print(std::cout);
    // std::cout << std::endl << "-----------------------------------------" << std::endl;
    // std::cout << "Solution:" << std::endl << std::endl;;
    // //solver.solveBackTrack();
    // solver.print(std::cout);

    std::cout << "fragmentTableToBoxes" << std::endl;
    solver.fragmentTableToBoxes();
    std::cout << "sendBoxesToNodes" << std::endl;
    solver.sendBoxesToNodes(solver.getBoxBatches());
   //std::cout << "Master" << std::endl;
}

void run_slave(int rank) {    
    MPI_Status status;
    
	int sizeWorkBox = 9;
	int sizeBoxesInRow = 2 * 9;
	int sizeBoxesInColumn = 2 * 9;
	int batchesToReceive = 1;
    
    auto workBoxData = std::vector<char>(9);
    auto rowData = std::vector<Box>(2);
    auto columnData = std::vector<Box>(2);

    std::vector<Batch> batches;

   	for(int i = 0; i < batchesToReceive; i++)
   	{
	  	MPI_Recv(workBoxData.data(), sizeWorkBox, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		std::cout << "WorkBox Received, " << rank << std::endl;
	  	MPI_Recv(rowData.data(), sizeBoxesInRow, MPI_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
	  	std::cout << "RowData Received, " << rank << std::endl;
	  	MPI_Recv(columnData.data(), sizeBoxesInColumn, MPI_CHAR, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
	  	std::cout << "ColumnData Received, " << rank << std::endl;
	  	// std::cout << "TAG: " << MPI_ANY_TAG << std::endl;
  	
	  	Box workBox(workBoxData);
	  	//workBox.print(std::cout);

	  	Batch batch;
	  	batch.setWorkBox(workBox);
	  	batch.setRow(rowData);
	  	batch.setColumn(columnData);        
        batch.initPossibleValues();
        batches.push_back(batch);
  	}

    //for (int i = 0; i < batchesToReceive; ++i) {
    for (int i = 0; i < batchesToReceive; ++i) {
        batches[i].calculatePossibleValues();
    }
}


int main()
{
    auto const mpi_guard = crf::mpi_guard{};
    int wrank;
    MPI_Comm_rank( MPI_COMM_WORLD, &wrank );
    switch( wrank ) {
        case 0:
            run_master();
            break; 
        default:
            run_slave( wrank );
    }
    return 0;

}

