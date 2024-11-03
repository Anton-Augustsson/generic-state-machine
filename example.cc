#include "expr.hpp"
#include "gsm.hpp"

#include <array>
#include <functional>
#include <iostream>
#include <cstdint>


void s1_enter() { printf("enter state 1\n"); }
void s1_execute() { printf("executing state 1\n"); }
void s1_exit() { printf("exit state 1\n"); }

void s2_enter() { printf("enter state 2\n"); }
void s2_execute() { printf("executing state 2\n"); }
void s2_exit() { printf("exit state 2\n"); }

void s3_enter() { printf("enter state 3\n"); }
void s3_execute() { printf("executing state 3\n"); }
void s3_exit() { printf("exit state 3\n"); }

void s4_enter() { printf("enter state 4\n"); }
void s4_execute() { printf("executing state 4\n"); }
void s4_exit() { printf("exit state 4\n"); }


int main() {
    int x; // This will change and influence the state matrix
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

    GSM state_machine(start_state, state_transition_table, states);
    
    state_machine.print_state_transition_stable();

    // Change x and print the matrix
    x = 1;
    state_machine.print_evaluates();
    state_machine.step();
   
    x = 2;
    state_machine.print_evaluates();
    state_machine.step();

    x = 3;
    state_machine.print_evaluates();
    state_machine.step();

    x = 3;
    state_machine.print_evaluates();
    state_machine.step();


    return 0;
}