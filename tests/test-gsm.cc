#include "../expr.hpp"
#include "../gsm.hpp"
#include "stub-func.hpp"

#include <array>
#include <functional>
#include <iostream>
#include <cstdint>
#include <ostream>


namespace stateVariables {
    int x; // This will change and influence the state matrix
}

GSM<4U> basic_state_machine() {
    using namespace stateVariables;

    BoolExpr no_transition = EXPR(false);

    const std::size_t num_states = 4U;


    State s1(0, s1_enter, s1_execute, s1_exit);
    State s2(1, s2_enter, s2_execute, s2_exit);
    State s3(2, s3_enter, s3_execute, s3_exit);
    State s4(3, s4_enter, s4_execute, s4_exit);

    States_t<num_states> states = {s1, s2, s3, s4};
    uint8_t start_state = 0; // TODO: easy error by one s0 is that a thing

    // 2D array of BoolExpr
    StateTable_t<num_states> state_transition_table = {{
        {no_transition, EXPR(x == 2),  EXPR(x == 3),  EXPR(x >= 4)},
        {EXPR(x == 1),  no_transition, EXPR(x == 3),  EXPR(x >= 4)},
        {EXPR(x == 1),  EXPR(x == 2),  no_transition, EXPR(x >= 4)},
        {EXPR(x == 1),  EXPR(x == 2),  EXPR(x == 3),  EXPR(x >= 4)}
    }};

    GSM<num_states> state_machine(start_state, state_transition_table, states);

    return state_machine;
}

int testNoTransition() {
    GSM<4U> sm = basic_state_machine();

    sm.print_state_transition_stable();

    stateVariables::x = 1;
    sm.print_evaluates();
    sm.step();
    sm.print_evaluates();
 
    return 1;
}

int testTransition() {
    GSM<4U> sm = basic_state_machine();

    sm.print_state_transition_stable();

    stateVariables::x = 1;
    sm.print_evaluates();
    sm.step();

    stateVariables::x = 2;
    sm.print_evaluates();
    sm.step();
     
    return 1;
}

int main() {
    int testPassed = 0;
    int numTests = 0;
    
    testPassed += testNoTransition(); numTests++;
    testPassed += testTransition(); numTests++;

    std::cout << "\nPASSED: " << testPassed << "/" << numTests << std::endl;
    return 0;
}