#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include <QColor>
#include <QMessageBox>
#include <cstdlib>
#include <sudoku.h>
#include <random.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this-> setStyleSheet("background-image:url(:/images/pen.jpg); background-position: center;" );

    connect(ui -> tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(progressBar_change()));
    //connect(ui -> tableWidget, SIGNAL(cellEntered(int,int)), this, SLOT(progressBar_change()));
    //connect(ui ->file_in, SIGNAL(clicked(bool)), this, SLOT());

    ui -> tableWidget -> hide();
    ui -> random -> hide();
    ui -> zero -> hide();
    ui -> record_map -> hide();
    ui -> file_in -> hide();
    ui -> solve -> hide();
    ui -> check -> hide();
    ui -> progressBar -> hide();
    ui -> file_list -> hide();

    ui -> easy -> hide();
    ui -> normal -> hide();
    ui -> hard -> hide();
    ui -> hell -> hide();

    ui -> background -> hide();
    ui -> colorA -> hide();
    ui -> colorB -> hide();
    ui -> colorC -> hide();

    ui -> selC -> hide();
    ui -> sColorA -> hide();
    ui -> sColorB -> hide();
    ui -> sColorC -> hide();

    ui -> checkTable -> hide();

    ui -> colorA -> setStyleSheet("background-color: rgb(223, 236, 235)");
    ui -> colorB -> setStyleSheet("background-color: rgb(246, 204, 255)");
    ui -> colorC -> setStyleSheet("background-color: rgb(218, 233, 216)");

    ui -> sColorA -> setStyleSheet("background-color: rgb(255, 195, 0)");
    ui -> sColorB -> setStyleSheet("background-color: rgb(255, 242, 0)");
    ui -> sColorC -> setStyleSheet("background-color: rgb(255, 94, 0)");

    cBa.setRgb(223, 236, 235);
    cBb.setRgb(246, 204, 255);
    cBc.setRgb(218, 233, 216);

    cSa.setRgb(255, 195, 0);
    cSb.setRgb(255, 242, 0);
    cSc.setRgb(255, 94, 0);

    QcB = cBa;
    QcS = cSa;

    QString s = QString::number(0);
    QTableWidgetItem *ex_cell, *cell[81], *ex_cell_odd;

    ex_cell = new QTableWidgetItem(s, 0);
    ex_cell_odd = new QTableWidgetItem(s, 0);
    ex_cell -> setTextAlignment(Qt::AlignHCenter);
    ex_cell -> setTextAlignment(Qt::AlignCenter);
    ex_cell_odd = ex_cell -> clone();
    ex_cell_odd -> setBackgroundColor(QColor(255, 255, 255));
    ex_cell -> setBackgroundColor(QcB);
    // QTableWidget::setItemPrototype(ex_cell);

    difficulty = 40;

    QTableWidgetItem *checkCell[9];
    //initial checkTable
    for(int i = 0; i < 9; ++i){
        checkCell[i] = ex_cell_odd -> clone();
        checkCell[i] -> setText(QString::number(i+1));
        checkCell[i] -> setTextColor(QColor(0,0,0));
        ui -> checkTable -> setItem(i/3, i%3, checkCell[i]);
        checkTable[i] = 0;
    }

    for(int i = 0; i < 9; ++i){ //row
        for(int j = 0; j < 9; ++j){ //col
            cell[9*i + j] = new QTableWidgetItem(s, 0);
            if(((i < 3 || i > 5) && j < 3)
                    || (i > 2 && i < 6 && j > 2 && j < 6)
                    || (((i < 3 || i > 5) && j > 5))){

                cell[9*i + j] = ex_cell_odd -> clone();

            }else{
                cell[9*i + j] = ex_cell->clone();
            }
            ui -> tableWidget -> setItem(i, j, cell[9*i +j]);

        }

    }

    stdAction = new QAction(QIcon(":/images/standard"), tr("&Standard..."), this);
    ownselfAction = new QAction(QIcon(":/images/custom"), tr("&Custom..."), this);
    settingAction = new QAction(QIcon(":/images/setting"), tr("&Setting..."), this);
    //StdnAction->setShortcuts(QKeySequence::Open);
    stdAction->setStatusTip(tr("To solve random question"));
    ownselfAction -> setStatusTip(tr("Set up your own Sudoku question"));
    settingAction -> setStatusTip(tr("Set property about interface"));
    connect(stdAction, &QAction::triggered, this, &MainWindow::stdMode);
    connect(ownselfAction, &QAction::triggered, this, &MainWindow::ownselfMode);
    connect(settingAction, &QAction::triggered, this, &MainWindow::settingMode);

    //QMenu *file = menuBar()->addMenu(tr("&File"));
    //file->addAction(stdAction);

    //QToolBar *toolBar = addToolBar(tr("&Standard"));
    ui -> toolBar -> addAction(stdAction);
    ui -> toolBar -> addAction(ownselfAction);
    ui -> toolBar -> addAction(settingAction);
    statusBar() ;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_random_clicked()
{
    QString map_str;
    //QTableWidgetItem *cell[81];
    QFont qf;
    Random rd(difficulty);

    for(int i = 0;i < 81; ++i){
        map[i] = rd.getElement(i);

        map_str = QString::number(map[i]);
        //cell[i] = new QTableWidgetItem(map_str, 0);
        ui->tableWidget -> item(i/9, i%9)->setText(map_str);

        if(map[i] > 0){
            qf.setWeight(75);
            ui -> tableWidget -> item(i/9, i%9) -> setFont(qf);
            //can't allow to edit
            ui -> tableWidget -> item(i/9, i%9) -> setFlags(ui -> tableWidget ->item(i/9, i%9)->flags() &= ~Qt::ItemIsEditable);

        } else if(map[i] == 0){
            qf.setWeight(50);
            ui -> tableWidget -> item(i/9, i%9) -> setFont(qf);
            ui -> tableWidget ->item(i/9, i%9)->setFlags(ui -> tableWidget ->item(i/9, i%9)->flags() | Qt::ItemIsEditable);
        }
    }

    QFile file("random.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            return;
    }

    QTextStream out(&file);
    for(int i = 0; i < 81; ++i){
        out << map[i];
        if(i % 9 == 8){
            out << "\n";
        }
    }


    ui -> file_in -> show();

    file.close();
}

