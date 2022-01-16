#ifndef CUBE_H
#define CUBE_H

#include "vertex.h"
#include <QList>

class Cube
{
public:
    Cube();
    virtual ~Cube();

    QList<Vertex> vertices() const;

private:
    QList<Vertex> m_vertices;
};

#endif // CUBE_H
