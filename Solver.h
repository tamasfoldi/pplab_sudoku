#pragma once

#include <iostream>

class Solver
{
public:
	Solver();
	Solver(const char* init);
	Solver(const Solver* init);
	// Solver(int n, const Solver* init);

	~Solver();

	void print(std::ostream &s);

	bool isSolved();
	bool isAllowed(char val, int x, int y);
	bool solveBackTrack();

	void set(char val, int x, int y);

	void initSize(int n);
private:
	void sendBoxesToNodes();
	void collectPossibleValues(char* box, char** relevantBoxes);

	char* getBox(int boxRowNum,int boxColNum);			
	char** getRelevantBoxes(int boxRowNum,int boxColNum);

	char data[9][9];
	char possibleValues[9][9][9];

	int N;
	int boxWidth;

	const int numNodes = 4;
	int boxNumPerNode;
};

