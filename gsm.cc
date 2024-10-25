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

class ConditionData {
  public: 
    int data_id; // Or just the function to get the data, but what is the type??

    void callback(); // When we have got the data
};


class Condition {
  public:
    const std::function<bool(ConditionData*)> &func_expresion;
};

class Transition {
  public:
    Condition* conditions;

};

class GSM {
  public:
    State* states;
    Transition** state_table; // FIXME: should be state on either access and then transition on each element.
};

// Thread 1: grpc server -> populate queue
// Thread 2: state, reads from queue, depending on the state it should be able to self terminate or it should handle that event.
// If it self terminates then what state will it go to? How does that relate to the transition conditions?