#include "Batch.h"

Batch::Batch(){}

Batch::Batch(const Batch * init)
{
	workBox = init->workBox;
	row = init->row;
	column = init->column;
}

Batch::~Batch() {}

void Batch::setWorkBox(Box box)
{
	this->workBox = box;
}


void Batch::setRow(std::vector<Box> newRow)
{
	this->row = newRow;
}

void Batch::setColumn(std::vector<Box> newColumn)
{
	this->column = newColumn;
}

void Batch::addBoxToRow(Box box)
{
	this->row.push_back(box);
}

void Batch::addBoxToColumn(Box box)
{
	this->column.push_back(box);
}

Box Batch::getWorkBox()
{
	return this->workBox;
}

std::vector<Box> Batch::getBoxesInRow()
{
	return this->row;
}

std::vector<Box> Batch::getBoxesInColumn()
{
	return this->column;
}

void Batch::print(std::ostream & s)
{
	s << "Work box:" << std::endl;
	this->workBox.print(s);

	s << "Row: " << std::endl;
	for(auto &box: this->row)
	{
		box.print(s);
	}

	s << "Column: " << std::endl;
	for(auto &box: this->column)
	{
		box.print(s);
	}
}
	