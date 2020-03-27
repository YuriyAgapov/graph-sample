#pragma once

#include <QSize>
#include <QPixmap>

/// Класс буфера кадра
/// Обеспечивает изменение размера кадра
class ViewBuffer
{
public:
    ViewBuffer();

    /// Изменить размер буфера кадра,
    void resize(const QSize& size);

    /// Возвращает указатель (буз передачи владения) на устройство рисования, используется для передачи в QPainter
    QPaintDevice* getPaintDevice();

    /// Выводит текущий кадр
    void paint(QPainter& painter) const;

private:
    /// размер прирощения при изменении размера
    int _cellSize = 256;
    QSize _size = {_cellSize, _cellSize};
    QPixmap _pixmap;
};

