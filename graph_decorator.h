#pragma once

#include "pointers.h"
#include "graph_connection.h"

#include <QPen>
#include <QBrush>

class QPainter;

/// Классс для рисования элементов графа
class GraphDecorator
{
public:
    /// Стили рисования элементов графа
    enum class Style
    {
        Normal,
        Selected
    };

    /// Рисует узел \a node заданным \a painter, у положению узла применяются преобразования \a transform,
    /// размер узла фиксированный и не зависит от transform
    /// \a style - стиль рисования узла:
    ///     Normal - используется стандартная кисть,
    ///     Selected - используется кисть для выделенных узлов
    void paintNode(QPainter& painter,
                   GraphNodeSP node,
                   const QTransform& transform,
                   const Style style) const;

    /// Возвращает обрамляющую узел область
    QRectF buindingRect(GraphNodeSP node, const QTransform& transform) const;
    QRectF buindingRect(const GraphConnection& connection, const QTransform& transform) const;

    /// Возвращает величину косметического отступа (полный размер, самой толстой кисти)
    double getCosmeticMargin() const;

    /// Рисует связь между узлами
    void paintConnection(QPainter& painter,
                         const QPointF& start,
                         const QPointF& end,
                         const Style style) const;
    void paintConnection(QPainter& painter,
                         const GraphConnection& connection,
                         const QTransform& transform,
                         const Style style) const;

private:
    QPen _nodePen = {Qt::blue, 2};
    QPen _selectedNodePen = {QColor("#ff9c00"), 3};
    QPen _nodeTitlePen = {Qt::black};
    QBrush _nodeBrush = {Qt::white};
    double _nodeRadius = 15.0;

    QPen _linePen = {Qt::black, 1};
    QPen _selectedLinePen = {QColor("#ff9c00"), 2};
    double _arrawLength = 15.0;
    double _arrawWidth = 10.0;
};

