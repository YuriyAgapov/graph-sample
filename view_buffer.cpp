#include "view_buffer.h"

#include <QDebug>
#include <QPainter>

#include <cmath>

template <typename Type>
inline int adjustTopValue(const Type value, const Type step)
{
    return ::ceil(value / static_cast<double>(step)) * step;
}

ViewBuffer::ViewBuffer()
{

}

void ViewBuffer::resize(const QSize& size)
{
    if (_size.width() >= size.width() && _size.height() >= size.height())
        return;

    const int width = adjustTopValue(size.width(), _cellSize);
    const int height = adjustTopValue(size.height(), _cellSize);

    _size = {width, height};
    _pixmap = QPixmap{_size};
}

QPaintDevice* ViewBuffer::getPaintDevice()
{
    return &_pixmap;
}

void ViewBuffer::paint(QPainter& painter) const
{
    painter.drawPixmap(0, 0, _pixmap);
}
