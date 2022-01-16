#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QDebug>

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texture_coord;
};

QDebug operator<<(QDebug debug, const Vertex &v);

#endif // VERTEX_H
