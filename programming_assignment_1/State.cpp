#include "State.h"
#include "Node.h"
#include <iostream>
State::State() {
    _state = std::vector<std::string>();
}

State::State(std::vector<std::string> state) {
    for (auto stack : state) {
        this->_state.push_back(stack);
    }
}

std::vector<State*> State::successors() {
    std::vector<State*> possible_states;
    std::vector<std::string> curr_state = _state;
    possible_states.push_back(new State(curr_state));
    for (int i = 0; i < _state.size(); i++) {
        char block = curr_state[i][curr_state[i].size()-1];
        for (int j = 0; j < _state.size(); j++) {
            if (i == j|| curr_state[i].empty()) { 
                continue;
            }
            curr_state[i].pop_back();
            curr_state[j].push_back(block);
            possible_states.push_back(new State(curr_state));
            curr_state = _state;
        }
    }
    return possible_states;
}

void State::print_state() {
    for (int i = 0; i < _state.size(); i++) {
        std::cout << "Stack " << i << ": ";
            for (int j = 0; j < _state[i].size(); j++) {
                std::cout << _state[i][j];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
}

int State::h(State* goal_state, int heuristic) {
    int score = 0;

    switch(heuristic) {
        case 0: // score based on path_cost alone
            return 0;
        case 1: // Score based on how many stacks don't equal the goal_state's
            for (int i = 0; i < _state.size(); i++) {
                if (this->_state[i] != goal_state->_state[i]) {
                    score++;
                }
            }
            return score;
        case 2: // Score based on how many blocks are not in the correct position
            for (int i = 0; i < goal_state->_state.size(); i++) {
                int diff = abs(goal_state->_state[i].size() - _state[i].size());
                for (int j = 0; j < goal_state->_state[i].size(); j++) {
                    if (j >= _state[i].size()) { // Prevent out of range accessing
                        break;
                    }
                    if (goal_state->_state[i][j] != _state[i][j]) {
                        score++;
                    }
                }
                score += diff;
            }
            return score;
        case 3: // Find some measure of 'distance' of each block from the solution
            for (int i = 0; i < _state.size(); i++) {
                for (int j = 0; j < _state[i].size(); j++) {
                    int distance = 0;
                    char block = _state[i][j]; // want to find goal position of this block
                    // Check the current stack
                    int stack_num = 0;
                    int block_pos = 0;
                    for (int k = 0; k < goal_state->_state.size(); k++) {
                        for (int l = 0; l < goal_state->_state[k].size(); l++) {
                            if (goal_state->_state[k][l] == block) {
                                stack_num = k;
                                block_pos = l;
                                break;
                            }
                        }
                    }
                    // Measure the distance from i and j to stack_num, block_pos
                }
            }
    }
    
    return score;
}
