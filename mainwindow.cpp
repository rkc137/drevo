#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("drevo");
    //this->setWindowIcon(QIcon("icon.png"));
    scena = new QGraphicsScene;
    scena->setSceneRect(1, 1, 770, 430);
    ui->graphicsView->setScene(scena);
    connect(ui->clr_butt, SIGNAL(clicked(bool)), this, SLOT(clear()));
    connect(ui->kol_way_butt, SIGNAL(clicked(bool)), this, SLOT(col_way()));
    connect(ui->kol_way_all, SIGNAL(clicked(bool)), this, SLOT(col_way_all()));
}

void MainWindow::clear()
{
    ptr_point = 0;
    way_act = 1;
    scena->clear();
    ui->label->setText("");
    ui->lcdNumber->display("");
    ui->comboBox->setCurrentIndex(0);
    ways.clear();
    points.clear();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    ui->label->clear();
    QMainWindow::mousePressEvent(event);
    int e_x = event->x(), e_y = event->y();
    if(((10 < e_x) && (e_x < 781)) && ((10 < e_y) && (e_y < 441)))
    {
        if(ui->point_butt->isChecked())
        {
            bool us = ((e_x > (10 + half_size_of_point)) && (e_x < (781 - half_size_of_point))) &&
                      ((e_y > (10 + half_size_of_point)) && (e_y < (441 - half_size_of_point)));
            if(us && no_point(e_x - size_of_point, e_y - size_of_point))
            {
                way_act = 1;
                if(e_x)
                    points.insert(points.count(),
                                  new point(e_x - correct,
                                            e_y - correct,
                                            ui->comboBox->currentText()));
                scena->addItem(points[points.count() - 1]);
                ui->comboBox_last->setCurrentIndex(ui->comboBox->currentIndex());
                ui->comboBox->setCurrentIndex(int(ui->comboBox->currentIndex()) + 1);
            }
            else
            {
                ui->label->setText("нет места!");
            }
        }
        else if(ui->way_butt->isChecked())
        {
            point *local = find_point(e_x - half_size_of_point, e_y - half_size_of_point);
            if(way_act)
            {
                if(local != 0)
                {
                    ptr_point = local;
                    local->setPen(QPen(QColor(255, 0, 0)));
                }
                else{return;}
            }
            else
            {
                if(local != 0)
                {
                    connection con;
                    con.in = find_index(local);
                    con.out = find_index(ptr_point);
                    ways.insert(ways.count(), con);

                    int count = ways.count() - 1;
                    ways[count].in = find_index(local);
                    draw_way(con);
                }
                else{return;}
            }
            way_act = !way_act;
        }
    }
}

int MainWindow::find_index(point* ptr_point)
{
    int count = points.count();
    for(int i = 0; i < count; i++)
    {
        if(points[i] == ptr_point)
        {
            return i;
        }
    }
    return -1;
}

bool MainWindow::no_point(int X, int Y)
{
    for(int i = 0; i < points.count(); i++)
    {
        if(size_of_point + half_size_of_point >= qSqrt(((X - points[i]->x()) * (X - points[i]->x())) + ((Y - points[i]->y()) * (Y - points[i]->y()))))
        {
            return 0;
        }
    }
    return 1;
}

point* MainWindow::find_point(int X, int Y)
{
    for(int i = 0; i < points.count(); i++)
    {
        if(size_of_point >= qSqrt(((X - points[i]->x()) * (X - points[i]->x())) + ((Y - points[i]->y()) * (Y - points[i]->y()))))
        {
            return points[i];
        }
    }
    return nullptr;
}

void MainWindow::draw_way(connection con)
{
    int fx, fy;
    int dx = points[con.out]->x() - points[con.in]->x();
    int dy = points[con.out]->y() - points[con.in]->y();
    float corrX = half_size_of_point * (dx / sqrt((dx * dx) + (dy * dy)));
    float corrY = -half_size_of_point * (dy / sqrt((dx * dx) + (dy * dy)));
    QGraphicsLineItem *ptr_line = new QGraphicsLineItem(points[con.out]->x() + half_size_of_point - corrX,
                                                        points[con.out]->y() + half_size_of_point + corrY,
                                                        points[con.in]->x() + half_size_of_point + corrX,
                                                        points[con.in]->y() + half_size_of_point - corrY);

    int x1 = points[con.out]->x() + half_size_of_point - corrX;
    int x2 = points[con.in]->x() + half_size_of_point + corrX;
    int y1 = points[con.out]->y() + half_size_of_point + corrY;
    int y2 = points[con.in]->y() + half_size_of_point - corrY;

    float ugl = asin((y2 - y1) / (sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)))));
    if(x2 < x1)
    {
        ugl = 3.1415 - ugl;
    }

    fx = x2 + (S * sin(ugl - 1.15));
    fy = y2 - (S * cos(ugl - 1.15));
    scena->addLine(x2, y2, fx, fy);
    fx = x2 - (S * sin(ugl + 1.15));
    fy = y2 + (S * cos(ugl + 1.15));
    scena->addLine(x2, y2, fx, fy);

    scena->addItem(ptr_line);
    ptr_point->setPen(QPen(QColor(0, 0, 0)));
}

int MainWindow::shet(int start, int end)
{
    int endP = points.count(), endW = ways.count();
    points[start]->price = 1;
    while(points[end]->price == -1)
    {
        for(int i = 0; i < endP; i++)
        {
            if(points[i]->price != -1)
            {
                continue;
            }
            points[i]->price = 0;
            for(int j = 0; j < endW; j++)
            {
                if((i == ways[j].in) && (ways[j].have == 1))
                {
                    if(points[ways[j].out]->price != -1)
                    {
                        points[i]->price += points[ways[j].out]->price;
                    }
                    else
                    {
                        points[i]->price = -1;
                        break;
                    }
                }
            }
        }
    }
    return points[end]->price;
}

void MainWindow::col_way()
{
    int endP = points.count(), endW = ways.count();
    int col = 0;
    int first = ui->comboBox_one->currentIndex(), last = ui->comboBox_last->currentIndex(), midl = ui->comboBox_have->currentIndex();


    for(int i = 0; i < endP; i++)
    {points[i]->price = -1;}

    if(ui->radioButton_isnt_have->isChecked())
    {
        for(int i = 0; i < endW; i++)
        {
            if(ways[i].in == ui->comboBox_have->currentIndex())
            {
                ways[i].have = 0;
            }
            else
            {
                ways[i].have = 1;
            }
        }
        col = shet(first, last);
    }
    else if(ui->radioButton_is_have->isChecked())
    {
        for(int i = 0; i < endW; i++){ways[i].have = 1;}
        int col1 = shet(first, midl);

        for(int i = 0; i < endP; i++)
        {
            points[i]->price = -1;
        }
        for(int i = 0; i < endW; i++)
        {
            ways[i].have = 1;
        }
        col = col1 * shet(midl, last);
    }
    else//  ui.radioButton_standart->isChecked()
    {
        for(int i = 0; i < endW; i++)
        {
            ways[i].have = 1;
        }
        col = shet(first, last);
    }
    ui->lcdNumber->display(col);
}

void MainWindow::col_way_all()
{
    int endP = points.count();
    for(int i = 0; i < endP; i++)
    {
        points[i]->P->setPlainText(QString::number(points[i]->price));
        points[i]->P->update();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
