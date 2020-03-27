#include "graph_node_listener.h"

#include <QtGlobal>

void GraphNodeListener::beforePosChanged(GraphNodeSP node, const QPointF& newPos)
{
    Q_UNUSED(node);
    Q_UNUSED(newPos);
    //nothing
}

void GraphNodeListener::afterPosChanged(GraphNodeSP node, const QPointF& newPos)
{
    Q_UNUSED(node);
    Q_UNUSED(newPos);
    //nothing
}
