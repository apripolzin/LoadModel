#include "vertex.h"

QDebug operator<<(QDebug debug, const Vertex &v)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "Vertex(" << v.position << ", " << v.normal << ", " << v.texture_coord << ')';

    return debug;
}
