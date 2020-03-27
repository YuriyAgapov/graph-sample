#include <QApplication>

#include "graph_widget.h"
#include "graph_model.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    auto widget = std::make_shared<GraphWidget>();
    widget->setModel(std::make_shared<GraphModel>());
    widget->show();

    return app.exec();
}
