#include "graph_widget_p.h"

#include "graph_model.h"
#include "graph_view.h"

#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <QDebug>

GraphWidgetPrivate::GraphWidgetPrivate(QWidget& widget)
    : _widget(widget)
    , _view(std::make_shared<GraphView>())
    , _editController(std::make_unique<GraphEditController>(_view))
    , _viewController(std::make_unique<GraphViewController>(_view))
{
    _widget.installEventFilter(_viewController.get());
    _widget.installEventFilter(_editController.get());
    _widget.setMouseTracking(true);
    _view->setSize(_widget.size());
    _view->setSceneRect({{0.0, 0.0}, _widget.size()});
    _view->connect(_view.get(), &GraphView::changed, [this]()
    {
        _widget.update();
    });
}

GraphModelSP GraphWidgetPrivate::getModel() const
{
    return _view->getModel();
}

void GraphWidgetPrivate::setModel(GraphModelSP model)
{
    //
    auto oldModel = _view->getModel();
    if (oldModel)
        oldModel->removeListener(shared_from_this());

    _view->setModel(model);

    if (model)
        model->addListener(shared_from_this());
}

void GraphWidgetPrivate::paint()
{
    QPainter painter(&_widget);
    _view->paint(painter);
}

void GraphWidgetPrivate::resize(const QSize& size)
{
    _view->setSize(size);
}

void GraphWidgetPrivate::update()
{
    _view->setDirty();
}

void GraphWidgetPrivate::nodeAdded(GraphModelSP model, GraphNodeSP node)
{
    Q_UNUSED(model);
    Q_UNUSED(node);

    //TODO: на основе информации о узле, можно реализовать частичное обновление кадра
    update();
}

void GraphWidgetPrivate::nodeRemoved(GraphModelSP model, GraphNodeSP node)
{
    Q_UNUSED(model);
    Q_UNUSED(node);

    //TODO: на основе информации о узле, можно реализовать частичное обновление кадра
    update();
}
void GraphWidgetPrivate::afterPosChanged(GraphNodeSP node, const QPointF& newPos)
{
    Q_UNUSED(node);
    Q_UNUSED(newPos);

    //TODO: на основе информации о узле, можно реализовать частичное обновление кадра
    update();
}
