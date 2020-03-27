#pragma once

#include "pointers.h"

#include <QPointF>
#include <QVariant>

/// Узел графа, содержит собственные координаты
class GraphNode : public std::enable_shared_from_this<GraphNode>
{
public:
    GraphNode();

    /// \name Координаты узла
    QPointF getPos() const;
    void setPos(const QPointF& pos);
    ///@}

    /// \name Подписчик для обработки событий изменения узла
    GraphNodeListenerWP getListener() const;
    void setListener(GraphNodeListenerWP listener);
    ///@}

    QString getTitle() const;
    void setTitle(const QString& title);

private:
    QPointF _pos;
    GraphNodeListenerWP _listener;
    QString _title;
};

