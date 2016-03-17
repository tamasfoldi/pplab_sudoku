#pragma once

#include "Box.h"
#include <vector>
#include <set>

class Batch
{
public:
	Batch();
	Batch(const Batch* init);

	~Batch();

	void initPossibleValues();

	void setWorkBox(Box box);
	void setRow(std::vector<Box> newRow);
	void setColumn(std::vector<Box> newColumn);

	void addBoxToRow(Box box);
	void addBoxToColumn(Box box);

	Box getWorkBox();
	std::vector<Box> getBoxesInRow();
	std::vector<Box> getBoxesInColumn();

	void print(std::ostream & s);

	void calculatePossibleValues();
	std::vector<std::set<char>> getPossibleValues();
	std::set<char> substractSet(std::set<char> set1, std::set<char> set2);
	
private:
	Box workBox;
	std::vector<Box> row;
	std::vector<Box> column;

	std::vector<std::set<char>> possibleValues;
};

