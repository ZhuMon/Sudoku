#ifndef RANDOM_H
#define RANDOM_H
#include "sudoku.h"

class Random
{
public:
    Random(int difficulty = 40);
    int getElement(int row, int col);
    int getElement(int index);

private:
    int map[81];
};

#endif // RANDOM_H
