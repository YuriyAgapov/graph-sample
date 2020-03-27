#include "graph_view_controller.h"

#include "graph_view.h"

#include <QMouseEvent>
#include <QWheelEvent>

GraphViewController::GraphViewController(GraphViewSP view, QObject* parent)
    : QObject(parent)
    , _view(view)
{
    Q_ASSERT(view);
}

bool GraphViewController::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonPress:
        return mousePressEvent(static_cast<QMouseEvent*>(event));

    case QEvent::MouseButtonRelease:
        return mouseReleaseEvent(static_cast<QMouseEvent*>(event));

    case QEvent::MouseMove:
        return mouseMoveEvent(static_cast<QMouseEvent*>(event));

    case QEvent::Wheel:
        return mouseWheelEvent(static_cast<QWheelEvent*>(event));
    }

    return QObject::eventFilter(watched, event);
}

bool GraphViewController::mousePressEvent(QMouseEvent* mouseEvent)
{
    if (mouseEvent->buttons() & Qt::RightButton)
    {
        _lastPos = mouseEvent->localPos();
        return true;
    }
    else if (mouseEvent->buttons() & Qt::MiddleButton)
    {
        _view->resetTransformations();
        return true;
    }
    return false;
}

bool GraphViewController::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    if (mouseEvent->buttons() & Qt::RightButton)
    {
        const QTransform sceneTransform = _view->getSceneTransform();
        const QPointF pos = mouseEvent->localPos();
        const QPointF diff = sceneTransform.map(_lastPos) - sceneTransform.map(pos);
        QTransform transform;
        transform.translate(diff.x(), diff.y());
        _view->setSceneRect(transform.mapRect(_view->getSceneRect()));
        _lastPos = pos;
        return true;
    }
    return false;
}

bool GraphViewController::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    Q_UNUSED(mouseEvent);
    return false;
}

bool GraphViewController::mouseWheelEvent(QWheelEvent* wheelEvent)
{
    const double scaleFactor = wheelEvent->delta() > 0 ? 0.8 : 1.2;
    double scaleX = scaleFactor;
    double scaleY = scaleFactor;

    // Блокировка масштабирования по осям
    if (wheelEvent->modifiers() == Qt::ShiftModifier)
        scaleY = 1.0;
    else if (wheelEvent->modifiers() == Qt::ControlModifier)
        scaleX = 1.0;

    // Расчитываем матрицу преобразований таким образом, чтобы масштабирование происходило в области курсора
    const QPointF pos = _view->getSceneTransform().map(wheelEvent->posF());
    QTransform transform;
    transform.translate(pos.x(), pos.y());
    transform.scale(scaleX, scaleY);
    transform.translate(-pos.x(), -pos.y());
    _view->setSceneRect(transform.mapRect(_view->getSceneRect()));
    return true;
}
