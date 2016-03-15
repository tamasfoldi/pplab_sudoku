#include "Box.h"

Box::Box()
{
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			cells[y][x] = 0;
		}
	}
}

Box::Box(const char ** init)
{

	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			cells[y][x] = init[y][x] - '0';
		}
	}
}

Box::Box(const Box * init)
{
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			cells[y][x] = init->cells[y][x];
		}
	}
}

Box::Box(std::vector<char> data)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			cells[i][j] = data[i * 3 + j];
		}
	}
}


Box::~Box()
{
}

void Box::set(char val, int x, int y)
{
	cells[x][y] = val;
}

/*
void Box::printBox(std::ostream & s, char** box)
{
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			s << (char)(box[x][y] + '0') << " ";
		}
		s << std::endl;
	}
	s << std::endl;
}
*/
void Box::print(std::ostream & s)
{
	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			s << (char)(cells[x][y] + '0') << " ";
		}
		s << std::endl;
	}
	s << std::endl;
}

std::vector<char> Box::getCells()
{
	std::vector<char> ret;
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			ret.push_back(cells[i][j]);
		}
	}

	return ret;
}