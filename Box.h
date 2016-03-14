#pragma once

#include <iostream>

class Box
{
public:
	Box();
	Box(const char** init);
	Box(const Box* init);

	~Box();
	void set(char val, int x, int y);
	void print(std::ostream & s);

private:
	char cells[3][3];
};

