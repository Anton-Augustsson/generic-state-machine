#include "expr.hpp"
#include "gsm.hpp"

#include <array>
#include <functional>
#include <iostream>


int main() {
    int x; // This will change and influence the state matrix
    BoolExpr no_transition = EXPR(false);

    const std::size_t num_states = 4U;

    State s1, s2, s3, s4;
    States_t<num_states> states = {s1, s2, s3, s4};

    // 2D array of BoolExpr
    StateTable_t<num_states> state_transition_table = {{
        {EXPR(x > 1), EXPR(x < 2), EXPR(x == 2), EXPR(x != 2)},
        {EXPR(x > 2), EXPR(x < 4), EXPR(x == 3), EXPR(x != 3)},
        {no_transition, EXPR(x < 8), EXPR(x == 4), EXPR(x != 4)},
        {EXPR(x > 4), EXPR(x < 16), EXPR(x == 5), EXPR(x != 5)}
    }};

    GSM state_machine(state_transition_table, states);
    
    // Change x and print the matrix
    x = 3;
    state_machine.print();
   
    x = 4;
    state_machine.print();

    return 0;
}