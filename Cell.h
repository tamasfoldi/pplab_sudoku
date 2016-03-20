#pragma once

#include <vector>
#include <iostream>

class Cell
{
public:
    Cell(int maxPossibleValue, int box, int cell); // n is MaxPossibleValue
    Cell(char val, int maxPossibleValue, int box, int cell);
    Cell(char val, std::vector<char> possibleValues, int box, int cell);
    Cell(Cell *c);
    ~Cell();

    char getValue();    
    void setValue(char val);

    std::vector<char> getPossibleValues();

    void addPossibleValue(char newPossVal);
    void removePossibleValue(char val);

    void setPossibleValues(std::vector<char> newPossibleValues);
    void clearPossibleValues();

    void print(std::ostream & s);
    
    int getBoxNumber();
    int getCellNumber();

private:
    char value;
    int boxNumber;
    int cellNumber;
    std::vector<char> possibleValues;    
};

