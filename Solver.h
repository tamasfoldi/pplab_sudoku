#pragma once

#include <iostream>
#include <vector>
#include "Box.h"
#include "Batch.h"
#include "Cell.h"

class Solver
{
public:
    Solver(int n);
    Solver(const char* init, int n);
    Solver(const Solver* init);
    // Solver(int n, const Solver* init);

    ~Solver();

    void print(std::ostream &s);
    // void printBox(std::ostream &s, char** box);
    bool isSolved();
    bool isAllowed(char val, int x, int y);
    bool solveBackTrack();

    void set(char val, int x, int y);

    void initSize(int n);

    void sendBoxesToNodes(std::vector<Batch> batches);

    void fragmentTableToBoxes();
    std::vector<Batch> getBoxBatches();
    
    bool isEqual(Solver s);
    
    std::vector<Box> getBoxes();
    
    int getN();
private:

    Batch getBatchForBox(int boxIndex);
    Box getBox(int boxRowNum,int boxColNum);
    
    std::vector<Box> boxes;

    char ** data;

    int N;
    int boxWidth;

    int numNodes = 4;
    int boxNumPerNode;
};