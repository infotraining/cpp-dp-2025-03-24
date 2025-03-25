#pragma once

#include <memory>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <string>
#include "shape.hpp"
#include "shape_readers_writers/shape_reader_writer.hpp"

template <
    typename TProduct, 
    typename TId = std::string, 
    typename TCreator = std::function<std::unique_ptr<TProduct>()>
>
class GenericFactory
{
    std::unordered_map<TId, TCreator> creators_;
public:
    std::unique_ptr<TProduct> create(const TId& id) const
    {
        auto& creator = creators_.at(id);
        return creator();
    }

    bool register_creator(const TId& id, TCreator creator)
    {
        return creators_.emplace(id, creator).second;
    }
};

using ShapeFactory = GenericFactory<Drawing::Shape>;
using ShapeRWFactory = GenericFactory<Drawing::IO::ShapeReaderWriter, std::type_index>;

template <typename T>
std::type_index make_type_index()
{
    return std::type_index(typeid(T));
}

template <typename T>
std::type_index make_type_index(const T& obj)
{
    return std::type_index(typeid(obj));
}

template <typename T>
class Singleton
{
public:
    Singleton() = delete;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T& instance()
    {
        static T unique_instance;
        return unique_instance;
    }
};

using SingletonShapeFactory = Singleton<ShapeFactory>;
using SingletonShapeRWFactory = Singleton<ShapeRWFactory>;