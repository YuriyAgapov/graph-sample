#pragma once

#include "graph_node_listener.h"
#include "pointers.h"

/// Интерфейс подписчика на изменения модели графа
class GraphModelListener : public GraphNodeListener
{
public:
    virtual ~GraphModelListener() = default;

    /// Вызывается после добавления узла в модель
    virtual void nodeAdded(GraphModelSP model, GraphNodeSP node);

    /// вызывается после удаления узла из модели
    virtual void nodeRemoved(GraphModelSP model, GraphNodeSP node);
};

