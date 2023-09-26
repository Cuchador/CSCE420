#ifndef STATE_H
#define STATE_H
#include <string>
#include <vector>

class State {
public:
    std::vector<std::string> _state;
    State();
    State(std::vector<std::string> state);
    std::vector<State*> successors();
    void print_state();
    int h(State* goal_state, int heuristic);

    struct StateHasher {
        std::size_t operator()(const State* s) const {
            std::size_t hash = 0;
            for (const std::string& str : s->_state) {
                hash ^= std::hash<std::string>{}(str);
            }
            return hash;
        }
    };

    bool operator==(const State& other) const {
        if (other._state.size() != this->_state.size()) {
            return false;
        }
        for (int i = 0; i < other._state.size(); i++) {
            if (other._state[i] != this->_state[i]) {
                return false;
            }
        }

        return true;
    }
}; 

#endif // STATE_H