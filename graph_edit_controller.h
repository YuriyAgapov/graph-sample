#pragma once

#include <QLineF>
#include <QObject>
#include <QPointF>

#include <set>

#include "pointers.h"

class QMouseEvent;
class QWheelEvent;

/// Реализует редактирование графа оператором
/// левая кнопка мыши - создать узел под курсором мыши
/// правая кнопка мыши (зажать и вести от одного узла к другому) - создать направленную связь между узлами
/// правая кнопка мыши (щелчек) - удалить узел под курсором
class GraphEditController : public QObject
{
public:
    GraphEditController(GraphViewSP view, QObject* parent = nullptr);

private:
    bool eventFilter(QObject* watched, QEvent* event);
    bool mousePressEvent(QMouseEvent* mouseEvent);
    bool mouseMoveEvent(QMouseEvent* mouseEvent);
    bool mouseReleaseEvent(QMouseEvent* mouseEvent);
    bool mouseWheelEvent(QWheelEvent* wheelEvent);

private:
    /// Возвращает верхний узел графа
    GraphNodeSP nodeAt(const QPointF& screenPos) const;

    /// Создает узел в указанных координатах
    GraphNodeSP createNode(const QPointF& scenePos);

    /// Создает соединение для выбранных узлов _startNode, _endNode
    void createConnection();

    /// Удаляет узел в фокусе
    void removeFocusedNode();

    GraphViewSP _view;

    /// сквозной счетчик созданных узлов, используется для присвоения им заголовка
    size_t _nodeCount = 0;
    /// текущие координаты соединения
    QLineF _connection;
    /// выбранные для соединения узлы
    GraphNodeSP _startNode;
    GraphNodeSP _endNode;
    /// признак того что мышь была смещена после зажатия кнопки
    bool _mouseMoved = false;
    /// нажатые кнопки мыши
    Qt::MouseButtons _buttons = Qt::NoButton;

    /// Предудущее положение курсора, используется для определения величины сдвига узлов при перемещении
    QPointF _lastPos;
    std::set<GraphNodeSP> _movingNodes;
};