void MainWindow::on_record_map_clicked()
{
    static int file_num;
    QString file_name = "data" + QString::number(file_num) + ".txt";
    QFile file(file_name);
    ++file_num;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            return;
    }

    QTextStream out(&file);

    for(int i = 0; i < 81; ++i){
        out << map[i];
        if(i % 9 == 8){
            out << "\n";
        }
    }

    ui -> file_list -> insertItem(file_num-1, new QListWidgetItem(file_name));

    file.close();
}

//make all cell to 0
void MainWindow::on_zero_clicked()
{
    QString s = QString::number(0);
    //QTableWidgetItem *cell[81];
    QFont qf;
    qf.setWeight(50);

    for(int i = 0; i < 9; ++i){ //row
        for(int j = 0; j < 9; ++j){ //col
            //cell[9*i + j] = new QTableWidgetItem(s, 0);
            ui -> tableWidget -> item(i, j) -> setText(s);
            ui -> tableWidget -> item(i, j) -> setFont(qf);
            ui -> tableWidget -> item(i, j) -> setFlags(ui -> tableWidget ->item(i, j)->flags() | Qt::ItemIsEditable);

        }
    }
}

void MainWindow::on_file_in_clicked()
{

    QFile file("random.txt");   //read data from
    file.open(QIODevice::ReadOnly);

    QString line[9], s = NULL;
    int i = 0, j;

    while(!file.atEnd() || i < 9){
        line[i] = file.readLine();
        line[i].replace(" ", "");
        ++i;
    }

    for(i = 0; i < 9; ++i){ //row
        for(j = 0; j < 9; ++j){ //col
            map[9*i+j] = line[i].mid(j, 1).toInt();
            s = QString::number(map[9*i+j]);
            ui -> tableWidget -> item(i, j) ->setText(s);
            QFont qf;
            if(map[9*i+j] > 0){
                qf.setWeight(75);
                ui -> tableWidget -> item(i, j) -> setFont(qf);
                ui -> tableWidget -> item(i, j) -> setFlags(ui -> tableWidget ->item(i, j)->flags() &= ~Qt::ItemIsEditable);

            } else{
                qf.setWeight(50);
                ui -> tableWidget -> item(i, j) -> setFont(qf);
                ui -> tableWidget -> item(i, j) -> setFlags(ui -> tableWidget ->item(i, j)->flags() | Qt::ItemIsEditable);

            }
        }
    }

    file.close();
}

