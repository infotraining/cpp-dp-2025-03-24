#ifndef COMPOSITE_HPP_
#define COMPOSITE_HPP_

#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <string>

// "Component"
class Component
{
protected:
    std::string name_;

public:
    explicit Component(const std::string& name) : name_(name)
    {
    }
	Component(const Component&) = default;
	Component& operator=(const Component&) = default;
    virtual ~Component() = default;
    
    virtual std::unique_ptr<Component> clone() const = 0;
    virtual void display(int depth) = 0;
};

template <typename TComponent, typename TBase = Component>
class CloneableComponent : public TBase
{
public:
    using TBase::TBase;

    std::unique_ptr<Component> clone() const override
    {
        return std::make_unique<TComponent>(static_cast<const TComponent&>(*this));
    }
};

using ComponentPtr = std::shared_ptr<Component>;

// "Composite"
class Composite : public CloneableComponent<Composite>
{
private:
    std::list<ComponentPtr> children;

    using Base = CloneableComponent<Composite>;
public:
    explicit Composite(const std::string& name) : Base(name)
    {
    }

    void add(ComponentPtr c)
    {
        children.push_back(c);
    }

    void remove(ComponentPtr c)
    {
        children.remove(c);
    }

    void display(int depth) override
    {
        std::cout << std::string(depth, '-') << name_ << std::endl;

        for (const auto& child : children)
            child->display(depth + 2);
    }
};

// "Leaf"
class Leaf : public CloneableComponent<Leaf>
{
    using Base = CloneableComponent<Leaf>;
public:
    explicit Leaf(const std::string& name) : Base(name)
    {
    }

    void display(int depth) override
    {
        std::cout << std::string(depth, '-') << name_ << std::endl;
    }
};

#endif /*COMPOSITE_HPP_*/
