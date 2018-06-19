#include "pencil.h"

using namespace std;

Pencil::Pencil(){
//reset
    for(int i = 0; i < 9; ++i){
        n[i] = 0;
    }
}

Pencil::Pencil(int set_map[]){
    //reset
    for(int i = 0; i < 9; ++i){
        n[i] = 0;
    }

    for(int i = 0; i < 81; ++i){
        map[i] = set_map[i];
    }
}

void Pencil::setMap(int set_map[]){
    for(int i = 0; i < 9; ++i){
        n[i] = 0;
    }

    for(int i = 0; i < 81; ++i){
        map[i] = set_map[i];
    }
}
QString Pencil::getPencil(int row, int col){
    QString out = NULL, num;
    int i, j;

    //initial n[] be true
    for(i = 0; i < 9; ++i){
        n[i] = true;
    }
    //if the cell has number
    if(map[col+9*row] > 0){
        for(i = 0; i < 9; ++i){
            n[i] = false;
        }
        out = QString::number(map[col+9*row]);
        return out;
    }

    //check whether the row has the #
    for(i = row*9; i < 9 + row*9; ++i){
        if(map[i] != 0){
            n[map[i]-1] = false;
        }
    }

    //check whether the column has the #
    for(j = col; j < 81; j = j + 9){
        if(map[j] != 0){
            n[map[j]-1] = false;
        }
    }

    //check whether the big cell has the #
    int nw_col, nw_row; //the north-west cell of big cell
    int nw_index; //0, 3, 6, 27, 30, 33, 54, 57, 60

    if(col % 3 == 1){
        nw_col = col - 1;
    } else if(col % 3 == 2){
        nw_col = col - 2;
    } else {
        nw_col = col;
    }

    if(row % 3 == 1){
        nw_row = row - 1;
    } else if(row % 3 == 2){
        nw_row = row - 2;
    } else {
        nw_row = row;
    }

    nw_index = nw_col + nw_row*9;
    for(j = 0; j < 3; nw_index += 9, ++j){
        for(i = 0; i < 3; ++i){
            if(map[nw_index + i] > 0){
                n[map[nw_index + i]-1] = false;
            }
        }
    }

    //output pencil by String
    for(i = 0; i < 9; ++i){
        if(n[i] == true){
            num = QString::number(i+1);
            out = out + num;
        }
    }

    return out;
}
