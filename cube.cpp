#include "cube.h"

struct Face {
    const int vertex_index;
    const int vertex_texture_index;
    const int vertex_normal_index;
};

static const QList<QVector3D> _vertices = {
    {0, 0, 0}, //dummy
    {1.000000, 1.000000, -1.000000},
    {1.000000, -1.000000, -1.000000},
    {1.000000, 1.000000, 1.000000},
    {1.000000, -1.000000 ,1.000000},
    {-1.000000, 1.000000, -1.000000},
    {-1.000000, -1.000000, -1.000000},
    {-1.000000, 1.000000, 1.000000},
    {-1.000000, -1.000000, 1.000000},
};

static const QList<QVector2D> _vertex_textures = {
    {0, 0,}, //dummy
    {0.875000, 0.500000},
    {0.625000, 0.750000},
    {0.625000, 0.500000},
    {0.375000, 1.000000},
    {0.375000, 0.750000},
    {0.625000, 0.000000},
    {0.375000, 0.250000},
    {0.375000, 0.000000},
    {0.375000, 0.500000},
    {0.125000, 0.750000},
    {0.125000, 0.500000},
    {0.625000, 0.250000},
    {0.875000, 0.750000},
    {0.625000, 1.000000},
};

static const QList<QVector3D> _vertex_normals = {
    {0, 0, 0}, //dummy
    {0.0000 ,1.0000 ,0.0000},
    {0.0000 ,0.0000 ,1.0000},
    {-1.0000, 0.0000, 0.0000},
    {0.0000 ,-1.0000, 0.0000},
    {1.0000 ,0.0000 ,0.0000},
    {0.0000 ,0.0000 ,-1.0000},
};

static const QList<Face> _faces = {
    {5,1,1},   {3,2,1},  {1,3,1},
    {3,2,2},   {8,4,2},  {4,5,2},
    {7,6,3},   {6,7,3},  {8,8,3},
    {2,9,4},   {8,10,4}, {6,11,4},
    {1,3,5},   {4,5,5},  {2,9,5},
    {5,12,6},  {2,9,6},  {6,7,6},
    {5,1,1},   {7,13,1}, {3,2,1},
    {3,2,2},   {7,14,2}, {8,4,2},
    {7,6,3},   {5,12,3}, {6,7,3},
    {2,9,4},   {4,5,4},  {8,10,4},
    {1,3,5},   {3,2,5},  {4,5,5},
    {5,12,6},  {1,3,6},  {2,9,6},
};


Cube::Cube()
{
    for (const Face &f : _faces) {
        Vertex v = {_vertices[f.vertex_index], _vertex_normals[f.vertex_normal_index], _vertex_textures[f.vertex_texture_index]};
        m_vertices.push_back(v);
    }
}

Cube::~Cube()
{
}

QList<Vertex> Cube::vertices() const
{
    return m_vertices;

}
