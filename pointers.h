/// \file Содержит предъобъявление указателей для основных типов данных проекта
#pragma once

#include <memory>

/// Декларирует псевдонимы для типов умных указателей вида:
/// * разделяемый - ClassSP;
/// * слабый - ClassWP;
/// * уникальный - ClassUP.
#define DECLARE_PTR(Class) \
    class Class;\
    using Class##SP = std::shared_ptr<Class>; \
    using Class##WP = std::weak_ptr<Class>; \
    using Class##UP = std::unique_ptr<Class>;

/// Предекларация умных указателей для основных типов
DECLARE_PTR(GraphEditController)
DECLARE_PTR(GraphModel)
DECLARE_PTR(GraphModelListener)
DECLARE_PTR(GraphNode)
DECLARE_PTR(GraphNodeListener)
DECLARE_PTR(GraphView)
DECLARE_PTR(GraphViewController)
