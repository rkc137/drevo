#include "point.h"

point::point(int x, int y, QString name)
{
    int size_of_point = 30;
    setRect(0, 0, size_of_point, size_of_point);
    setPos(x, y);
    G = new QGraphicsTextItem(name, this);
    G->setPos(8.5, 5);
    P = new QGraphicsTextItem("", this);
    P->setPos(8.5, -5);
}
