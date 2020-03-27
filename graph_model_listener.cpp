#include "graph_model_listener.h"

#include <QtGlobal>

void GraphModelListener::nodeAdded(GraphModelSP model, GraphNodeSP node)
{
    Q_UNUSED(model);
    Q_UNUSED(node);
}

void GraphModelListener::nodeRemoved(GraphModelSP model, GraphNodeSP node)
{
    Q_UNUSED(model);
    Q_UNUSED(node);
}
