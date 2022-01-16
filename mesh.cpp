#include "mesh.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

static float toFloat(const QString &s) {
    bool ok = false;
    const float rezult = s.toFloat(&ok);
    Q_ASSERT(ok);
    return rezult;
}

static int toInt(const QString &s) {
    bool ok = false;
    const float rezult = s.toInt(&ok);
    Q_ASSERT(ok);
    return rezult;
}

struct Face {
    const int vertex_index;
    const int vertex_texture_index;
    const int vertex_normal_index;
};

static QVector3D processVertex(const QString &line);
static QVector2D processVertexTexture(const QString &line);
static QVector3D processVertexNormal(const QString &line);
static QList<Face> processFace(const QString &line);
static Face parseFace(const QString &str);

Mesh::Mesh(const QString &objfilename)
{
    QFile f(objfilename);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&f);

        QList<QVector3D> vertices;
        QList<QVector2D> texture_coords;
        QList<QVector3D> vertex_normals;

        vertices.append(QVector3D()); // dummy
        texture_coords.append(QVector2D()); // dummy
        vertex_normals.append(QVector3D()); // dummy
        QList<Face> faces;

        while (!in.atEnd()) {
            const QString line = in.readLine();
            if (line.startsWith(QLatin1String("v "))) {
                vertices.append( processVertex(line) );
            } else if (line.startsWith(QLatin1String("vt"))) {
                texture_coords.append( processVertexTexture(line) );
            } else if (line.startsWith(QLatin1String("vn"))) {
                vertex_normals.append( processVertexNormal(line) );
            } else if (line.startsWith(QLatin1String("f"))) {
                 faces.append(processFace(line));
            }
        }

        for (const Face &f : faces) {
            Vertex v = {vertices.at(f.vertex_index), vertex_normals.at(f.vertex_normal_index), texture_coords.at(f.vertex_texture_index)};
            m_vertices.push_back(v);
        }
    }
}

QList<Vertex> Mesh::vertices() const
{
    return m_vertices;
}

QVector3D processVertex(const QString &line)
{
    const QStringList values =  line.split(' ', Qt::SkipEmptyParts);
    Q_ASSERT(values.size() == 4);
    return { toFloat(values.at(1)), toFloat(values.at(2)), toFloat(values.at(3)) };
}

QVector2D processVertexTexture(const QString &line)
{
    const QStringList vertices =  line.split(' ', Qt::SkipEmptyParts);
    Q_ASSERT(vertices.size() == 3);
    return { toFloat(vertices.at(1)), toFloat(vertices.at(2)) };
}

QVector3D processVertexNormal(const QString &line)
{
    const QStringList vertices =  line.split(' ', Qt::SkipEmptyParts);
    Q_ASSERT(vertices.size() == 4);
    return { toFloat(vertices.at(1)), toFloat(vertices.at(2)), toFloat(vertices.at(3)) };
}

QList<Face> processFace(const QString &line)
{
    const QStringList faces =  line.split(' ', Qt::SkipEmptyParts);

    QList<Face> rezult;
    for (auto it = ++faces.begin(); it != faces.end(); ++it) {
        rezult.append(parseFace(*it));
    }
    return rezult;
}

Face parseFace(const QString &str)
{
    const QStringList index_texture_normal = str.split(QLatin1Char('/'));
    Q_ASSERT(index_texture_normal.size() == 3);
    return {toInt(index_texture_normal.at(0)), toInt(index_texture_normal.at(1)), toInt(index_texture_normal.at(2))};
}
