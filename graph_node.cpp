#include "graph_node.h"

GraphNode::GraphNode()
{

}

QPointF GraphNode::getPos() const
{
    return _pos;
}

void GraphNode::setPos(const QPointF& pos)
{
    _pos = pos;
}

GraphNodeListenerWP GraphNode::getListener() const
{
    return _listener;
}

void GraphNode::setListener(GraphNodeListenerWP listener)
{
    _listener = listener;
}

QString GraphNode::getTitle() const
{
    return _title;
}

void GraphNode::setTitle(const QString& title)
{
    _title = title;
}

