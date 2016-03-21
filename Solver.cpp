#include "Solver.h"
#include "Batch.h"
#include <mpi.h>
#include <math.h>
#include <vector>

Solver::Solver(int n)
{
    initSize(n);
    for (int y = 0; y < n; ++y)
    {
        for (int x = 0; x < n; ++x)
        {
            data[y][x] = 0;
        }
    }
}

Solver::Solver(const char * init, int n)
{
    initSize(n);
    for (int i = 0; i < n * n ; ++i)
    {
        int x = i % n;
        int y = i / n;
        data[y][x] = init[i] - '0';
    }
}

Solver::Solver(const Solver * init)
{
    initSize(init->N);
    for (int y = 0; y < N; ++y)
    {
        for (int x = 0; x < N; ++x)
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
    for (int y = 0; y < N; ++y)
    {
        for (int x = 0; x < N; ++x)
        {
            s << (char)(data[y][x] + '0') << " ";
        }
        s << std::endl;
    }
}

bool Solver::isSolved()
{
    // Minden cella ki van t�ltve a t�bl�ban?
    for (int y = 0; y < N; ++y)
    {
        for (int x = 0; x < N; ++x)
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
    for (int i = 0; i < N; ++i)
    {
        if (data[y][i] == val) allowed = false;
        if (data[i][x] == val) allowed = false;
    }

    // Az adott 3x3-as cell�ban csak egy 'val' lehet
    int cellBaseX = boxWidth * (int)(x / boxWidth);
    int cellBaseY = boxWidth * (int)(y / boxWidth);
    for (int y = cellBaseY; y < cellBaseY + boxWidth; ++y)
    {
        for (int x = cellBaseX; x < cellBaseX + boxWidth; ++x)
        {
            if (data[y][x] == val) allowed = false;
        }
    }

    return allowed;
}

bool Solver::solveBackTrack()
{
    // K�szen vagyunk?
    if (isSolved())
    {
        return true;
    }

    // Keress�nk egy poz�ci�t, amely m�g nincs kit�ltve
    for (int y = 0; y < N; ++y)
    {
        for (int x = 0; x < N; ++x)
        {
            // Nincs m�g kit�ltve?
            if (data[y][x] == 0)
            {
                // Keress�nk egy �rt�ket, amely megfelel a szab�lyoknak
                for (int n = 1; n <= N; ++n)
                {
                    // Be�rhat� az adott poz�ci�ba?
                    if (isAllowed(n, x, y))
                    {
                        // M�soliuk le a t�bl�t
                        Solver tmpSolver(this);
                        // �riuk bele az �i �rt�ket
                        tmpSolver.set(n, x, y);
                        // Pr�b�liuk megoldani az �i t�bl�t
                        if (tmpSolver.solveBackTrack())
                        {
                            // Megold�s
                            *this = tmpSolver;
                            return true;
                        }
                    }
                }
            }
            // Nem tudtunk �rt�ket �rni a cell�ba, �gy l�pi�nk vissza
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

    data = new char*[N];
    for(int i = 0; i < N; i++)
    {
        data[i] = new char[N];
    }
}

// box �s relev�ns boxok k�ld�se egyes node oknak
void Solver::sendBoxesToNodes(std::vector<Batch> batches)
{
    int slaveNum = 3;

    int batchesPerSlave = ceil(N / slaveNum);

    std::vector<Batch> batchesToSend;
    int numOfCollectedBatches = 0;
    int slaveId = 1;
    for(auto &batch: batches)
    {
        // batchesToSend.push_back(batch);
        numOfCollectedBatches++;

        int N = 9;
        int sizeBoxesInRow = (boxWidth - 1) * N;
        int sizeBoxesInColumn = (boxWidth - 1) * N;
        // std::cout << "CollectedBatches " << numOfCollectedBatches << " slaveId: " << slaveId << "batchesPerSlave: " << batchesPerSlave << std::endl;

        MPI_Send(batch.getWorkBox().getCells().data(), N, MPI_CHAR, slaveId, 0, MPI_COMM_WORLD);
        MPI_Send(batch.getBoxesInRow().data(), sizeBoxesInRow, MPI_CHAR, slaveId, 1, MPI_COMM_WORLD);
        MPI_Send(batch.getBoxesInColumn().data(), sizeBoxesInColumn, MPI_CHAR, slaveId, 2, MPI_COMM_WORLD);
        
        if(numOfCollectedBatches == batchesPerSlave)
        {
            slaveId++;
            numOfCollectedBatches = 0;
        }

    }
}

// visszadia a rowNum sor �s colNum oszlopban tal�lhat� Dobozt
Box Solver::getBox(int rowNum, int colNum)
{
    int rowStart = rowNum * boxWidth;
    int colStart = colNum * boxWidth;

    Box box;

    // std::cout << "GetBOx: " << std::endl;
    for(int i = rowStart; i < rowStart + boxWidth; i++)
    {
        // std::cout << "row: " << i << std::endl;
        for(int j = colStart; j < colStart + boxWidth; j++)
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
    int boxesPerRow = boxWidth;
    int boxesPerCol = boxWidth;
    boxes.clear();

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
    int boxesPerRow = boxWidth;
    int boxesPerCol = boxWidth;

    Batch batch;
    int rowNum = boxIndex / boxesPerRow;
    int colNum = boxIndex % boxesPerRow;

    for(int i = 0; i < N; i++)
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

std::vector<Box> Solver::getBoxes() {
    return boxes;
}

bool Solver::isEqual(Solver s) {
    if(s.getBoxes().size() != getBoxes().size())
        return false;
    for(int i = 0; i < getBoxes().size(); i++) {
        if(!getBoxes()[i].isEqual(s.getBoxes()[i]))
            return false;
    }
    
    return true;
}

int Solver::getN()
{
    return N;
}