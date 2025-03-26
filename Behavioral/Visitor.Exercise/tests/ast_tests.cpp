#include "ast.hpp"
#include "visitors.hpp"
#include <variant>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace AST;
using namespace AST::helpers;

TEST_CASE("expression evaluator visitor", "[ast]")
{
    ExprEvalVisitor visitor;

    SECTION("integer")
    {
        auto expr = integer(4);
        expr->accept(visitor);

        REQUIRE(visitor.result() == 4);
    }

    SECTION("addition")
    {
        auto expr = add(integer(1), integer(2));
        expr->accept(visitor);

        REQUIRE(visitor.result() == 3);
    }

    SECTION("multiplication")
    {
        auto expr = multiply(integer(2), integer(3));
        expr->accept(visitor);

        REQUIRE(visitor.result() == 6);
    }

    SECTION("composite expression")
    {
        auto expr = add(integer(3), multiply(integer(2), integer(5)));

        expr->accept(visitor);

        REQUIRE(visitor.result() == 13);
    }
}

TEST_CASE("printing visitor")
{
    // TODO - uncomment tests

    PrintingVisitor visitor;

    SECTION("integer")
    {
        auto expr = integer(4);
        expr->accept(visitor);

        REQUIRE(visitor.str() == "4");
    }

    SECTION("addition")
    {
        auto expr = add(integer(1), integer(2));
        expr->accept(visitor);

        REQUIRE(visitor.str() == "(1 + 2)");
    }

    SECTION("multiplication")
    {
        auto expr = multiply(integer(2), integer(3));
        expr->accept(visitor);

        REQUIRE(visitor.str() == "(2 * 3)");
    }

    SECTION("composite expression")
    {
        auto expr = add(integer(3), multiply(integer(2), integer(5)));

        expr->accept(visitor);

        REQUIRE(visitor.str() == "(3 + (2 * 5))");
    }
}


struct Rect
{
    int w, h;

    void draw() const
    {
        std::cout << "Drawing rectangle: " << w << "x" << h << '\n';
    }

    double area() const
    {
        return w * h;
    }
};

struct Circle
{
    int r;

    void draw() const
    {
        std::cout << "Drawing circle: " << r << '\n';
    }

    double area() const
    {
        return 3.14 * r * r;
    }
};

struct Square
{
    int s;

    void draw() const
    {
        std::cout << "Drawing square: " << s << '\n';
    }

    double area() const
    {
        return s * s;
    }
};

struct AreaVisitor
{
    double operator()(const Rect& r) const
    {
        return r.w * r.h;
    }

    double operator()(const Circle& c) const
    {
        return 3.14 * c.r * c.r;
    }

    double operator()(const Square& s) const
    {
        return s.s * s.s;
    }
};

struct Shape
{
    using TShape = std::variant<Rect, Circle, Square>;
    TShape shape;

    template <typename T>
    Shape(T&& s) : shape{std::forward<T>(s)} {}

    template <typename T>
    Shape& operator=(T&& s)
    {
        shape = std::forward<T>(s);
        return *this;
    }

    void draw() const
    {
        std::visit([](const auto& s) { s.draw(); }, shape);
    }

    double area() const
    {
        return std::visit(AreaVisitor{}, shape);
    }
};

TEST_CASE("visiting variants")
{
    Shape shape = Rect{10, 20};
    shape.draw();
    REQUIRE(shape.area() == 200);

    shape = Circle{5};
    shape.draw();

    std::vector<Shape> shapes = {Rect{10, 20}, Circle{5}, Square{10}};
    for (const auto& s : shapes)
    {
        s.draw();
        std::cout << "Area: " << s.area() << '\n';
    }
}
