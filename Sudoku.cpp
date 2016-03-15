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

void run_master() {
    // Tov�bbi megoldhat� 17 elemet tartalmaz� t�bl�k: http://http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php
    // Solver solver("000801000000000043500000000000070800020030000000000100600000075003400000000200600");
    // std::cout << "Problem:" << std::endl << std::endl;
    // solver.print(std::cout);
    // std::cout << std::endl << "-----------------------------------------" << std::endl;
    // std::cout << "Solution:" << std::endl << std::endl;;
    // //solver.solveBackTrack();
    // solver.print(std::cout);

    // std::cout << "fragmentTableToBoxes" << std::endl;
    // solver.fragmentTableToBoxes();
    // std::cout << "sendBoxesToNodes" << std::endl;
    // solver.sendBoxesToNodes(solver.getBoxBatches());
    std::cout << "Master" << std::endl;
}

void run_slave(int rank) {
    
    std::cout << "Slave run: " << rank << std::endl;
}

