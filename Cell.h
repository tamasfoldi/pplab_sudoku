#pragma once

#include <vector>
#include <iostream>

class Cell
{
public:
	Cell(int maxPossibleValue); // n is MaxPossibleValue
	Cell(char val, int maxPossibleValue);
	Cell(char val, std::vector<char> possibleValues);

	~Cell();

	char getValue();	
	void setValue(char val);

	std::vector<char> getPossibleValues();

	void addPossibleValue(char newPossVal);
	void removePossibleValue(char val);

	void setPossibleValues(std::vector<char> newPossibleValues);
	void clearPossibleValues();

	void print(std::ostream & s);

private:
	char value;
	std::vector<char> possibleValues;	
};

