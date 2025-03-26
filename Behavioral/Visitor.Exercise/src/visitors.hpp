#ifndef VISITORS_HPP
#define VISITORS_HPP

#include "ast.hpp"

class ExprEvalVisitor : public AST::AstVisitor
{
    int result_{};

public:
    void visit(AST::AddNode& node)
    {
        ExprEvalVisitor lv, rv;
        node.left().accept(lv);
        node.right().accept(rv);
        result_ = lv.result() + rv.result();
    }

    void visit(AST::MultiplyNode& node)
    {
        ExprEvalVisitor lv, rv;
        node.left().accept(lv);
        node.right().accept(rv);
        result_ = lv.result() * rv.result();
    }

    void visit(AST::IntNode& node)
    {
        result_ = node.value();
    }

    int result() const
    {
        return result_;
    }
};


class PrintingVisitor : public AST::AstVisitor
{
    std::string result_{};
public:
    void visit(AST::IntNode& node)
    {
        result_ = std::to_string(node.value());
    }

    void visit(AST::AddNode& node)
    {
        PrintingVisitor lv, rv;
        node.left().accept(lv);
        node.right().accept(rv);
        result_ = "(" + lv.str() + " + " + rv.str() + ")";
    }

    void visit(AST::MultiplyNode& node)
    {
        PrintingVisitor lv, rv;
        node.left().accept(lv);
        node.right().accept(rv);
        result_ = "(" + lv.str() + " * " + rv.str() + ")";
    }

    std::string str() const
    {
        return result_;
    }
};

#endif // VISITORS_HPP