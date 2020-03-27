#include "graph_view.h"

#include "graph_decorator.h"
#include "graph_model.h"
#include "graph_node.h"

#include <QPainter>
#include <QDebug>

#include <set>

GraphView::GraphView(QObject* parent) : QObject(parent)
{
}

QRectF GraphView::getSceneRect() const
{
    return _sceneRect;
}

void GraphView::setSceneRect(const QRectF& rect)
{
    _sceneRect = rect;
    setDirty();
}

GraphModelSP GraphView::getModel() const
{
    return _model;
}

void GraphView::setModel(GraphModelSP model)
{
    _model = model;
    setDirty();
}

void GraphView::paint(QPainter& painter)
{
    if (_sceneRect.isNull() || _sceneRect.width() <= 0 || _sceneRect.height() <= 0 )
        return;

    if (_dirty)
    {
        updateContents();
    }
    _viewBuffer.paint(painter);
}

void GraphView::setSize(const QSize& size)
{
    _size = size;
    _viewBuffer.resize(size);
    // Расчет области отображения с учетом отступа
    _viewRect.setRect(_margin, _margin,
                      _size.width() - _margin * 2,
                      _size.height() - _margin * 2);
    setDirty();
}

bool GraphView::isDirty() const
{
    return _dirty;
}

void GraphView::setDirty()
{
    _dirty = true;
    emit changed();
}

QTransform GraphView::getScreenTransform() const
{
    QTransform transform;
    transform.translate(_margin + _cosmeticMargins.left(), _margin + _cosmeticMargins.top());
    transform.scale(_scaneScaleX, _scaneScaleY);
    transform.translate(-_sceneRect.x(), -_sceneRect.y());
    return transform;
}

QTransform GraphView::getSceneTransform() const
{
    QTransform transform;
    transform.translate(_sceneRect.x(), _sceneRect.y());
    transform.scale(1.0/_scaneScaleX, 1.0/_scaneScaleY);
    return transform;
}

void GraphView::sceneZoom(const double x, const double y)
{
    QTransform transform;
    const QPointF center = _sceneRect.center();
    transform.translate(center.x(), center.y());
    transform.scale(x, y);
    transform.translate(-center.x(), -center.y());
    _sceneRect = transform.mapRect(_sceneRect);
    setDirty();
}

void GraphView::resetTransformations()
{
    setSceneRect(QRectF(0, 0, _sceneRect.width(), _sceneRect.height()));
}

GraphNodeSP GraphView::getFocused() const
{
    return _focused;
}

void GraphView::setFocused(GraphNodeSP node)
{
    if (_focused == node)
        return;

    _focused = node;
    setDirty();
}

void GraphView::setLinePreview(const QLineF& linePreview)
{
    _linePreview = linePreview;
    setDirty();
}

void GraphView::updateContents()
{
    // расчет масштабных коэффициентов для
    _scaneScaleX = (_viewRect - _cosmeticMargins).width() / _sceneRect.width();
    _scaneScaleY = (_viewRect - _cosmeticMargins).height() / _sceneRect.height();

    QPainter painter(_viewBuffer.getPaintDevice());
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(QRect(0, 0, _size.width(), _size.height()), Qt::white);

    const QTransform transform = getScreenTransform();
    GraphDecorator decorator;
    paintNodes(decorator, painter, transform);
    paintFocesed(decorator, painter, transform);

    // draw control help
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawText(_viewRect,
        tr(
            "Mouse control:\n"
            " Left button (click) - create node\n"
            " Left button (hold) - drag node tree\n"
            " Right button - create node\n"
            " Right button (hold) - navigate\n"
            " Middle button - reset scene view\n"
            " Wheel Up/Down - zoom in/out (use shift and ctrl for lock axis by X/Y)."
        )
    );

    _dirty = false;
}

void GraphView::paintFocesed(const GraphDecorator& decorator, QPainter& painter, const QTransform& transform) const
{
    if (!_linePreview.isNull())
    {
        // предпросмотр для соединения узлов
        decorator.paintConnection(painter,
                                  transform.map(_linePreview.p1()),
                                  transform.map(_linePreview.p2()),
                                  GraphDecorator::Style::Selected);
    }
}

void GraphView::paintNodes(const GraphDecorator& decorator, QPainter& painter, const QTransform& transform) const
{
    // рисуем узлы
    const QPolygonF viewPolygon = QPolygonF(_viewRect);
    std::set<GraphConnection> connections;

    _model->forEachNodes([&painter, viewPolygon, &decorator, transform, this, &connections](GraphNodeSP node) -> bool
    {
        const QPolygonF nodePolygon(decorator.buindingRect(node, transform));
        if (viewPolygon.intersects(nodePolygon))
        {
            decorator.paintNode(painter, node, transform,
                                _focused == node ? GraphDecorator::Style::Selected
                                                 : GraphDecorator::Style::Normal);
            for (auto neibor : _model->getOutConnections(node))
                connections.insert({node, neibor});
            for (auto neibor : _model->getInConnections(node))
                connections.insert({neibor, node});
        }
        return true;
    });

    for (auto connection : connections)
        decorator.paintConnection(painter, connection, transform, GraphDecorator::Style::Normal);

}
