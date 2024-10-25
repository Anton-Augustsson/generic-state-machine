#include <array>
#include <functional>
#include <iostream>

#define EXPR(e) BoolExpr([&]() { return (e); }, #e)

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

int main() {
    BoolExpr no_transition = EXPR(false);

    int x; // This will change and influence the state matrix

    const std::size_t num_states = 4U;
    
    // 2D array of BoolExpr
    std::array<std::array<BoolExpr, num_states>, num_states> state_table = {{
        {EXPR(x > 1), EXPR(x < 2), EXPR(x == 2), EXPR(x != 2)},
        {EXPR(x > 2), EXPR(x < 4), EXPR(x == 3), EXPR(x != 3)},
        {no_transition, EXPR(x < 8), EXPR(x == 4), EXPR(x != 4)},
        {EXPR(x > 4), EXPR(x < 16), EXPR(x == 5), EXPR(x != 5)}
    }};

    // Function to print the matrix with its description and evaluated result
    auto print_matrix = [&]() {
        std::cout << "Matrix values with x = " << x << ":\n";
        for (std::size_t i = 0; i < num_states; ++i) {
            for (std::size_t j = 0; j < num_states; ++j) {
                std::cout << "state_table[" << i << "][" << j << "] = ";
                state_table[i][j].print();  // Print the expression
                std::cout << " (evaluates to " << state_table[i][j].evaluate() << ")  ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    };

    // Change x and print the matrix
    x = 3;
    print_matrix();
   
    x = 4;
    print_matrix();

    return 0;
}