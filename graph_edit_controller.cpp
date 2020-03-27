#include "graph_edit_controller.h"

#include "graph_view.h"
#include "graph_node.h"
#include "graph_model.h"
#include "graph_decorator.h"

#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

#include <set>

GraphEditController::GraphEditController(GraphViewSP view, QObject* parent)
    : QObject(parent)
    , _view(view)
{
    Q_ASSERT(view);
}

bool GraphEditController::eventFilter(QObject* watched, QEvent* event)
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

    default:
        //nothing
        break;
    }

    return QObject::eventFilter(watched, event);
}

bool GraphEditController::mousePressEvent(QMouseEvent* mouseEvent)
{
    _mouseMoved = false;
    _buttons = mouseEvent->buttons();

    if (_buttons == Qt::LeftButton)
    {
        _startNode = _view->getFocused();
        const QTransform transform = _view->getSceneTransform();
        _lastPos = transform.map(mouseEvent->localPos());
        // режим добавления
        if (!_startNode)
        {
            _startNode = createNode(_lastPos);
        }
        return true;
    }
    // подготовка к созданию связи между узлами
    else if (_buttons == Qt::RightButton)
    {
        _startNode = _view->getFocused();
        if (_startNode)
        {
            const QPointF nodePos = _startNode->getPos();
            _connection = QLineF(nodePos, nodePos + QPointF(1,1));
        }
    }
    return false;
}

bool GraphEditController::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    _mouseMoved = true;

    const QPointF pos = mouseEvent->localPos();

    GraphNodeSP focusedNode = nodeAt(pos);
    _view->setFocused(focusedNode);

    // режим перемещения связанных узлов
    if (_startNode && _buttons == Qt::LeftButton)
    {
        if (_movingNodes.empty())
        {
            _movingNodes = _view->getModel()->findConnected(_startNode);
            _movingNodes.insert(_startNode);
        }

        const QTransform transform = _view->getSceneTransform();
        const QPointF pos = transform.map(mouseEvent->localPos());
        const QPointF offset = _lastPos - pos;

        for (auto node : _movingNodes)
        {
            node->setPos(node->getPos() - offset);
        }
        _view->setDirty();

        _lastPos = pos;
    }

    // режим соединения
    if (_startNode && _buttons == Qt::RightButton)
    {
        // определяем конец соединительной линии, если находимся в режиме соединения
        _endNode = focusedNode;
        const QPointF scenePos = _endNode && _startNode != focusedNode
                ? _endNode->getPos()
                : _view->getSceneTransform().map(pos);
        _connection.setP2(scenePos);
        _view->setLinePreview(_connection);
        return true;
    }

    return false;
}

bool GraphEditController::mouseReleaseEvent(QMouseEvent* mouseEvent)
{
    // создание нового узла
    if (_buttons == Qt::LeftButton)
    {
        if (_mouseMoved)
        {
            _movingNodes.clear();
        }
    }
    else if (_buttons == Qt::RightButton)
    {
        // завершаем процедуру соединения
        if (_mouseMoved)
        {
            createConnection();
        }
        // удаляем узел в фокусе, если был щелчек ПКМ
        else
        {
            removeFocusedNode();
        }
    }

    // очистка состояния
    _connection = QLineF();
    _startNode = nullptr;
    _endNode = nullptr;
    _startNode = nullptr;
    _endNode = nullptr;
    _view->setLinePreview(_connection);

    return false;
}

bool GraphEditController::mouseWheelEvent(QWheelEvent* wheelEvent)
{
    Q_UNUSED(wheelEvent);
    return false;
}

GraphNodeSP GraphEditController::nodeAt(const QPointF& screenPos) const
{
    // Поиск в экранных координатах
    const QTransform transform = _view->getScreenTransform();

    GraphNodeSP result;
    GraphDecorator decorator;
    _view->getModel()->forEachNodes([screenPos, &decorator, transform, &result](GraphNodeSP node) -> bool
    {
        const QRectF nodeRect = decorator.buindingRect(node, transform);
        if (nodeRect.contains(screenPos))
        {
            result = node;
        }
        return true;
    });
    return result;
}

GraphNodeSP GraphEditController::createNode(const QPointF& scenePos)
{
    auto node = _view->getModel()->createNode(scenePos);
    node->setTitle(QString::number(++_nodeCount));
    _view->setFocused(node);
    return node;
}

void GraphEditController::createConnection()
{
    if (!_startNode || !_endNode)
        return;

    auto model = _view->getModel();

    if (model->isConnectionExists(_startNode, _endNode))
        return;

    model->addConnection({_startNode, _endNode});
}

void GraphEditController::removeFocusedNode()
{
    auto node = _view->getFocused();
    if (!node)
        return;

    _view->setFocused(nullptr);
    _view->getModel()->removeNode(node);
}

