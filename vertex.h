#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector2D texture_coord;
};

#endif // VERTEX_H