void MainWindow::progressBar_change(){
    int i;
    float percent = 0;

    for(i = 0; i < 81; ++i){
        //map[i] = ui -> tableWidget ->item(i/9, i%9) -> text().toInt();
        if(map[i] > 0){
            percent = percent + 100.0/81.0;
        }
    }

    ui -> progressBar -> setValue((int)percent);

}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    //make every input to map
    map[row*9 + column] = ui -> tableWidget
           -> item(row, column) -> text().toInt();

    QString s, s0 = NULL, s1 = NULL;
    s = ui ->tableWidget->item(row, column) -> text();
    if(ui -> tableWidget->item(4, 0))
        s0 = ui ->tableWidget->item(4, 0) -> text();
    if(ui -> tableWidget->item(4, 2))
        s1 = ui ->tableWidget->item(4, 2) -> text();
    QFont f;
    f.setBold(true);
    //click 1, all 1 will turn yellow
    if(s0 == "C"&&
            (s == "C" || s == "O" || s == "M"
            ||s == "P" || s == "L" || s == "E"
            ||s == "T" || s == "D")){
        ui -> tableWidget -> item(row, column) -> setTextColor(QColor(174, 63, 255));
        ui -> tableWidget -> item(row, column) -> setFont(f);
    } else if(s1 == "F" &&(s == "F" || s == "A" || s == "L"
              ||s == "S" || s == "E" )){
        ui -> tableWidget -> item(row, column) -> setTextColor(QColor(176, 119, 30));
        ui -> tableWidget -> item(row, column) -> setFont(f);
    } else if(s.toInt() == 0){
        ui -> tableWidget -> item(row, column) -> setTextColor(ui -> tableWidget -> item(row, column) -> backgroundColor());
    } else {
        ui -> tableWidget -> item(row, column) -> setTextColor(QColor(0,0,0));
    }

    if(ui -> solve ->isVisible()){
        for(int i = 0; i < 9; ++i){
            checkTable[i] = 0;
        }
        for(int i = 0; i < 81; ++i){
            if(map[i]>0){
                ++checkTable[map[i]-1];
            }
        }

        for(int i = 0; i < 9; ++i){
            if(checkTable[i] >= 9){
                ui -> checkTable -> item(i/3, i%3) -> setBackgroundColor(QcS);
            } else {
                ui -> checkTable -> item(i/3, i%3) -> setBackgroundColor(QColor(255,255,255));
            }
        }
    }
    /*QString t;
    //s = ui -> tableWidget -> item(row, column) -> text();

    //set background
    for(int i = 0; i < 9; ++i){ //row
        for(int j = 0; j < 9; ++j){ //col
            if(((i < 3 || i > 5) && j < 3)
                    || (i > 2 && i < 6 && j > 2 && j < 6)
                    || (((i < 3 || i > 5) && j > 5))){

                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QColor(255, 255, 255));

            }else{
                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QColor(216, 233, 232));
            }

            t = ui -> tableWidget -> item(i, j) -> text();
            if(t.toInt() > 0 && t == s){
                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QColor(255, 205, 41));
            }
        }

    }*/

}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{

    QString s, t;
    s = ui -> tableWidget -> item(row, column) -> text();

    for(int i = 0; i < 9; ++i){ //row
        for(int j = 0; j < 9; ++j){ //col
            if(((i < 3 || i > 5) && j < 3)
                    || (i > 2 && i < 6 && j > 2 && j < 6)
                    || (((i < 3 || i > 5) && j > 5))){

                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QColor(255, 255, 255));

            }else{
                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QcB);
            }

            t = ui -> tableWidget -> item(i, j) -> text();
            if(t.toInt() > 0 && t == s){
                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QcS);
            }
        }

    }
}

void MainWindow::on_solve_clicked()
{
    for(int i = 0; i < 9; ++i){ //row
        for(int j = 0; j < 9; ++j){ //col
            map[i*9+j] =
                    ui -> tableWidget -> item(i, j) -> text().toInt();
        }
    }

    Sudoku ans(map);
    while(ans.solve(ans));
    if(ans.isCorrect()){
        QString s;

        for(int i = 0; i < 9; ++i){ //row
            for(int j = 0; j < 9; ++j){ //col
                s = QString::number(ans.getElement(j, i));
                ui -> tableWidget -> item(i, j) ->setText(s);
            }
        }
    } else {
        if(ans.superSolve(ans)){
            QString s;

            for(int i = 0; i < 9; ++i){ //row
                for(int j = 0; j < 9; ++j){ //col
                    s = QString::number(ans.getElement(j, i));
                    ui -> tableWidget -> item(i, j) ->setText(s);
                }
            }
        }
    }
}

