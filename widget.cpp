#include "widget.h"

#include <QImage>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QDateTime>
#include <QKeyEvent>
#include <QtMath>
#include <QElapsedTimer>

#include "cube.h"
#include "cone.h"
#include "mesh.h"
#include "obj_loader.h"



Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , meshVertexBuffer(QOpenGLBuffer::VertexBuffer)
    , meshIndexBuffer(QOpenGLBuffer::IndexBuffer)
    , lampVertexBuffer(QOpenGLBuffer::VertexBuffer)
{
}

Widget::~Widget()
{
    meshVertexBuffer.destroy();
    lampVertexBuffer.destroy();
}

void Widget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    initializeMeshGeometry();
    initializeLampGeometry();
    initializeShaders();

    container = initializeTexture(":/container2.png");
    container_specular = initializeTexture(":/container2_specular.png");
}

void Widget::resizeGL(int w, int h)
{
    camera.setViewport(w, h);
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.update();

    static constexpr QVector3D meshPositions[] = {
        QVector3D( 0.0f,  0.0f,  0.0f),
//        QVector3D( 2.0f,  5.0f, -15.0f),
//        QVector3D(-1.5f, -2.2f, -2.5f),
//        QVector3D(-3.8f, -2.0f, -12.3f),
//        QVector3D( 2.4f, -0.4f, -3.5f),
//        QVector3D(-1.7f,  3.0f, -7.5f),
//        QVector3D( 1.3f, -2.0f, -2.5f),
//        QVector3D( 1.5f,  2.0f, -2.5f),
//        QVector3D( 1.5f,  0.2f, -1.5f),
//        QVector3D(-1.3f,  1.0f, -1.5f)
    };
    int i = 0;
    for (const auto &position : meshPositions) {
        const float angle =  20.0f * i;
        ++i;
        drawMesh(position, angle);
    }
    for (const auto &position : lightPositions) {
        drawLamp(position);
    }

    update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        camera.move(Camera::MoveDirection::Direction_Forward);
        break;
    case Qt::Key_S:
        camera.move(Camera::MoveDirection::Direction_Backward);
        break;
    case Qt::Key_A:
        camera.move(Camera::MoveDirection::Direction_Left);
        break;
    case Qt::Key_D:
        camera.move(Camera::MoveDirection::Direction_Right);
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        camera.move(Camera::MoveDirection::Direction_Forward, false);
        break;
    case Qt::Key_S:
        camera.move(Camera::MoveDirection::Direction_Backward, false);
        break;
    case Qt::Key_A:
        camera.move(Camera::MoveDirection::Direction_Left, false);
        break;
    case Qt::Key_D:
        camera.move(Camera::MoveDirection::Direction_Right, false);
        break;
    default:
        break;
    }
    QWidget::keyPressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    static QPointF lastPos = event->pos();
    QPointF currentPos = event->pos();

    float xoffset = currentPos.x() - lastPos.x();
    float yoffset = -(currentPos.y() - lastPos.y());

    lastPos = currentPos;

    static constexpr float sensitivity = 0.2f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.rotate(xoffset, yoffset);
    QWidget::mouseMoveEvent(event);
}

void Widget::wheelEvent(QWheelEvent *event)
{
    camera.zoom(event->angleDelta().y()/100.0f);
    QWidget::wheelEvent(event);
}

void Widget::initializeMeshGeometry()
{
    const IndexedModel im = OBJModel(":/monkey3.obj").ToIndexedModel();
    const auto mesh_vertices = im.toVerticesArray();
    const auto indices = im.indices;
    m_meshDrawCount = im.indices.size();

    meshVao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&meshVao);

    meshVertexBuffer.create();
    meshVertexBuffer.bind();
    meshVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    meshVertexBuffer.allocate(mesh_vertices.data(), mesh_vertices.size() * sizeof(Vertex));

    //  location, size(vec3), type, nomalize, stride(step), start position (offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    meshIndexBuffer.create();
    meshIndexBuffer.bind();
    meshIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    meshIndexBuffer.allocate(indices.data(), sizeof(indices[0]) * indices.size());
}

void Widget::initializeLampGeometry()
{
    static const Cube cube;
    static const auto cube_vertices = cube.vertices();
    m_lampDrawCount = cube.drawCount();

    lampVao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&lampVao);

    lampVertexBuffer.create();
    lampVertexBuffer.bind();
    lampVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    lampVertexBuffer.allocate(cube_vertices.data(), cube_vertices.size() * sizeof(Vertex));

    //  location, size(vec3), type, nomalize, stride(step), start position (offset)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
}

