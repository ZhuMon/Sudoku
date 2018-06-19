#include "sudoku.h"
#include "pencil.h"

#include <QString>
#include <cmath>

using namespace std;

Sudoku::Sudoku(){
    for(int i = 0; i < sudokuSize; ++i){
        map[i] = 0;
    }
}

Sudoku::Sudoku(const int init_map[]){
    for(int i = 0; i < sudokuSize; ++i){
        map[i] = init_map[i];
    }
}

void Sudoku::setMap(const int set_map[]){
    for(int i = 0; i < sudokuSize; ++i){
        map[i] = set_map[i];
    }
}

int Sudoku::getElement(int x, int y){
    return map[x + 9*y];
}

bool Sudoku::checkUnity(int arr[], int status){
    int arr_unity[9];

    for(int i = 0; i < 9; ++i){
        arr_unity[i] = 0;
    }
    for(int i = 0; i < 9; ++i){
        ++arr_unity[arr[i]-1];
    }
    for(int i = 0; i < 9; ++i){
        switch(status){
        case 0:
            if(arr_unity[i] != 1)  //exactly correct
                return false;
            break;
        case 1:
            if(arr_unity[i] > 1)  //not wrong
                return false;
            break;
        default:
            break;
        }

    }
    return true;
}

bool Sudoku::isCorrect(){
    bool check_result;
    int check_arr[9];
    int location;
    for(int i = 0; i < 81; i += 9){ //check row
        for(int j = 0; j < 9; ++j){
            check_arr[j] = map[i+j];
        }
        check_result = checkUnity(check_arr);
        if(check_result == false){
            return false;
        }
    }

    for(int i = 0; i < 9; ++i) {// check columns
        for(int j = 0; j < 9; ++j){
            check_arr[j] = map[i+9*j];
        }
        check_result = checkUnity(check_arr);
        if(check_result == false){
            return false;
        }
    }
    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            location = 27*(i/3) + 3*(i%3) + 9*(j/3) + (j % 3);
            check_arr[j] = map[location];
        }
        check_result = checkUnity(check_arr);

        if(check_result == false){
            return false;
        }

    }
    return true;
}

bool Sudoku::notWrong(){
    bool check_result;
    int check_arr[9];
    int location;
    for(int i = 0; i < 81; i += 9){ //check row
        for(int j = 0; j < 9; ++j){
            check_arr[j] = map[i+j];
        }
        check_result = checkUnity(check_arr, 1);
        if(check_result == false){
            return false;
        }
    }

    for(int i = 0; i < 9; ++i) {// check columns
        for(int j = 0; j < 9; ++j){
            check_arr[j] = map[i+9*j];
        }
        check_result = checkUnity(check_arr, 1);
        if(check_result == false){
            return false;
        }
    }

    for(int i = 0; i < 9; ++i){
        for(int j = 0; j < 9; ++j){
            location = 27*(i/3) + 3*(i%3) + 9*(j/3) + (j % 3);
            check_arr[j] = map[location];
        }
        check_result = checkUnity(check_arr, 1);

        if(check_result == false){
            return false;
        }

    }

    Pencil pen(map);
    for(int i = 0; i < 81; ++i){
        if(pen.getPencil(i/9, i%9) == ""){
            return false;
        }
    }

    QString str = NULL;

    //check row whether it can be every number
    for(int i = 0; i < 9; ++i){ //row
        bool checkPencil[9] = {0};
        for(int j = 0; j < 9; ++j){ //col
            str = pen.getPencil(i, j);
            for(int k = 0; k < str.length(); ++k){
                checkPencil[str.mid(k, 1).toInt() - 1] = true;
            }
        }
        for(int j = 0; j < 9; ++j){
            if(!checkPencil[j]){
                return false;
            }

        }
    }

    //check col whether it can be every number
    for(int j = 0; j < 9; ++j){ //col
        bool checkPencil[9] = {0};
        for(int i = 0; i < 9; ++i){ //row
            str = pen.getPencil(i, j);
            for(int k = 0; k < str.length(); ++k){
                checkPencil[str.mid(k, 1).toInt() - 1] = true;
            }
        }
        for(int i = 0; i < 9; ++i){
            if(!checkPencil[i]){
                return false;
            }

        }
    }

    //check big cell whether it can be every number
    for(int i = 0; i < 9; ++i){
        bool checkPencil[9] = {0};
        for(int j = 0; j < 9; ++j){
            location = 27*(i/3) + 3*(i%3) + 9*(j/3) + (j % 3);
            str = pen.getPencil(location/9, location%9);
            for(int k = 0; k < str.length(); ++k){
                checkPencil[str.mid(k, 1).toInt() - 1] = true;
            }
        }
        for(int j = 0; j < 9; ++j){
            if(!checkPencil[j]){
                return false;
            }

        }
    }


    return true;

}

