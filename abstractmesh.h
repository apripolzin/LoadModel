#ifndef ABSTRACTMESH_H
#define ABSTRACTMESH_H

#include <QList>
#include "vertex.h"

class AbstractMesh
{
public:
    AbstractMesh();

    virtual QList<Vertex> vertices() const = 0;
    virtual int drawCount() const = 0;
};

#endif // ABSTRACTMESH_H
