#pragma once

#include "pointers.h"
#include "view_buffer.h"

#include <QTransform>

class GraphDecorator;

/// Реализует формирование буфера изображения графа
class GraphView : public QObject
{
    Q_OBJECT
public:
    GraphView(QObject* parent = nullptr);

    /// \name Матрица преобразования для отображения
    QTransform getTransform() const;
    void setTransform(const QTransform& getTransform);
    ///@}

    /// Get visible scene rect
    QRectF getSceneRect() const;

    /// Set visible scene rect
    void setSceneRect(const QRectF & rect);

    /// \name Модель графа
    GraphModelSP getModel() const;
    void setModel(GraphModelSP model);
    ///@}

    /// Вывести текущее изображение в заданный painter
    void paint(QPainter& painter);

    /// Задать размер изображения
    void setSize(const QSize& size);

    /// \name Признак того что буфер изображения утратил актуальность
    bool isDirty() const;
    void setDirty();
    ///@}

    /// Возвращает матрицу преобразований для преобразования из координат сцены в экранные координаты
    QTransform getScreenTransform() const;

    /// Возвращает матрицу преобразований для преобразования из экранных координат в координаты сцены
    QTransform getSceneTransform() const;

    /// Изменить масштаб отображения
    void sceneZoom(const double x, const double y);

    /// Расчитывает область отображения таким образом, чтобы отобразить все объекты
    void resetTransformations();

    /// \name Узел находящейся в "фокусе", данный узел будет отображен специальным образом (для режима редактирования)
    GraphNodeSP getFocused() const;
    void setFocused(GraphNodeSP node);
    ///@}

    /// Задать параметры для отображения соединительной линии в режиме предпросмотра (для режима редактирования)
    void setLinePreview(const QLineF& linePreview);

signals:
    /// Сигнал имитируется всякий раз, когда буфер кадра помечается как неактуальный
    void changed();

private:
    void updateContents();
    void paintFocesed(const GraphDecorator& decorator, QPainter& painter, const QTransform& transform) const;
    void paintNodes(const GraphDecorator& decorator, QPainter& painter, const QTransform& transform) const;

    QSize _size;
    GraphModelSP _model;
    ViewBuffer _viewBuffer;
    bool _dirty = true;

    // отступ для области отображения
    int _margin = 1;

    // масштабные коэффициенты для масштабирования области контента сцены в область отображения
    double _scaneScaleX = 1.0;
    double _scaneScaleY = 1.0;

    // Косметические отступы
    QMarginsF _cosmeticMargins;

    // Область сцены которая выводиться в область отображения
    QRectF _sceneRect = {0, 0, 100, 100};

    // Область отображения
    QRectF _viewRect;

    GraphNodeSP _focused;
    QLineF _linePreview;
};

