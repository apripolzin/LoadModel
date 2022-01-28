#ifndef CUBE_H
#define CUBE_H

#include "abstractmesh.h"

class Cube : AbstractMesh
{
public:
    Cube();
    virtual ~Cube();

    QList<Vertex> vertices() const override;
    int drawCount() const override;

private:
    QList<Vertex> m_vertices;
};

#endif // CUBE_H
