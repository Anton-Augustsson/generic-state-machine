#ifndef GSM_HPP // include guard
#define GSM_HPP

#include "expr.hpp"
#include <functional>
#include <cstdint>
#include <thread>
#include <chrono>

using func_t = std::function<void()>;

class State {
  public:
    State(uint8_t id_param, func_t enter_func_param, func_t execute_func_param, func_t exit_func_param, uint32_t period_ms_param) 
      : id(id_param), enter_func(enter_func_param), execute_func(execute_func_param), exit_func(exit_func_param), period_ms(period_ms_param) {}


    void enter() const { enter_func(); };
    void execute() const { execute_func(); };
    void exit() const { exit_func(); };

    void sleep() { 
      std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
    };

  private:
    uint8_t id; // This should be generated so no one can introduce error
    uint32_t period_ms;

    func_t enter_func;
    func_t execute_func;
    func_t exit_func;
};

/* TODO: add later
class StatePeriodic: public State {
  public:
    int period_ms;
};

class StateContinues: public State {
  public:
    // Not sure but should be that a periodic state always finnish but continues we need to stop it. Is that not exit
};
*/

template <std::size_t NUM_STATES>
using StateTable_t = std::array<std::array<BoolExpr, NUM_STATES>, NUM_STATES>;

template <std::size_t NUM_STATES>
using States_t = std::array<State, NUM_STATES>;


template <std::size_t NUM_STATES>
class GSM {
  public:
    GSM(uint8_t start_state_param, StateTable_t<NUM_STATES> stt, States_t<NUM_STATES> s) 
      : curent_state(start_state_param), state_transition_table(stt), states(s) {
        states[curent_state].enter();
        states[curent_state].execute();
      }

    // TODO: this evaluates so... should be mentioned in the name?
    void print_state_transition_stable() {
      std::cout << "\t";
      for (std::size_t i = 0; i < NUM_STATES; ++i) {
        std::cout << "s" << i+1 << "\t\t";
      }
      std::cout << std::endl;

      for (std::size_t i = 0; i < NUM_STATES; ++i) {
        std::cout << "s" << i+1 << "\t";

        for (std::size_t j = 0; j < NUM_STATES; ++j) {
          state_transition_table[i][j].print();
          std::cout << "\t\t";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }

    void print_evaluates() {
      std::cout << "\t";
      for (std::size_t i = 0; i < NUM_STATES; ++i) {
        std::cout << "s" << i+1 << "\t\t";
      }
      std::cout << std::endl;

      for (std::size_t i = 0; i < NUM_STATES; ++i) {
        std::cout << "s" << i+1 << "\t";

        for (std::size_t j = 0; j < NUM_STATES; ++j) {
          std::cout << state_transition_table[i][j].evaluate();
          std::cout << "\t\t";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }

    void step() {
      for (uint8_t i = 0; i < NUM_STATES; ++i ) {
        if (state_transition_table[curent_state][i].evaluate()) {
          states[curent_state].exit();
          curent_state = i;
          states[curent_state].enter();
          break;
        }
      }

      states[curent_state].execute();
    }

    void run() {
      // TODO: run for ever or until terminated
      for (;;) {
        step();
        states[curent_state].sleep();
      }
    }

    void runSteps(uint32_t steps) {
      for (uint32_t i = 0; i < steps; ++i) {
        step();
        states[curent_state].sleep();
      }
    }

  private:
    uint8_t curent_state;
    StateTable_t<NUM_STATES> state_transition_table;
    States_t<NUM_STATES> states;
};


#define EXPR(e) BoolExpr([&]() { return (e); }, #e)

#endif /* GSM_HPP */