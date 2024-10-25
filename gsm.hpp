#ifndef GSM_HPP // include guard
#define GSM_HPP

#include "expr.hpp"
#include <functional>


class State {
  public:
    int id;

    void enter();
    void execute();
    void exit();
};

class StatePeriodic: public State {
  public:
    int period_ms;
};

class StateContinues: public State {
  public:
    // Not sure but should be that a periodic state always finnish but continues we need to stop it. Is that not exit
};

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
    void print() {
      for (std::size_t i = 0; i < NUM_STATES; ++i) {
        for (std::size_t j = 0; j < NUM_STATES; ++j) {
          std::cout << "state_table[" << i << "][" << j << "] = ";
          state_transition_table[i][j].print();  // Print the expression
          std::cout << " (evaluates to " << state_transition_table[i][j].evaluate() << ")  ";
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