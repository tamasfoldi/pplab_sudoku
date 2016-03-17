#include "Batch.h"

Batch::Batch()
{}

Batch::Batch(const Batch * init)
{
	workBox = init->workBox;
	row = init->row;
	column = init->column;
}

Batch::~Batch() {}

void Batch::initPossibleValues()
{
	int n = 9;
	std::set<char> values;
	for(int i = 1; i <= n; ++i)
	{
		values.insert(i);
	}

	for(int i = 0; i < n; i++)
	{
		possibleValues.push_back(values);
	}
}

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
	
void Batch::calculatePossibleValues()
{
	//workBox;
	//row;
	//column;
	//print(std::cout);
	//std::cout << row[0].getCells()[0];
	std::set<char> workBoxValues;
	for(auto &cellValue: workBox.getCells())
	{
		if(cellValue != '0')
		{
			workBoxValues.insert(cellValue);
		}
	}

	std::vector<std::set<char>> rowValues;
	std::vector<std::set<char>> colValues;
	int numRows = 3;
	int n = 9;
	for(int i = 0; i < numRows; i++)
	{
		rowValues.push_back(std::set<char>());
		colValues.push_back(std::set<char>());
	}

	for(auto &box: row)
	{
		for(int i = 0; i < n; i++)
		{
			if(box.getCells()[i] != 0)
			{
				rowValues[i / numRows].insert(box.getCells()[i]);
				// std::cout <<  box.getCells()[i] << " ";
			}
		}
		//std::cout<< std::endl;
	}

	for(auto &box: column)
	{
		for(int i = 0; i < n; i++)
		{
			if(box.getCells()[i] != 0)
			{
				colValues[i % numRows].insert(box.getCells()[i]);
				// std::cout <<  box.getCells()[i] << " ";
			}
		}
		//std::cout<< std::endl;
	}

	for(int i = 0; i < n; i++)
	{
		auto valueSet = possibleValues[i];
		
    	std::cout << std::endl;
    	
		valueSet = substractSet(valueSet, workBoxValues);		
		valueSet = substractSet(valueSet, rowValues[i / numRows]);
		valueSet = substractSet(valueSet, colValues[i % numRows]);

		std::cout << "After " << std::endl;
		for(auto &c: valueSet)
    	{
    		std::cout << (char)(c + '0');
    	}
    	std::cout << std::endl;
	}
	/*
	for(auto &row: colValues) 
	{
		for(auto &value: row)
		{
			std::cout << (char)(value + '0') << " ";
		}
		std::cout<< std::endl;
	}*/
	/*std::set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(),
    	std::inserter(result, result.end()));*/
}

std::vector<std::set<char>> Batch::getPossibleValues()
{
	return possibleValues;
}

std::set<char> Batch::substractSet(std::set<char> set1, std::set<char> set2)
{
	std::set<char> tmp = set1;
	for(auto &element: set2)
	{
		tmp.erase(element);
	}

	return tmp;
}