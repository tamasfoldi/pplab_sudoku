#pragma once

#include "Box.h"
#include <vector>

class Batch
{
public:
	Batch();
	Batch(const Batch* init);

	~Batch();
	void setWorkBox(Box box);
	void addBoxToRow(Box box);
	void addBoxToColumn(Box box);

	Box getWorkBox();
	std::vector<Box> getBoxesInRow();
	std::vector<Box> getBoxesInColumn();

	void print(std::ostream & s);
private:
	Box workBox;
	std::vector<Box> row;
	std::vector<Box> column;
};

