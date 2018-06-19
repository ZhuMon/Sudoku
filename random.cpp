#include "random.h"
#include "sudoku.h"

Random::Random(int difficulty)
{
    Sudoku su;
    int i, j;
    int rand_index;
    //int tmp_map[81] = {0};

    for(i = 0; i < 81; ++i){
        map[i] = 0;
    }

    srand( (unsigned)time(NULL) );
    //rand_index = rand() % 81; //0~80
    //tmp_map[rand_index] = rand() % 9 + 1; //1~9
    //su.setMap(tmp_map);
    //su.superSolve(su);
/*
    for(i = 0; i < 9; ++i){ //col
        for(j = 0; j < 9; ++j){ //row
            map[i+j*9] = su.getElement(i, j);
        }
    }

    for(i = 0; i < 50; ++i){  //make 50 random cell to 0
        rand_index = rand() % 81; //0~80
        map[rand_index] = 0;
    }
*/

    int mem;

    for(mem = 0; mem < 10; ++mem){
        do{
            rand_index = rand() % 81; //0~80
        }while(map[rand_index] > 0);

        j = 0;

        do{
            map[rand_index] = rand() % 9 + 1; //1~9
            su.setMap(map);
            ++j;
        }while(!su.notWrong() && j < 20);

        if(j == 20){
            map[rand_index] = 0;
        }
    }

    su.superSolve(su);
    for(i = 0; i < 81; ++i){
        map[i] = su.getElement(i%9, i/9);
    }

    for(i = 0; i < 81-difficulty; ++i){
        rand_index = rand() % 81; //0~80
        map[rand_index] = 0;
    }

}

int Random::getElement(int row, int col){
    return map[row*9 + col];
}

int Random::getElement(int index){
    return map[index];
}
