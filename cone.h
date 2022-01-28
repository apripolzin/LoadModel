#ifndef CONE_H
#define CONE_H

#include "abstractmesh.h"

class Cone : public AbstractMesh
{
public:
    Cone();
    virtual ~Cone();

    QList<Vertex> vertices() const override;
    virtual int drawCount() const override;
private:
    QList<Vertex> m_vertices;
};

#endif // CONE_H
