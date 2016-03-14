#include "Solver.h"
#include <math.h>

Solver::Solver()
{
	initSize(9);
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			data[y][x] = 0;
		}
	}
}

Solver::Solver(const char * init)
{
	initSize(9);
	for (int i = 0; i < 81; ++i)
	{
		int x = i % 9;
		int y = i / 9;
		data[y][x] = init[i] - '0';
	}
}

Solver::Solver(const Solver * init)
{
	initSize(9);
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			data[y][x] = init->data[y][x];
		}
	}
}


Solver::~Solver()
{
}

void Solver::print(std::ostream & s)
{
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			s << (char)(data[y][x] + '0') << " ";
		}
		s << std::endl;
	}
}

bool Solver::isSolved()
{
	// Minden cella ki van töltve a táblában?
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			if (data[y][x] == 0) return false;
		}
	}
	return true;
}

bool Solver::isAllowed(char val, int x, int y)
{
	bool allowed = true;

	// Azonos sorban vagy oszlopban csak egy 'val' lehet
	for (int i = 0; i < 9; ++i)
	{
		if (data[y][i] == val) allowed = false;
		if (data[i][x] == val) allowed = false;
	}

	// Az adott 3x3-as cellában csak egy 'val' lehet
	int cellBaseX = 3 * (int)(x / 3);
	int cellBaseY = 3 * (int)(y / 3);
	for (int y = cellBaseY; y < cellBaseY + 3; ++y)
	{
		for (int x = cellBaseX; x < cellBaseX + 3; ++x)
		{
			if (data[y][x] == val) allowed = false;
		}
	}

	return allowed;
}
/*
void Solver::solverSuggestion() {
  // 1.
  // az eredeti sudoku tábla felosztása 9 részre (openMP) úgy hogy mindenki lássa a saját sorát és oszlopát
  // (tehát minden node kapni fog 5 3*3-as négyzetet ebbol egyen dolgozik és annak az eredményét adja vissza)
  // ez alapján fogja minden node párhuzamosan (MPI) megoldani a saját 3*3-as négyzetét a már implementált solveBackTrack-kel
  // majd a hibákat a host javítja (openMP vagy semmi)
   
}*/

bool Solver::solveBackTrack()
{
	// Készen vagyunk?
	if (isSolved())
	{
		return true;
	}

	// Keressünk egy pozíciót, amely még nincs kitöltve
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			// Nincs még kitöltve?
			if (data[y][x] == 0)
			{
				// Keressünk egy értéket, amely megfelel a szabályoknak
				for (int n = 1; n <= 9; ++n)
				{
					// Beírható az adott pozícióba?
					if (isAllowed(n, x, y))
					{
						// Másoljuk le a táblát
						Solver tmpSolver(this);
						// Írjuk bele az új értéket
						tmpSolver.set(n, x, y);
						// Próbáljuk megoldani az új táblát
						if (tmpSolver.solveBackTrack())
						{
							// Megoldás
							*this = tmpSolver;
							return true;
						}
					}
				}
			}
			// Nem tudtunk értéket írni a cellába, így lépjünk vissza
			if (data[y][x] == 0) return false;
		}
	}

	return false;
}

void Solver::set(char val, int x, int y)
{
	data[y][x] = val;
}

void Solver::initSize(int n) 
{
	N = n;
	boxWidth = N;
	boxNumPerNode = ceil(sqrt(N) /  numNodes);
}

// box és releváns boxok küldése egyes node oknak
void Solver::sendBoxesToNodes()
{
	int boxesInRow = boxWidth;
	int boxesInCol = boxWidth;

	for(int rowNum = 0; rowNum < boxesInRow; rowNum++)
	{
		for(int colNum = 0; colNum < boxesInCol; colNum++)
		{
			// char box[boxWidth * boxWidth] = getBox(rowNum, colNum);			
			// char relevantBoxes[boxesInRow] = getRelevantBoxes(rowNum, colNum);
			// send boxes
		}
	}

}

// visszadja a rowNum sor és colNum oszlopban található Doboxt
char* Solver::getBox(int rowNum, int colNum)
{

}

// visszadja a rowNum sor és colNum oszlopban található Dobozhoz tartozó lehetséges
// értékek szempontjából fontos dobozokat
char** Solver::getRelevantBoxes(int rowNum,int colNum)
{

}