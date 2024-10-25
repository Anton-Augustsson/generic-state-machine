#include "expr.hpp"
#include "gsm.hpp"

#include <array>
#include <functional>
#include <iostream>



int main() {
    int x; // This will change and influence the state matrix
    BoolExpr no_transition = EXPR(false);

    const std::size_t num_states = 4U;
    
    // 2D array of BoolExpr
    StateTable_t<num_states> state_table = {{
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