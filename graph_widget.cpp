#include "graph_widget.h"

#include "graph_widget_p.h"

#include <QResizeEvent>

GraphWidget::GraphWidget(QWidget* parent) : QWidget(parent)
    , _private(std::make_shared<GraphWidgetPrivate>(*this))
{
}

GraphWidget::~GraphWidget()
{
}

GraphModelSP GraphWidget::getModel() const
{
    return _private->getModel();
}

void GraphWidget::setModel(const GraphModelSP& model)
{
    _private->setModel(model);
}

void GraphWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    _private->paint();
}

void GraphWidget::resizeEvent(QResizeEvent* event)
{
    _private->resize(event->size());
}
