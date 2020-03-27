#pragma once

#include <QObject>
#include <QPointF>

#include "pointers.h"

class QMouseEvent;
class QWheelEvent;

/// \brief Реализует управление отображением графа
/// зажатая правая кнопка мыши - перемещение
/// средняя кнопка мыши - сбросить настройки отображения
/// колесо мыши вверх/вниз - масштабирование (shift - не масштабировать по оси Y, ctrl - не масштабировать по оси X)
class GraphViewController : public QObject
{
public:
    GraphViewController(GraphViewSP view, QObject* parent = nullptr);

private:
    bool eventFilter(QObject* watched, QEvent* event);
    bool mousePressEvent(QMouseEvent* mouseEvent);
    bool mouseMoveEvent(QMouseEvent* mouseEvent);
    bool mouseReleaseEvent(QMouseEvent* mouseEvent);
    bool mouseWheelEvent(QWheelEvent* wheelEvent);

private:
    QPointF _lastPos;
    GraphViewSP _view;
};
