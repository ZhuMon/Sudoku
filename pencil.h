#ifndef PENCIL_H
#define PENCIL_H

#include <QString>

class Pencil{
public:
    Pencil();
    Pencil(int set_map[]);
    //Pencil(int map[], int row, int col);
    void setMap(int set_map[]);
    QString getPencil(int row, int col); //0~8
private:
    bool n[9];  //whether the cell be i
    int map[81];

};
#endif // PENCIL_H
