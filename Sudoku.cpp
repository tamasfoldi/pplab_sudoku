// Sudoku.cpp : Defines the entry point for the console application.

#include <iostream>
#include <algorithm>
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

bool findInVector(std::vector<char> vec, char c) {
    for(auto &v: vec) {
        if (c == v){
            return true;
        }
    }
    return false;
}

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

Solver run_master(Solver solver) {
    // Tov�bbi megoldhat� 17 elemet tartalmaz� t�bl�k: http://http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php
    // std::cout << "Problem:" << std::endl << std::endl;
    solver.print(std::cout);
    std::cout<<std::endl<<std::endl;
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
                Cell newCell(n, j, k);
                newCell.setPossibleValues(possibleValuesOfACell.getCells());
                cellsOfBox.push_back(newCell);
            }
            allPossibleValues.push_back(cellsOfBox);
        }
    }
    
    //for all boxes
    int boxNum = 0;
    for(auto &box: allPossibleValues)
    {
        int cellNum = 0;        
        for(auto &cell: box)
        {
            std::cout << "Box Num: " << boxNum <<  ", Cell Num: " << cellNum << std::endl;
            cell.print(std::cout);            
            cellNum++;
        }
        boxNum++;
    }
    
    for(int i = 0; i < allPossibleValues.size(); i++) {
        for(int j = 0; j < allPossibleValues[i].size(); j++) {
            if(allPossibleValues[i][j].getPossibleValues()[1] == 0) {
                solver.set(allPossibleValues[i][j].getPossibleValues()[0], (i % 3)*3 + (j % 3), (j / 3) + ((i / 3)*3));
            } else {
                bool found = false;
                for(auto &possibleValue: allPossibleValues[i][j].getPossibleValues()) {
                    if(possibleValue != 0) {
                        for(int l = 0; l < allPossibleValues.size(); l++) {
                            if(l / 3 == i / 3) {
                                for(int m = 0; m < allPossibleValues[l].size(); m++) {
                                    if(allPossibleValues[i][j].getCellNumber() / 3 == allPossibleValues[l][m].getCellNumber() / 3 &&
                                    (i != l || m != j) && findInVector(allPossibleValues[l][m].getPossibleValues(), possibleValue)) {    
                                        found = true; 
                                        break; 
                                    } 
                                }
                                if(found) {
                                    break;
                                }
                            } 
                        }
                        if(found) { 
                            found = false;                            
                            for(int l = 0; l < allPossibleValues.size(); l++) { 
                                if(l % 3 == i % 3) {
                                    for(int m = 0; m < allPossibleValues[l].size(); m++) { 
                                        if(allPossibleValues[i][j].getCellNumber() % 3 == allPossibleValues[l][m].getCellNumber() % 3 &&
                                        (i != l || m != j)  &&  findInVector(allPossibleValues[l][m].getPossibleValues(), possibleValue)) {                                        
                                            found = true;
                                            break;
                                        } 
                                    }
                                    if(found) {
                                        break;
                                    }
                                } 
                            } 
                        } 
                        if(!found) {
                            solver.set(possibleValue,(i % 3)*3 + (j % 3), (j / 3) + ((i / 3)*3));
                            allPossibleValues[i][j].clearPossibleValues();
                            allPossibleValues[i][j].addPossibleValue(possibleValue);
                            break;
                        } else {
                            found = false;
                        }
                    } 
                }
            }
        }
    }
    

    
    solver.print(std::cout);
    std::cout<<std::endl<<std::endl;
    return solver;
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
          std::cout<< "RECV" << std::endl;
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
    Solver solver("030700050008005700000630004004100300000000000290400060001000439080002000900000180");
    
    MPI_Comm_rank( MPI_COMM_WORLD, &wrank );
    while(!solver.isSolved()){
        
        switch( wrank ) {
            case 0:
                solver = run_master(solver);
                break; 
            default:
                run_slave( wrank );
        }
    }
    
    return 0;

}

