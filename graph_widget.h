#pragma once

#include "pointers.h"

#include <QWidget>

class GraphWidgetPrivate;

/// Виджет для отображения и редактирования графа
class GraphWidget : public QWidget
{
public:
    explicit GraphWidget(QWidget* parent = nullptr);
    virtual ~GraphWidget();

    /// \name Модель графа
    GraphModelSP getModel() const;
    void setModel(const GraphModelSP& getModel);
    ///@}

private:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    std::shared_ptr<GraphWidgetPrivate> _private;
};
