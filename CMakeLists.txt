cmake_minimum_required(VERSION 3.7)
project(graph-sample)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(SOURCE_FILES
    graph_decorator.cpp
    graph_decorator.h
    graph_connection.h
    graph_edit_controller.cpp
    graph_edit_controller.h
    graph_model.cpp
    graph_model.h
    graph_model_listener.cpp
    graph_model_listener.h
    graph_node.cpp
    graph_node.h
    graph_node_listener.cpp
    graph_node_listener.h
    graph_view.cpp
    graph_view.h
    graph_view_controller.cpp
    graph_view_controller.h
    graph_widget.cpp
    graph_widget.h
    graph_widget_p.cpp
    graph_widget_p.h
    main.cpp
    pointers.h
    view_buffer.cpp
    view_buffer.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

if (WIN32)
    set(DEFAULT_COMPILER_OPTIONS "/W4")
endif()
if (UNIX)
    set(DEFAULT_COMPILER_OPTIONS "-Wpedantic;-Wall;-Wextra")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS ${DEFAULT_COMPILER_OPTIONS}
        AUTOMOC ON
        RUNTIME_OUTPUT_DIRECTORY "bin"
)