bool Sudoku::solve(Sudoku & ans){
    int i, j, k;
    int count[10] = {0}; //count times of each number appear
    int maxCount, maxIndex;
    int count_num = 0;

    for(i = 0; i < 81; ++i){
        ans.map[i] = map[i];

        if(map[i] > 0)  ++count_num;
    }

    if(count_num < 17) return false; //no unique solution

    //find which big row has most member
    for(i = 0, k = 0; i < 81; i = i + 27, ++k){
        for(j = 0; j < 27; ++j){
            if(map[i + j] > 0){
                count[k] = count[k] + 1;
            }
        }
    }

    //find which big column has most member
    for(k = 3; k < 6; ++k){  //big column
        for(i = 0; i < 3; ++i ){ //column
            for(j = 0; j < 81; j = j + 9){  //each row
                if(map[i + j + (k-3)*3] > 0){
                    count[k] = count[k] + 1;
                }
            }
        }
    }

    maxCount = findMaxIndex(count, 6);

next_maxCount:


    Pencil p(map);
    QString str;
    //has checked all big c/r
    j = 0;
    for(i = 0; i < 6; ++i){
        if(count[i] == 0){
            ++j;
        }
    }
    if(j == 6){
        goto the_other_sol;
    }
{
    int count_ele[10] = {0};
    int ele_num = 0; //count # in the r/c
    int b_cell_row = 0; //record the blank cell row
    int b_cell_col = 0; //record the blank cell column

    if(maxCount < 3){  //check row to find which # appear more
        for(i = 0; i < 3; ++i){
            ele_num = 0;
            for(j = 0; j < 9; ++j){
                if(map[i*9 + j + maxCount*27] > 0){
                    ++ele_num;
                }
                else{
                    b_cell_row = i + maxCount*3;
                    b_cell_col = j;
                }
            }

            //the line only has one blank cell

            if(ele_num == 8 && map[b_cell_col + b_cell_row*9] == 0){
                str = p.getPencil(b_cell_row, b_cell_col);
                if(str == NULL){return false;}
                ans.map[b_cell_col + b_cell_row*9] = str.toInt();
                return true;
            }
        }

        //check whether there is a big cell has 8 #
        for(i = 0; i < 3; ++i) { //big cell
            ele_num = 0;
            for(j = 0; j < 3; ++j){ //row
                for(k = 0; k < 3; ++k){ //column
                    if(map[i*3 + j*9 + k + maxCount*27] > 0){
                        ++ele_num;
                    }
                    else{
                        b_cell_row = j + maxCount*3;
                        b_cell_col = i*3 + k;
                    }
                }
            }

            if(ele_num == 8 && map[b_cell_col + b_cell_row*9] == 0){
                str = p.getPencil(b_cell_row, b_cell_col);
                if(str == NULL){return false;}
                ans.map[b_cell_col + b_cell_row*9] = str.toInt();
                return true;
            }

        }

        for(i = 0; i < 27; ++i){
            count_ele[map[i + maxCount*27]]
                    = count_ele[map[i + maxCount*27]] + 1;
        }
    } else if(maxCount < 6){  //check column
        for(i = 0; i < 3; ++i){
            ele_num = 0;
            for(j = 0; j < 9; ++j){
                if(map[i + 9*j + (maxCount-3)*3] > 0){
                    ++ele_num;
                }
                else{
                    b_cell_row = j;
                    b_cell_col = i + (maxCount-3)*3;
                }
            }

            //the line only has one blank cell
            str = p.getPencil(b_cell_row, b_cell_col);
            if((ele_num == 8 || str.size() == 1)&& map[b_cell_col + b_cell_row*9] == 0){
                ans.map[b_cell_col + b_cell_row*9] = str.toInt();
                if(str == NULL){return false;}
                return true;
            }
        }

        //check whether there is a big cell has 8 #
        for(i = 0; i < 3; ++i) { //big cell
            ele_num = 0;
            for(j = 0; j < 3; ++j){ //row
                for(k = 0; k < 3; ++k){ //column
                    if(map[i*27 + j*9 + k + (maxCount-3)*3] > 0){
                        ++ele_num;
                    }
                    else{
                        b_cell_row = i*3 + j;
                        b_cell_col = k + (maxCount-3)*3;
                    }
                }
            }

            if(ele_num == 8 && map[b_cell_col + b_cell_row*9] == 0){
                str = p.getPencil(b_cell_row, b_cell_col);
                if(str == NULL){return false;}
                ans.map[b_cell_col + b_cell_row*9] = str.toInt();
                return true;
            }

        }

        for(j = 0; j < 81; j = j + 9){
            for(k = 0; k < 3; ++k){
                count_ele[map[j + k + (maxCount-3)*3]]
                        =count_ele[map[j + k + (maxCount-3)*3]]+1;
            }
        }
    }

    //get index of big c/r, which most appear
    maxIndex = findMaxIndex(count_ele, 10);

    while(count_ele[maxIndex] >= 3){ //appear 3 times
        count_ele[maxIndex] = 0;
        maxIndex = findMaxIndex(count_ele, 10);
    }

    int w = -1; //limit while loop times
    while(w < 9){
        ++w;



        int check = -1; //check 3 r/c which hasn't the maxIndex
        int record[2] = {0}; //record position of two maxIndex
        int r = -1; //for record

        if(maxCount < 3){  //row
            for(j = 0; j < 3; ++j){       //3 rows
                for(i = 0; i < 9; ++i){   //9 cols
                    if(map[i + j*9 + maxCount*27] == maxIndex){
                        record[++r] = i + j*9 + maxCount*27;
                        break;
                    }
                }
                if(i == 9){  //can't find maxIndex in the row
                    check = j + 3*maxCount;
                }
            }
        }else if(maxCount < 6){  //column
            for(i = 0; i < 3; ++i){      //3 cols
                for(j = 0; j < 81; j = j + 9){  //9 rows
                    if(map[i + j + (maxCount-3)*3] == maxIndex){
                        record[++r] = i + j + (maxCount-3)*3;
                        break;
                    }
                }
                if(j >= 81){ //can't find maxIndex in the column
                    check = i + (maxCount-3)*3;
                }
            }
        }

        int x1, y1, x2, y2; // x:col y:row
        int sum;  //record sum of index

        x1 = record[0] % 9;
        y1 = record[0] / 9;

        //if r < 0
        //the big c/r hasn't the maxIndex
        if(r < 0) {
            continue;
        } else if( r == 0){ //the big c/r has only one maxIndex
            count[maxCount] = 0;
            maxCount = findMaxIndex(count, 6);
            goto next_maxCount;
        }
        if(r > 0){
            x2 = record[1] % 9;
            y2 = record[1] / 9;
        }
        int no_MI_Line = 0;  //the other line that no maxIndex
        int no_MI_Big_Cell = 0; //the other big cell that has no maxIndex

        if(maxCount < 3){
            //find the other line that no maxIndex
            //get row of the big row that has no maxIndex
            no_MI_Line = check;
            /*sum = (y1 - maxCount*3) + (y2 - maxCount*3);
        switch (sum) {
        case 1: no_MI_Line = 2 + maxCount*3;  //0, 1
            break;
        case 2: no_MI_Line = 1 + maxCount*3;  //0, 2
            break;
        case 3: no_MI_Line = 0 + maxCount*3;  //1, 2
            break;
        default:
            break;
        }*/

            //find the other big cell that has no maxIndex
            //get the first column of the big cell
            sum = (x1/3) + (x2/3);

        } else if(maxCount < 6){
            //find the other line that no maxIndex
            //get column of the big column that has no maxInde
            no_MI_Line = check;

            //find the other big cell that has no maxIndex
            //get the first row of the big cell
            sum = (y1/3) + (y2/3);
        }

        switch (sum) {
        case 1: no_MI_Big_Cell = 6;  //012, 345
            break;
        case 2: no_MI_Big_Cell = 3;  //012, 678
            break;
        case 3: no_MI_Big_Cell = 0;  //345, 678
            break;
        default:
            break;
        }

        //Pencil p(map); //defined after label"next_maxCount"
        QString num = QString::number(maxIndex);
        bool cell_can_be_MI[3] = {0};  //whether the cell can be maxIndex
        int MI_num = 0; //number of the cell can be maxIndex
        int record_pos[3] = {0}; //record the three position that maybe is maxIndex
        int rp = -1; //for record_pos to check index


        if(maxCount < 3){

            for(i = 0; i < 3; ++i){
                //already has value in cell
                if(map[no_MI_Line*9 + no_MI_Big_Cell + i] > 0){
                    continue;
                }

                //if the cell can be maxIndex(have pencil of maxIndex)
                if(p.getPencil(no_MI_Line, no_MI_Big_Cell + i).contains(num)){
                    //record
                    cell_can_be_MI[i] = true;
                    ++MI_num;
                    record_pos[++rp] = no_MI_Line*9 + no_MI_Big_Cell + i;
                    continue;
                }
            }




        } else if(maxCount < 6){
            for(i = 0; i < 3; ++i){
                //already has value in cell
                if(map[no_MI_Line + no_MI_Big_Cell*9 + i*9] > 0){
                    continue;
                }

                //if the cell can be maxIndex(have pencil of maxIndex)
                if(p.getPencil(no_MI_Big_Cell+i, no_MI_Line).contains(num)){
                    //record
                    cell_can_be_MI[i] = true;
                    ++MI_num;
                    record_pos[++rp] = no_MI_Line + no_MI_Big_Cell*9 + i*9;
                    continue;
                }
            }
        }

        if(MI_num == 1){ //solve one cell
            ans.map[record_pos[0]] = maxIndex;
            return true;
        }

        count_ele[maxIndex] = 0;
        maxIndex = findMaxIndex(count_ele, 10);
    }
}
the_other_sol:
    for(i = 0; i < 9; ++i){
        for(j = 0; j < 9; ++j){
            if(getElement(j,i) == 0){
                str = p.getPencil(i, j);
                if(str.size() == 1){
                    ans.map[9*i + j] = str.toInt();
                    return true;
                }
            }
        }
    }


    //check cell in row which can only be #
    //for(k = 1; k <= 9; ++k){ //# 1~9
    for(i = 0; i < 9; ++i){ //row
        int check[9] = {0};
        int rj[9] = {0}; //record j
        for(j = 0; j < 9; ++j){ //col
            if(map[9*i + j] == 0){
                str = p.getPencil(i, j);
                for(k = 0; k < 9; ++k){ //# 1~9
                    if(str.contains(QString::number(k+1))){
                        ++check[k];
                        if(check[k] > 1){
                            continue;
                        }
                        rj[k] = j; //k only can put in (i, rj[k])
                    }
                }
            }
        }
        for(k = 0; k < 9; ++k){
            if(check[k] == 1 ){
                ans.map[9*i + rj[k]] = k+1;
                return true;
            }
        }
    }

    //check cell in column which can only be #
    //for(k = 1; k <= 9; ++k){ //# 1~9
    for(i = 0; i < 9; ++i){ //col
        int check[9] = {0};
        int rj[9] = {0}; //record j
        for(j = 0; j < 9; ++j){ //row
            if(map[i + 9*j] == 0){
                str = p.getPencil(j, i);
                for(k = 0; k < 9; ++k){
                    if(str.contains(QString::number(k+1))){
                        ++check[k];
                        if(check[k] > 1){
                            continue;
                        }
                        rj[k] = j;  //record every j first time appear k+1
                    }
                }
            }
        }
        for(k = 0; k < 9; ++k){
            if(check[k] == 1){
                ans.map[i + 9*rj[k]] = k+1;
                return true;
            }
        }
    }

    int l;
    //the north west cell in big cell
    int NWcell[9] = {0, 3, 6, 27, 30, 33, 54, 57, 60};
    //check cell in big cell which can only be #
    //for(k = 1; k <= 9; ++k){ //# 1~9
    for(i = 0; i < 9; ++i){ //big cell
        int check[9] = {0};
        int rjl[9] = {0};
        for(j = 0; j < 3; ++j){ //small row
            for(l = 0; l < 3; ++l){ //small col
                if(map[NWcell[i] + 9*j + l]==0){
                    str = p.getPencil(NWcell[i]/9 + j, NWcell[i]%9 + l);
                    for(k = 0; k < 9; ++k){
                        if(str.contains(QString::number(k+1))){
                            ++check[k];
                            if(check[k] > 1){
                                continue;
                            }
                            rjl[k] = 9*j + l;
                        }
                    }
                }
            }
        }
        for(k = 0; k < 9; ++k){
            if(check[k] == 1){
                ans.map[NWcell[i] + rjl[k]] = k+1;
                return true;
            }

        }
    }


    return false;
}

