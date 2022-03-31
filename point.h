#ifndef POINT_H
#define POINT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>

class point : public QGraphicsEllipseItem
{
public:
    explicit point(int x, int y, QString name);
    QGraphicsTextItem *G, *P;
    int price;
};

#endif // POINT_H