void MainWindow::on_file_list_itemClicked(QListWidgetItem *item)
{
    QString file_name = item -> text();
    QFile file(file_name);
    file.open(QIODevice::ReadOnly);

    QString line[9], s = NULL;
    int i = 0, j;

    while(!file.atEnd() || i < 9){
        line[i] = file.readLine();
        line[i].replace(" ", "");
        ++i;
    }

    for(i = 0; i < 9; ++i){ //row
        for(j = 0; j < 9; ++j){ //col
            map[9*i+j] = line[i].mid(j, 1).toInt();
            s = QString::number(map[9*i+j]);
            ui -> tableWidget -> item(i, j) ->setText(s);
            QFont qf;
            if(map[9*i+j] > 0){
                qf.setWeight(75);
                ui -> tableWidget -> item(i, j) -> setFont(qf);
                ui -> tableWidget -> item(i, j) -> setFlags(ui -> tableWidget ->item(i, j)->flags() &= ~Qt::ItemIsEditable);
            } else{
                qf.setWeight(50);
                ui -> tableWidget -> item(i, j) -> setFont(qf);
                ui -> tableWidget -> item(i, j) -> setFlags(ui -> tableWidget ->item(i, j)->flags() | Qt::ItemIsEditable);
            }
        }
    }

    file.close();
}

void MainWindow::on_check_clicked()
{
    Sudoku su(map);
    if(su.isCorrect()){
        for(int i = 0; i < 9; ++i){ //col
            for(int j = 0; j < 9; ++j){ //row
                ui -> tableWidget -> item(j, i) -> setBackgroundColor(QColor(255,255,255));
                ui -> tableWidget -> item(j, i) -> setText("");
                if(((i == 0 || (i == 8) )&& j < 4)
                        || (i == 4 && j == 3)
                        || (j - i == 5 && i < 3)
                        || (j - i == 6)
                        || (i == 3 &&(j == 6 || j == 7))
                        || (i > 3 && i < 7 && j - i == 1)
                        || (i > 3 && j - i == 2)
                        || (i == 7 &&(j == 6 || j == 7))
                        || (i == 8 &&(j == 5 || j == 6))){
                    ui -> tableWidget -> item(j, i) -> setBackgroundColor(QColor(255,126,0));
                }

            }
        }
        ui -> tableWidget -> item(4, 0) -> setText("C");
        ui -> tableWidget -> item(4, 1) -> setText("O");
        ui -> tableWidget -> item(4, 2) -> setText("M");
        ui -> tableWidget -> item(4, 3) -> setText("P");
        ui -> tableWidget -> item(4, 4) -> setText("L");
        ui -> tableWidget -> item(4, 5) -> setText("E");
        ui -> tableWidget -> item(4, 6) -> setText("T");
        ui -> tableWidget -> item(4, 7) -> setText("E");
        ui -> tableWidget -> item(4, 8) -> setText("D");
    } else {
        for(int i = 0; i < 9; ++i){ //col
            for(int j = 0; j < 9; ++j){ //row
                ui -> tableWidget -> item(j, i) -> setBackgroundColor(QColor(255,255,255));
                ui -> tableWidget -> item(j, i) -> setText("");
                if((i<3 && j == 0) || (i == 1) || (j == 8) ||(i == 8 && j >5)){
                    ui -> tableWidget -> item(j, i) -> setBackgroundColor(QColor(255,126,0));
                }

            }
        }

        ui -> tableWidget -> item(4, 2) -> setText("F");
        ui -> tableWidget -> item(4, 3) -> setText("A");
        ui -> tableWidget -> item(4, 4) -> setText("L");
        ui -> tableWidget -> item(4, 5) -> setText("S");
        ui -> tableWidget -> item(4, 6) -> setText("E");

    }
}

