#include "Cell.h"

Cell::Cell(int maxPossibleValue, int box, int cell)
{
    this->boxNumber = box;
    this->cellNumber = cell;
    value = '0';    
    for(int i = 1; i <= maxPossibleValue; i++)
    {
        addPossibleValue('0' + i);
    }
}

Cell::Cell(Cell* c) {
    boxNumber = c->getBoxNumber();
    cellNumber = c->getCellNumber();
    value = c->getValue();
    possibleValues = c->getPossibleValues();
}

Cell::Cell(char val, int maxPossibleValue, int box, int cell)
{
    this->boxNumber = box;
    this->cellNumber = cell;
    value = val;
    for(int i = 1; i <= maxPossibleValue; i++)
    {
        addPossibleValue('0');
    }

}

Cell::Cell(char val, std::vector<char> possValues, int box, int cell)
{
    this->boxNumber = box;
    this->cellNumber = cell;
    value = val;
    possibleValues = possValues;
}


Cell::~Cell()
{

}


char Cell::getValue()
{
    return value;
}
    
void Cell::setValue(char val)
{    
    value = val;
    if(val != '0')
    {
        clearPossibleValues();
    }
}


std::vector<char> Cell::getPossibleValues()
{
    return possibleValues;
}


void Cell::addPossibleValue(char newPossVal)
{
    bool found = false;
    for(auto &val: possibleValues)
    {
        if(val == newPossVal)
        {
            found = true;
            break;
        }
    }
    if(!found) {
        possibleValues.push_back(newPossVal);
    }
}

void Cell::removePossibleValue(char possVal)
{
    std::vector<char> newPossibleValues;
    for(auto &val: possibleValues)
    {
        if(val != possVal)
        {
            newPossibleValues.push_back(val);            
        }
    }
    possibleValues = newPossibleValues;
}

void Cell::setPossibleValues(std::vector<char> newPossValues)
{
    possibleValues =  newPossValues;
}

void Cell::clearPossibleValues()
{
    possibleValues.clear();
}

void Cell::print(std::ostream & s)
{
    s << "Position: BoxNumber" << boxNumber << ", CellNumber" << cellNumber << std::endl;
    s << "Value: " << (char)(value) << std::endl;
    s << "Possible values: ";
    for(auto &posVal: possibleValues)
    {
        s << (char)('0' + posVal) << " ";
    }
    s << std::endl << std::endl;
}

int Cell::getBoxNumber() {
    return boxNumber;
}

int Cell::getCellNumber() {
    return cellNumber;
}
