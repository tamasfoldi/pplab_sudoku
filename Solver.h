#pragma once

#include <iostream>
#include <vector>
#include "Box.h"
#include "Batch.h"

class Solver
{
public:
	Solver();
	Solver(const char* init);
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
	void collectPossibleValues(char* box, char** relevantBoxes);

	void fragmentTableToBoxes();
	std::vector<Batch> getBoxBatches();

private:

	Batch getBatchForBox(int boxIndex);
	Box getBox(int boxRowNum,int boxColNum);			

	std::vector<Box> boxes;

	char data[9][9];
	char possibleValues[9][9][9];

	int N;
	int boxWidth;

	int numNodes = 4;
	int boxNumPerNode;
};