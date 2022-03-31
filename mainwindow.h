#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <stdlib.h>
#include <QLCDNumber>
#include <QMessageBox>
#include <QVector>
#include <QComboBox>
#include <QtMath>
//#include <windows.h>

#include "point.h"

struct connection
{
    bool have;
    int in, out;
    QGraphicsLineItem *line;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QGraphicsScene *scena;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void mousePressEvent(QMouseEvent* event);
    const static int size_of_point = 30,S = 15, half_size_of_point = size_of_point / 2, correct = ((size_of_point / 2) - 10) * 5;
    bool way_act = 1;

    point* ptr_point;
    QVector<point*> points;
    QVector<connection> ways;


private:
    Ui::MainWindow *ui;

public slots:
    point* find_point(int X, int Y);
    bool no_point(int X, int Y);
    int shet(int start, int end);
    int find_index(point* ptr_point);
    void draw_way(connection con);
    void col_way();
    void col_way_all();
    void clear();
};

#endif // MAINWINDOW_H