void MainWindow::stdMode(){
    ui -> tableWidget -> show();
    ui -> random -> hide();
    if(ui -> easy->isChecked() | ui -> normal -> isChecked()
            | ui -> hard -> isChecked() | ui -> hell -> isChecked()){
        ui -> random -> show();
    }
    ui -> zero -> hide();
    ui -> record_map -> hide();

    ui -> file_in -> hide();
    ui -> solve -> show();
    ui -> check -> show();
    ui -> progressBar -> show();
    ui -> file_list -> hide();
    ui -> easy -> show();
    ui -> normal -> show();
    ui -> hard -> show();
    ui -> hell -> show();

    ui -> background -> hide();
    ui -> colorA -> hide();
    ui -> colorB -> hide();
    ui -> colorC -> hide();

    ui -> selC -> hide();
    ui -> sColorA -> hide();
    ui -> sColorB -> hide();
    ui -> sColorC -> hide();

    ui -> checkTable -> show();
}

void MainWindow::ownselfMode(){
    ui -> tableWidget -> show();
    ui -> random -> hide();
    ui -> zero -> show();
    ui -> record_map -> show();
    ui -> file_in -> hide();
    ui -> solve -> show();
    ui -> check -> show();
    ui -> progressBar -> show();
    ui -> file_list -> show();
    ui -> easy -> hide();
    ui -> normal -> hide();
    ui -> hard -> hide();
    ui -> hell -> hide();

    ui -> background -> hide();
    ui -> colorA -> hide();
    ui -> colorB -> hide();
    ui -> colorC -> hide();

    ui -> selC -> hide();
    ui -> sColorA -> hide();
    ui -> sColorB -> hide();
    ui -> sColorC -> hide();

    ui -> checkTable -> show();
}

void MainWindow::settingMode(){
    ui -> tableWidget -> show();
    ui -> random -> hide();
    ui -> zero -> hide();
    ui -> record_map -> hide();
    ui -> file_in -> hide();
    ui -> solve -> hide();
    ui -> check -> hide();
    ui -> progressBar -> hide();
    ui -> file_list -> hide();
    ui -> easy -> hide();
    ui -> normal -> hide();
    ui -> hard -> hide();
    ui -> hell -> hide();


    ui -> colorA -> show();
    ui -> colorB -> show();
    ui -> colorC -> show();
    ui -> background -> show();

    ui -> selC -> show();
    ui -> sColorA -> show();
    ui -> sColorB -> show();
    ui -> sColorC -> show();

    ui -> checkTable -> hide();
}

void MainWindow::on_easy_clicked()
{
    difficulty = 60;
}

void MainWindow::on_normal_clicked()
{
    difficulty = 40;
}

void MainWindow::on_hard_clicked()
{
    difficulty = 30;
}

void MainWindow::on_hell_clicked()
{
    difficulty = 20;
}

void MainWindow::on_colorA_clicked()
{
    QcB = cBa;
    for(int i = 0; i < 9; ++i){ //row
        for(int j = 0; j < 9; ++j){ //col
            if(((i < 3 || i > 5) && j < 3)
                    || (i > 2 && i < 6 && j > 2 && j < 6)
                    || (((i < 3 || i > 5) && j > 5))){

                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QColor(255, 255, 255));

            }else{
                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QcB);
            }
        }
    }
}

void MainWindow::on_colorB_clicked()
{
    QcB = cBb;
    for(int i = 0; i < 9; ++i){ //row
        for(int j = 0; j < 9; ++j){ //col
            if(((i < 3 || i > 5) && j < 3)
                    || (i > 2 && i < 6 && j > 2 && j < 6)
                    || (((i < 3 || i > 5) && j > 5))){

                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QColor(255, 255, 255));

            }else{
                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QcB);
            }
        }
    }
}

void MainWindow::on_colorC_clicked()
{
    QcB = cBc;
    for(int i = 0; i < 9; ++i){ //row
        for(int j = 0; j < 9; ++j){ //col
            if(((i < 3 || i > 5) && j < 3)
                    || (i > 2 && i < 6 && j > 2 && j < 6)
                    || (((i < 3 || i > 5) && j > 5))){

                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QColor(255, 255, 255));

            }else{
                ui -> tableWidget -> item(i, j) -> setBackgroundColor(QcB);
            }
        }
    }
}

void MainWindow::on_sColorA_clicked()
{
    QcS = cSa;
}

void MainWindow::on_sColorB_clicked()
{
    QcS = cSb;
}

void MainWindow::on_sColorC_clicked()
{
    QcS = cSc;
}
