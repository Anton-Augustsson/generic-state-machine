#ifndef GSM_HPP // include guard
#define GSM_HPP

#include "expr.hpp"
#include <functional>

using func_t = std::function<void()>;

class State {
  public:
    State(func_t enter_func_param, func_t execute_func_param, func_t exit_func_param) 
      : enter_func(enter_func_param), execute_func(enter_func_param), exit_func(enter_func_param) {}

    void enter() const { enter_func(); };
    void execute() const { execute_func(); };
    void exit() const { exit_func(); };

  private:
    int id;
    int period_ms;

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
    GSM(StateTable_t<NUM_STATES> stt, States_t<NUM_STATES> s) 
      : state_transition_table(stt), states(s) {}

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

  private:
    StateTable_t<NUM_STATES> state_transition_table;
    States_t<NUM_STATES> states;
};


#define EXPR(e) BoolExpr([&]() { return (e); }, #e)

#endif /* GSM_HPP */