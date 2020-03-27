#pragma once

#include "pointers.h"

class QPointF;

/// Интерфейс подписчика на изменения узла графа
class GraphNodeListener
{
public:
    virtual ~GraphNodeListener() = default;

    /// Вызывается перед изменения положения узла
    virtual void beforePosChanged(GraphNodeSP node, const QPointF& newPos);

    /// Вызывается после изменения положения узла
    virtual void afterPosChanged(GraphNodeSP node, const QPointF& newPos);
};