void Widget::initializeShaders()
{
    if (!meshProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vcube.vsh")
            || !meshProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fcube.fsh")
            || !meshProgram.link()) {
        qCritical() << "Shader error";
        close();
    }
    if (!lampProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vlamp.vsh")
            || !lampProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/flamp.fsh")
            || !lampProgram.link()) {
        qCritical() << "Shader error";
        close();
    }
}

QOpenGLTexture *Widget::initializeTexture(const QString &path)
{
    QOpenGLTexture *texture = new QOpenGLTexture(QImage(path).mirrored());

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);

    return texture;
}


void Widget::drawMesh(const QVector3D &position, float angle)
{
    meshProgram.bind();

    QMatrix4x4 model;
    model.translate(position);
    model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
    QMatrix4x4 view = camera.view();
    QMatrix4x4 projection = camera.projection();

    meshProgram.setUniformValue("model", model);
    meshProgram.setUniformValue("view", view);
    meshProgram.setUniformValue("projection", projection);
    meshProgram.setUniformValue("viewPos", camera.position());

    glActiveTexture(GL_TEXTURE0);
    container->bind();
    glActiveTexture(GL_TEXTURE1);
    container_specular->bind();

    // material
    meshProgram.setUniformValue("material.diffuse",  0);
    meshProgram.setUniformValue("material.specular", 1);
    meshProgram.setUniformValue("material.shininess", 32.0f);

    //direct light
    meshProgram.setUniformValue("dirLight.ambient", QVector3D(0.2f, 0.2f, 0.2f));
    meshProgram.setUniformValue("dirLight.diffuse", QVector3D(0.5f, 0.5f, 0.5f));
    meshProgram.setUniformValue("dirLight.specular", QVector3D(1.0f, 1.0f, 1.0f));
    meshProgram.setUniformValue("dirLight.direction", QVector3D(-0.2f, -1.0f, -0.3f));

    //point light
    int i = 0;
    for (const QVector3D &lightPosition : lightPositions) {
        const QByteArray prefix = QString("pointLight[%1]").arg(i).toUtf8(); ++i;

        meshProgram.setUniformValue(prefix + ".position", lightPosition);

        meshProgram.setUniformValue(prefix + ".ambient", QVector3D(0.2f, 0.2f, 0.2f));
        meshProgram.setUniformValue(prefix + ".diffuse", QVector3D(0.5f, 0.5f, 0.5f));
        meshProgram.setUniformValue(prefix + ".specular", QVector3D(1.0f, 1.0f, 1.0f));

        meshProgram.setUniformValue(prefix + ".constant", 1.0f);
        meshProgram.setUniformValue(prefix + ".linear", 0.09f);
        meshProgram.setUniformValue(prefix + ".quadratic", 0.032f);
    }

    // spot light
    meshProgram.setUniformValue("spotLight.position",  camera.position());
    meshProgram.setUniformValue("spotLight.direction", camera.front());
    meshProgram.setUniformValue("spotLight.cutOff",   qCos(qDegreesToRadians(12.5f)));
    meshProgram.setUniformValue("spotLight.outerCutOff", qCos(qDegreesToRadians(17.5f)));

    meshProgram.setUniformValue("spotLight.ambient", 0.2f, 0.2f, 0.2f);
    meshProgram.setUniformValue("spotLight.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
    meshProgram.setUniformValue("spotLight.specular", 1.0f, 1.0f, 1.0f);

    meshProgram.setUniformValue("spotLight.constant",  1.0f);
    meshProgram.setUniformValue("spotLight.linear",    0.09f);
    meshProgram.setUniformValue("spotLight.quadratic", 0.032f);

    QOpenGLVertexArrayObject::Binder vaoBinder(&meshVao);

    glDrawElements(GL_TRIANGLES, m_meshDrawCount, GL_UNSIGNED_INT, 0);
    meshProgram.release();
}

void Widget::drawLamp(const QVector3D &position)
{
    lampProgram.bind();

    QMatrix4x4 model;
    model.translate(position);
    model.scale({0.2f, 0.2f, 0.2f});

    QMatrix4x4 view = camera.view();
    QMatrix4x4 projection = camera.projection();

    lampProgram.setUniformValue("model", model);
    lampProgram.setUniformValue("view", view);
    lampProgram.setUniformValue("projection", projection);

    QOpenGLVertexArrayObject::Binder vaoBinder(&lampVao);
    glDrawArrays(GL_TRIANGLES, 0, m_lampDrawCount);

    lampProgram.release();
}

