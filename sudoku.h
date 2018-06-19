#ifndef SUDOKU_H
#define SUDOKU_H
#include <iostream>
class Sudoku {
public:
    Sudoku();
    Sudoku(const int init_map[]);
    void setMap(const int set_map[]);
    int getElement(int x, int y);
    bool isCorrect();
    bool notWrong(); //check map whether has 2 same # in a line
    bool solve(Sudoku & ans);
    bool superSolve(Sudoku & ans);
    int findMaxIndex(int arr[], int index);
    static const int sudokuSize = 81;

private:
    bool checkUnity(int arr[], int status = 0);
    int map[sudokuSize];

};

#endif // SUDOKU_H
