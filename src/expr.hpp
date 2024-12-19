#ifndef EXPR_HPP // include guard
#define EXPR_HPP

#include <array>
#include <functional>
#include <iostream>

struct BoolExpr {
    std::function<bool()> expr;   // Lambda expression that returns a bool
    std::string description;      // String description of the lambda expression

    // Constructor to initialize both the lambda and the string
    BoolExpr(std::function<bool()> f, std::string desc) : expr(f), description(desc) {}

    // Function to evaluate the lambda
    bool evaluate() const {
        return expr();
    }

    // Function to print the string representation of the lambda
    void print() const {
        std::cout << description;
    }

    // Implicit conversion to std::function<bool()>
    operator std::function<bool()>() const {
        return expr;
    }
};



#endif /* EXPR_HPP */