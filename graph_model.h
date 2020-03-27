#pragma once

#include "graph_node_listener.h"
#include "graph_connection.h"
#include "pointers.h"

#include <QRectF>
#include <QVariant>

#include <vector>
#include <set>
#include <unordered_map>

class QPointF;
class QString;

/// Модель графа, содержит узлы графа и соединения
class GraphModel :
        public std::enable_shared_from_this<GraphModel>,
        public GraphNodeListener
{
public:
    using Nodes = std::vector<GraphNodeSP>;

    GraphModel();

    /// Создает и добавляет в модель новый узел
    GraphNodeSP createNode(const QPointF& pos);

    /// Добавляет в модель узел
    void addNode(GraphNodeSP node);

    /// Удаляет из модели узел и связанные с ним соединения
    void removeNode(GraphNodeSP node);

    /// Возвращает true если указанные узлы уже соеденены, учитывает направление соединения
    bool isConnectionExists(GraphNodeSP start, GraphNodeSP end) const;

    /// Добавляет новое соединение, наличие дубикатов не проверяется \sa isConnectionExists
    void addConnection(const GraphConnection& connection);

    /// Удаляет все соединения связанные с заданным узлом
    void removeConnectionsForNode(GraphNodeSP node);

    /// Вызывает заданный предикат для всех узлов, пока предикат не вернет false
    void forEachNodes(std::function<bool (GraphNodeSP)> predicate) const;

    /// Возвращает все узлы связанные с заданным узлом, с учетом направления связей
    std::set<GraphNodeSP> findConnected(GraphNodeSP node) const;

    /// \name Управление подписчиками
    void addListener(const GraphModelListenerWP& listener);
    void removeListener(const GraphModelListenerWP& listener);
    ///@}

    /// Возвращает исходящие соединения для указанного узла
    Nodes getOutConnections(GraphNodeSP node) const;

    /// Возвращает исходящие и входящие соединения для указанного узла
    Nodes getInConnections(GraphNodeSP node) const;

private:
    void beforePosChanged(GraphNodeSP node, const QPointF& newPos) override;
    void afterPosChanged(GraphNodeSP node, const QPointF& newPos) override;

    Nodes _nodes;
    std::vector<GraphModelListenerWP> _listeners;
    std::unordered_map<GraphNodeSP, Nodes> _connections;
};
