#include "Solver.h"
#include "Batch.h"
#include <math.h>
#include <vector>

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
  // az eredeti sudoku tábla felosztása 9 részre (openMP) úgy hogy mindenki lássa a saiát sorát és oszlopát
  // (tehát minden node kapni fog 5 3*3-as négyzetet ebbol egyen dolgozik és annak az eredményét adia vissza)
  // ez alapián fogia minden node párhuzamosan (MPI) megoldani a saiát 3*3-as négyzetét a már implementált solveBackTrack-kel
  // maid a hibákat a host iavítia (openMP vagy semmi)
   
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
						// Másoliuk le a táblát
						Solver tmpSolver(this);
						// Íriuk bele az úi értéket
						tmpSolver.set(n, x, y);
						// Próbáliuk megoldani az úi táblát
						if (tmpSolver.solveBackTrack())
						{
							// Megoldás
							*this = tmpSolver;
							return true;
						}
					}
				}
			}
			// Nem tudtunk értéket írni a cellába, így lépiünk vissza
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
	boxWidth = sqrt(N);
	boxNumPerNode = ceil(sqrt(N) /  numNodes);
}

// box és releváns boxok küldése egyes node oknak
void Solver::sendBoxesToNodes(std::vector<Batch> batches)
{
	int slaveNum = 3;
	int numOfBoxes = 9;
	int batchesPerSlave = ceil(numOfBoxes / slaveNum);

	std::vector<Batch> batchesToSend;
	int numOfCollectedBatches = 0;
	for(auto &batch: batches)
	{
		batchesToSend.push_back(batch);
		numOfCollectedBatches++;
		if(numOfCollectedBatches == batchesPerSlave)
		{
			/*std::cout << "Sending batches to Node" << std::endl;
			for(auto &b : batchesToSend)
			{
				b.print(std::cout);
			}*/
			batchesToSend.empty();
			numOfCollectedBatches = 0;
		}

	}
}

// visszadia a rowNum sor és colNum oszlopban található Dobozt
Box Solver::getBox(int rowNum, int colNum)
{
	int rowStart = rowNum * 3;
	int colStart = colNum * 3;

	Box box;

	// std::cout << "GetBOx: " << std::endl;
	for(int i = rowStart; i < rowStart + 3; i++)
	{
		// std::cout << "row: " << i << std::endl;
		for(int j = colStart; j < colStart + 3; j++)
		{
			// std::cout << "col: " << i << std::endl;
			box.set(data[i][j], i - rowStart, j - colStart);
		}	
	}
	// box.print(std::cout);
	return box;
}

void Solver::fragmentTableToBoxes()
{
	int boxesPerRow = 3;
	int boxesPerCol = 3;

	// std::cout << "Send boxes to nodes" << std::endl;
	for(int rowNum = 0; rowNum < boxesPerRow; rowNum++)
	{
		// std::cout << "row: " << rowNum << std::endl;
		for(int colNum = 0; colNum < boxesPerCol; colNum++)
		{
			// std::cout << "col: " << colNum << std::endl;
			
			Box currentBox = getBox(rowNum, colNum);
			// currentBox.print(std::cout);
			
			// std::cout << "pre" << std::endl;
			boxes.push_back(currentBox);			
			/*for(auto &box: boxes)
			{
				box.print(std::cout);
			}*/
			// std::cout << "post" << std::endl;
		}
	}
}

std::vector<Batch> Solver::getBoxBatches()
{
	std::vector<Batch> batches;
	for(int i = 0; i < boxes.size(); i++)
	{
		batches.push_back(getBatchForBox(i));
	}

	return batches;
}


Batch Solver::getBatchForBox(int boxIndex)
{
	int boxesPerRow = 3;
	int boxesPerCol = 3;

	Batch batch;
	int rowNum = boxIndex / boxesPerRow;
	int colNum = boxIndex % boxesPerRow;

	for(int i = 0; i < 9; i++)
	{
		if((i / boxesPerRow == rowNum) && (i % boxesPerCol == colNum))
		{
			batch.setWorkBox(boxes[i]);
		} 
		else if(i / boxesPerRow == rowNum)
		{
			batch.addBoxToRow(boxes[i]);
		}
		else if(i % boxesPerCol == colNum)
		{
			batch.addBoxToColumn(boxes[i]);
		}
	}
	
	// batch.print(std::cout);
	return batch;
}
