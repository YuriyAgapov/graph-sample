#pragma once

#include "graph_edit_controller.h"
#include "graph_model_listener.h"
#include "graph_node_listener.h"
#include "graph_view_controller.h"
#include "pointers.h"

#include <QTransform>

class QWidget;
class QMouseEvent;

/// реализация виджета для отображения и редактирования графа
class GraphWidgetPrivate :
        public std::enable_shared_from_this<GraphWidgetPrivate>,
        public GraphModelListener
{
public:
    GraphWidgetPrivate(QWidget& widget);

    /// \name Модель графа
    GraphModelSP getModel() const;
    void setModel(GraphModelSP model);
    ///@}

    /// Выводит изображение графа на виджет
    void paint();

    /// Изменяет размер выводимого изображения
    void resize(const QSize& size);

private:
    void update();

    // GraphModelListener interface
    void nodeAdded(GraphModelSP model, GraphNodeSP node);
    void nodeRemoved(GraphModelSP model, GraphNodeSP node);

    // GraphNodeListener interface
    void afterPosChanged(GraphNodeSP node, const QPointF& newPos);

    QWidget& _widget;
    GraphViewSP _view;
    GraphEditControllerUP _editController;
    GraphViewControllerUP _viewController;
};

