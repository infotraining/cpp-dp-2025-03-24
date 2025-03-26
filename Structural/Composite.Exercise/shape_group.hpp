#ifndef SHAPEGROUP_HPP
#define SHAPEGROUP_HPP

#include <memory>
#include <vector>

#include "shape.hpp"

namespace Drawing
{    
    class ShapeGroup : public CloneableShape<ShapeGroup>
    {
        std::vector<std::unique_ptr<Shape>> shapes_;
    public:
        using const_iterator = std::vector<std::unique_ptr<Shape>>::const_iterator;
        using iterator = std::vector<std::unique_ptr<Shape>>::iterator;

        constexpr static auto id = "ShapeGroup";

        ShapeGroup() = default;

        ShapeGroup(const ShapeGroup& other)
        {
            for (const auto& shp : other.shapes_)
                shapes_.push_back(shp->clone());
        }

        ShapeGroup& operator=(const ShapeGroup& other)
        {
            if (this != &other)
            {
                ShapeGroup tmp{other};
                swap(tmp);
            }

            return *this;
        }

        ShapeGroup(ShapeGroup&& other) = default;
        ShapeGroup& operator=(ShapeGroup&& other) = default;

        void swap(ShapeGroup& other) noexcept
        {
            shapes_.swap(other.shapes_);
        }

        void add(std::unique_ptr<Shape> shp)
        {
            shapes_.push_back(std::move(shp));
        }

        void move(int dx, int dy) override
        {
            for (auto& shp : shapes_)
                shp->move(dx, dy);
        }

        void draw() const override
        {
            for (const auto& shp : shapes_)
                shp->draw();
        }

        size_t size() const
        {
            return shapes_.size();
        }

        iterator begin()
        {
            return shapes_.begin();
        }

        iterator end()
        {
            return shapes_.end();
        }

        const_iterator begin() const
        {
            return shapes_.begin();
        }

        const_iterator end() const
        {
            return shapes_.end();
        }
    };
}

#endif // SHAPEGROUP_HPP