bool Sudoku::superSolve(Sudoku & ans){
    Pencil guess(map);
    QString str;
    int guess_num = 0; //guess which pencil
    int i, j = 0;
    int guess_map[81] = {0};
    int guess_pos = 0; //position of the guessed cell
    static Sudoku guess_su;
    //int count_num = 0; //number of the elements in map

    //copy question to guess_map
    for(i = 0; i < 81; ++i){
        guess_map[i] = map[i];
        //if(map[i] > 0) ++count_num;
    }

    //if(count_num < 17)
    //    return false;
    //get first zero-cell pencil
    for(i = 0; i < 81; ++i){
        if(map[i] == 0){
            str = guess.getPencil(i/9, i%9);
            guess_pos = i;
            break;
        }
    }



next_pencil:
    if(guess_num >= str.size()){
        return false;
    }
    guess_map[guess_pos] = str.mid(guess_num,1).toInt();
    guess_su.setMap(guess_map);

    j = 0;
    while(guess_su.solve(guess_su)){++j;}

    if(guess_su.isCorrect()){
        /*for(i = 0; i < 81; ++i){
            guess_map[i] = guess_su.getElement(i%9, i/9);
        }*/
        //ans.setMap(guess_map);
        ans = guess_su;
        return true;
    } else {
        guess.setMap(guess_map);
        for(i = 0; i < 81; ++i){
            if(guess_map[i] == 0 && guess.getPencil(i/9, i%9) == NULL){
                ++guess_num;
                goto next_pencil;

            }
        }
        if(guess_su.superSolve(guess_su) == false){
            ++guess_num;
            goto next_pencil;
        }
    }


    ans = guess_su;
    return true;
}

int Sudoku::findMaxIndex(int arr[], int index){
    int max = 0, maxIndex = 0;
    for(int i = 0; i < index; ++i){
        if(arr[i] > max){
            maxIndex = i;
            max = arr[i];
        }
    }
    return maxIndex;
}
