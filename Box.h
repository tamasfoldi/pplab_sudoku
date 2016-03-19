#pragma once

#include <iostream>
#include <vector>

class Box
{
public:
	Box();
	Box(const char** init);
	Box(const Box* init);

	
	Box(std::vector<char> data);

	~Box();
	void set(char val, int x, int y);
	void print(std::ostream & s);

	std::vector<char> getCells();
private:
	char cells[3][3];

};

