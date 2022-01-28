#ifndef MESH_H
#define MESH_H

#include "abstractmesh.h"

class Mesh : public AbstractMesh
{
public:
    Mesh(const QString &objfilename);

    QList<Vertex> vertices() const override;
    virtual int drawCount() const override;

private:
    QList<Vertex> m_vertices;
    int m_drawCount = 0;

};

#endif // MESH_H
