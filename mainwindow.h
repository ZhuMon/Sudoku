#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QObject>
#include <QTableWidgetItem>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_random_clicked();

    void on_record_map_clicked();

    void on_zero_clicked();

    void on_file_in_clicked();

    void progressBar_change();

    void on_tableWidget_cellChanged(int row, int column);

    void on_tableWidget_cellClicked(int row, int column);

    void on_solve_clicked();

    void on_file_list_itemClicked(QListWidgetItem *item);

    void on_check_clicked();

    void on_easy_clicked();

    void on_normal_clicked();

    void on_hard_clicked();

    void on_hell_clicked();

    void on_colorA_clicked();

    void on_colorB_clicked();

    void on_colorC_clicked();

    void on_sColorA_clicked();

    void on_sColorB_clicked();

    void on_sColorC_clicked();

private:
    Ui::MainWindow *ui;
    int map[81];

    QColor QcB;//table background
    QColor QcS;//select

    QColor cBa, cBb, cBc;
    QColor cSa, cSb, cSc;

    QAction *stdAction;
    QAction *ownselfAction;
    QAction *settingAction;
    void stdMode();
    void ownselfMode();
    void settingMode();

    int difficulty;

    int checkTable[9];


};

#endif // MAINWINDOW_H
