#ifndef GSM_HPP // include guard
#define GSM_HPP

#include "expr.hpp"
#include <exception>
#include <functional>
#include <cstdint>
#include <thread>
#include <chrono>

using namespace expr;

namespace gsm {
using func_t = std::function<void()>;


class State {
  public:
    virtual ~State() = default;

    void enter() const { if (enter_func) enter_func(); }
    void execute() const { if (execute_func) execute_func(); }
    void exit() const { if (exit_func) exit_func(); }
    void sleep() const { 
      std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));
    }

  protected:
    State(const uint8_t id_param, func_t enter_func_param, func_t execute_func_param, func_t exit_func_param, uint32_t period_ms_param)
        : id(id_param), enter_func(enter_func_param), execute_func(execute_func_param), exit_func(exit_func_param), period_ms(period_ms_param) {}

    static uint8_t generate_id() {
        static uint8_t current_id = 0;
        return current_id++;
    }
  
    const uint8_t id;
    uint32_t period_ms;

    func_t enter_func;
    func_t execute_func;
    func_t exit_func;
};

class StatePeriodic : public State {
  public:
    StatePeriodic(const uint8_t id_param, func_t enter_func_param, func_t execute_func_param, func_t exit_func_param, uint32_t period_ms_param) 
      : State(id_param, enter_func_param, execute_func_param, exit_func_param, period_ms_param) {
    }
};

class StateContinues : public State {
  public:
    StateContinues(const uint8_t id_param, func_t enter_func_param, func_t execute_func_param, func_t exit_func_param, uint32_t period_ms_param) 
      : State(id_param, enter_func_param, execute_func_param, exit_func_param, period_ms_param) {
    }
};


template <std::size_t NUM_STATES>
using StateTable_t = std::array<std::array<BoolExpr, NUM_STATES>, NUM_STATES>;

template <std::size_t NUM_STATES>
using States_t = std::array<State, NUM_STATES>;



template <std::size_t NUM_STATES>
class GSM {
  public:
    GSM(uint8_t start_state_param, StateTable_t<NUM_STATES> state_transition_table_param, States_t<NUM_STATES> states_param) 
      : curent_state(start_state_param), state_transition_table(state_transition_table_param), states(states_param) {
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
}

#endif /* GSM_HPP */
