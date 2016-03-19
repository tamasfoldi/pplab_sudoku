// Sudoku.cpp : Defines the entry point for the console application.

#include <iostream>
#include <mpi.h>
#include "Solver.h"
#include "Cell.h"

namespace crf {
    class mpi_guard {
    public:
    explicit mpi_guard() { MPI_Init( nullptr, nullptr ); }
    ~mpi_guard() { MPI_Finalize(); }

    };
} // namespace crf

std::vector<char> setToVector(std::set<char> set)
{
    std::vector<char> res;
    int setSize = set.size();
    int i = 0;
    for(auto &val: set)
    {
        res.push_back(val);
        i++;
    }
    if(9 - i > 0)
    {
        for (int j = i; j < 9; ++j)
        {
            res.push_back(0);
        }
    }

    return res;
}

void run_master() {
    // Tov�bbi megoldhat� 17 elemet tartalmaz� t�bl�k: http://http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php
    Solver solver("000801000000000043500000000000070800020030000000000100600000075003400000000200600");
    // std::cout << "Problem:" << std::endl << std::endl;
    solver.print(std::cout);
    // std::cout << std::endl << "-----------------------------------------" << std::endl;
    // std::cout << "Solution:" << std::endl << std::endl;;
    // solver.solveBackTrack();
    // solver.print(std::cout);

    std::cout << "fragmentTableToBoxes" << std::endl;
    solver.fragmentTableToBoxes();
    std::cout << "sendBoxesToNodes" << std::endl;   
    solver.sendBoxesToNodes(solver.getBoxBatches());
    std::cout << "Master" << std::endl;

    int batchesToReceivePerNode = 3;
    int maxRank = 3;

    std::vector<std::vector<Cell>> allPossibleValues;
    int n;
    for(int i = 1; i <= maxRank; i++)
    {
        std::cout << "Rank: " << maxRank << std::endl;
        for (int j = 0; j < batchesToReceivePerNode; ++j) {
            int tag = i * 10 + j + 1;
            int siize = 9;
            std::cout <<  "Tag: " << tag <<  std::endl;

            std::vector<Cell> cellsOfBox;
            for(int k = 0; k < 9; k++)
            {
                std::cout << "CellNum: " << k << std::endl;
                std::vector<char> c = std::vector<char>(9);
                MPI_Status status;

                MPI_Recv(c.data(), siize, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);   
                Box possibleValuesOfACell(c);                
                possibleValuesOfACell.print(std::cout);     
                Cell newCell(n);
                newCell.setPossibleValues(possibleValuesOfACell.getCells());
                cellsOfBox.push_back(newCell);
            }
            allPossibleValues.push_back(cellsOfBox);
        }
    }

    // for all boxes
    int boxNum = 0;
    for(auto &box: allPossibleValues)
    {
        int cellNum = 0;        
        for(auto &cell: box)
        {
            std::cout << "Box Num: " << boxNum <<  ", Cell Num: " << boxNum << std::endl;
            cell.print(std::cout);            
            cellNum++;
        }
        boxNum++;
    }
}

void run_slave(int rank) {    
    MPI_Status status;
    
	int sizeWorkBox = 9;
	int sizeBoxesInRow = 2 * 9;
	int sizeBoxesInColumn = 2 * 9;
	int batchesToReceive = 3;
    
    auto workBoxData = std::vector<char>(9);
    auto rowData = std::vector<Box>(2);
    auto columnData = std::vector<Box>(2);

    std::vector<Batch> batches;

   	for(int i = 0; i < batchesToReceive; i++)
   	{
	  	MPI_Recv(workBoxData.data(), sizeWorkBox, MPI_CHAR, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		//std::cout << "WorkBox Received, " << rank << std::endl;
	  	MPI_Recv(rowData.data(), sizeBoxesInRow, MPI_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
		//std::cout << "RowData Received, " << rank << std::endl;
	  	MPI_Recv(columnData.data(), sizeBoxesInColumn, MPI_CHAR, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
	    //std::cout << "ColumnData Received, " << rank << std::endl;
	  	// std::cout << "TAG: " <<  << std::endl;

	  	Box workBox(workBoxData);
	  	//workBox.print(std::cout);

	  	Batch batch;
        batch.setWorkBox(workBoxData);
	  	batch.setRow(rowData);
	  	batch.setColumn(columnData); 

        batch.initPossibleValues();
          
        batches.push_back(batch);
  	}
   
    //std::cout << "BEGIN FOR slaves" << std::endl;
    for (int i = 0; i < batchesToReceive; ++i) {
        int tag = rank * 10 + i + 1; // azért +1 hogy 1 based legyen mind a két digit
        batches[i].calculatePossibleValues(); 
        for(int j = 0; j < 9; j++) {
        
            int vSize = 9;
            //std::cout<<"DOFOR slave" << std::endl;
            //batches[0].getWorkBox().print(std::cout);
            MPI_Send(setToVector(batches[i].getPossibleValues()[j]).data(), vSize, MPI_CHAR, 0, tag, MPI_COMM_WORLD);    
        }
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

