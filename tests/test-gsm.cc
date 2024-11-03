#include "../expr.hpp"
#include "../gsm.hpp"
#include "stub-func.hpp"

#include <array>
#include <functional>
#include <iostream>
#include <cstdint>
#include <memory>
#include <ostream>
#include <random>


namespace stateVariables {
    int x; // This will change and influence the state matrix
}

GSM<4U> basic_state_machine() {
    using namespace stateVariables;

    BoolExpr no_transition = EXPR(false);

    const std::size_t num_states = 4U;


    StatePeriodic s1 = StatePeriodic(0, s1_enter, s1_execute, s1_exit, 300);
    StatePeriodic s2 = StatePeriodic(1, s2_enter, s2_execute, s2_exit, 300);
    StatePeriodic s3 = StatePeriodic(2, s3_enter, s3_execute, s3_exit, 200);
    StatePeriodic s4 = StatePeriodic(3, s4_enter, s4_execute, s4_exit, 400);

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

void writeRandomValueToX(int steps, int sleep_ms) {
    std::random_device rd;  
    std::mt19937 gen(rd());
  
    std::uniform_int_distribution<> distr(1, 5);

    for (int i = 0; i < steps; ++i) {
        stateVariables::x = distr(gen);
        std::this_thread::sleep_for(std::chrono::microseconds(sleep_ms));
    }
}

void enter_t1() {
    GSM<4U> sm = basic_state_machine();
    sm.runSteps(20);
}

void enter_t2() {
    writeRandomValueToX(20, 100);
}

int testRunSteps() {
    std::thread t1(enter_t1);
    std::thread t2(enter_t2);

    t1.join();
    t2.join();

    return 1;
}


int main() {
    int testPassed = 0;
    int numTests = 0;
    
    testPassed += testNoTransition(); numTests++;
    testPassed += testTransition(); numTests++;
    testPassed += testRunSteps(); numTests++;


    std::cout << "\nPASSED: " << testPassed << "/" << numTests << std::endl;
    return 0;
}