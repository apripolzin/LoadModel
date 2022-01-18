#ifndef CONE_H
#define CONE_H

#include "vertex.h"
#include <QList>

class Cone
{
public:
    Cone();
    virtual ~Cone();

    QList<Vertex> vertices() const;

private:
    QList<Vertex> m_vertices;
};

#endif // CONE_H
