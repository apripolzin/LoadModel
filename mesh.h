#ifndef MESH_H
#define MESH_H

#include <QString>
#include <QVector3D>
#include <QList>

#include "vertex.h"

class Mesh
{
public:
    Mesh(const QString &objfilename);

    QList<Vertex> vertices() const;

private:
    QList<Vertex> m_vertices;

};

#endif // MESH_H
