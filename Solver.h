#pragma once

#include <iostream>
#include "Box.h"

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

	void sendBoxesToNodes();
	void collectPossibleValues(char* box, char** relevantBoxes);
private:

	Box getBox(int boxRowNum,int boxColNum);			
	Box* getRelevantBoxes(int boxRowNum,int boxColNum);

	char data[9][9];
	char possibleValues[9][9][9];

	int N;
	int boxWidth;

	int numNodes = 4;
	int boxNumPerNode;
};