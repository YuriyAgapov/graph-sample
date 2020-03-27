#include "graph_model.h"

#include "graph_model_listener.h"
#include "graph_node.h"

#include <QDebug>

inline bool operator == (const GraphModelListenerWP& l, const GraphModelListenerWP& r)
{
    return l.lock() == r.lock();
}

GraphModel::GraphModel()
{
}

GraphNodeSP GraphModel::createNode(const QPointF& pos)
{
    auto node = std::make_shared<GraphNode>();
    node->setPos(pos);
    addNode(node);
    return node;
}

void GraphModel::addNode(GraphNodeSP node)
{
    _nodes.push_back(node);

    for (auto weakListener : _listeners)
    {
        auto listener = weakListener.lock();
        if (listener)
            listener->nodeAdded(shared_from_this(), node);
    }
}

void GraphModel::removeNode(GraphNodeSP node)
{
    using namespace std;

    auto iter = find(begin(_nodes), end(_nodes), node);
    if (iter == end(_nodes))
        return;
    _nodes.erase(iter);

    removeConnectionsForNode(node);

    for (auto weakListener : _listeners)
    {
        auto listener = weakListener.lock();
        if (listener)
            listener->nodeRemoved(shared_from_this(), node);
    }
}

bool GraphModel::isConnectionExists(GraphNodeSP start, GraphNodeSP end) const
{
    auto iter = _connections.find(start);
    if (iter == std::end(_connections))
        return false;

    const auto& connections = iter->second;

    return std::find(connections.begin(), connections.end(), end) != std::end(connections);
}

void GraphModel::addConnection(const GraphConnection& connection)
{
    _connections[connection.first].emplace_back(connection.second);
}

void GraphModel::removeConnectionsForNode(GraphNodeSP node)
{
    auto iter = _connections.find(node);
    if (iter != _connections.end())
        _connections.erase(iter);

    for (auto& pair : _connections)
    {
        auto innerIter = std::find(pair.second.begin(), pair.second.end(), node);
        if (innerIter == pair.second.end())
            continue;
        pair.second.erase(innerIter);
    }
}

void GraphModel::forEachNodes(std::function<bool (GraphNodeSP)> predicate) const
{
    for (auto node : _nodes)
    {
        if (!predicate(node))
            return;
    }
}

std::set<GraphNodeSP> GraphModel::findConnected(GraphNodeSP node) const
{
    std::set<GraphNodeSP> processed;
    Nodes pending = getOutConnections(node);

    while (!pending.empty())
    {
        GraphNodeSP neibor = pending.front();
        pending.erase(pending.begin());

        if (processed.count(neibor) > 0)
            continue;

        processed.insert(neibor);

        auto iter = _connections.find(neibor);
        if (iter == std::end(_connections))
            continue;

        const auto& connections = iter->second;
        pending.insert(pending.end(), connections.begin(), connections.end());
    }
    return processed;
}

void GraphModel::addListener(const GraphModelListenerWP& listener)
{
    _listeners.emplace_back(listener);
}

void GraphModel::removeListener(const GraphModelListenerWP& listener)
{
    using namespace std;
    _listeners.erase(find(begin(_listeners), end(_listeners), listener));
}

GraphModel::Nodes GraphModel::getOutConnections(GraphNodeSP node) const
{
    auto iter = _connections.find(node);
    return iter == std::end(_connections) ? Nodes()
        : iter->second;
}

GraphModel::Nodes GraphModel::getInConnections(GraphNodeSP node) const
{
    Nodes nodes;
    for (auto& pair : _connections)
    {
        auto innerIter = std::find(pair.second.begin(), pair.second.end(), node);
        if (innerIter == pair.second.end())
            continue;
        nodes.emplace_back(pair.first);
    }
    return nodes;
}

void GraphModel::beforePosChanged(GraphNodeSP node, const QPointF& newPos)
{
    GraphNodeListener::beforePosChanged(node, newPos);
}

void GraphModel::afterPosChanged(GraphNodeSP node, const QPointF& newPos)
{
    GraphNodeListener::afterPosChanged(node, newPos);
}
