#include "graph_decorator.h"

#include "graph_node.h"

#include <QPainter>

#include <cmath>

void GraphDecorator::paintNode(QPainter& painter,
                               GraphNodeSP node,
                               const QTransform& transform,
                               const Style style) const
{
    const QPointF pos = transform.map(node->getPos());
    const QRectF rect(pos - QPointF{_nodeRadius, _nodeRadius},
                      QSizeF{_nodeRadius, _nodeRadius} * 2.0);

    painter.setPen(style == Style::Selected ? _selectedNodePen : _nodePen);
    painter.setBrush(_nodeBrush);
    painter.drawEllipse(rect);

    const QString title = node->getTitle();
    if (!title.isEmpty())
    {
        painter.setPen(_nodeTitlePen);
        painter.drawText(rect, Qt::AlignCenter, title);
    }
}

QRectF GraphDecorator::buindingRect(GraphNodeSP node, const QTransform& transform) const
{
    const QPointF pos = transform.map(node->getPos());
    const double margin = std::max(_nodePen.widthF(), _selectedNodePen.widthF()) / 2.0;
    const QRectF rect(pos - QPointF{_nodeRadius - margin, _nodeRadius - margin},
                      QSizeF{_nodeRadius + margin, _nodeRadius + margin} * 2.0);
    return rect;
}

QRectF GraphDecorator::buindingRect(const GraphConnection& connection, const QTransform& transform) const
{
    const double margin = std::max(_linePen.widthF(),
                                   _selectedLinePen.widthF());
    const QPointF p1 = transform.map(connection.first->getPos());
    const QPointF p2 = transform.map(connection.second->getPos());
    return QRectF(p1, p2) + QMarginsF(margin, margin, margin, margin);
}

double GraphDecorator::getCosmeticMargin() const
{
    return std::max(std::max(_nodePen.widthF(),
                             _selectedNodePen.widthF()),
                    std::max(_linePen.widthF(),
                     _selectedLinePen.widthF()));
}

void GraphDecorator::paintConnection(QPainter& painter,
                                     const QPointF& start,
                                     const QPointF& end,
                                     const GraphDecorator::Style style) const
{
    const QPointF diff = end - start;
    const double length = hypot(diff.x(), diff.y());

    const QPen pen = style == Style::Selected ? _selectedLinePen: _linePen;
    painter.setPen(pen);
    painter.setBrush(QBrush(pen.color()));

    // Длина стрелки должна быть не болше длины текущей соединительной линии
    const double arrowLength = std::min(_arrawLength, length);

    // Единичный направляющий вектор соединения
    const QPointF dir = diff / length;

    // Формируем две нормали к прямой
    const QPointF turnLeft(-dir.y(), dir.x());
    const QPointF turnRight(dir.y(), -dir.x());

    // отступ для соединительной линии, если длина линии меньше 2х радиусов узла, то рисуем без отступа
    const double offset = length < _nodeRadius * 2.0 ? 0.0 : _nodeRadius;

    // точка начала "головы" стрелки, с учетом радиуса узла
    const QPointF localStartPos = start + dir * offset;
    const QPointF localEndPos = end - dir * offset;
    const QPointF basePos = localEndPos - dir * arrowLength;
    const double arrowHalfWidth = _arrawWidth / 2.0;

    // "голова" стрелки
    QPolygonF arrowHead;
    arrowHead << localEndPos;
    arrowHead << (basePos + turnLeft * arrowHalfWidth);
    arrowHead << (basePos + turnRight * arrowHalfWidth);
    arrowHead << localEndPos;

    painter.drawLine(localStartPos, localEndPos);
    painter.drawPolygon(arrowHead);
}

void GraphDecorator::paintConnection(QPainter& painter,
                                     const GraphConnection& connection,
                                     const QTransform& transform,
                                     const GraphDecorator::Style style) const
{
    paintConnection(painter,
                    transform.map(connection.first->getPos()),
                    transform.map(connection.second->getPos()),
                    style);
}


