#include "State.h"
#include "Node.h"
#include <iostream>
#include <unordered_map>
#include <utility>
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
                for (int j = 0; j < goal_state->_state[i].size(); j++) {
                    if (j >= _state[i].size()) { // Prevent out of range accessing
                        score++;
                    } else if (goal_state->_state[i][j] != _state[i][j]) {
                        score++;
                    }
                }
            }
            return score;
        case 3: // Find how many moves between block pos and goal pos
            for (int i = 0; i < _state.size(); i++) {
                std::string current_stack = _state[i];
                for (int j = 0; j < current_stack.size(); j++) {
                    std::string current_block (1, _state[i][j]);
                    auto block_pos = goal_state->_state[i].find(current_block);
                    if (block_pos == std::string::npos) { // block not in stack
                        // Find how many moves to get the block out of the current stack
                        for (int block_num = j; block_num < current_stack.size(); block_num++) {
                            score++;
                        }
                        score += 1;
                        // check other stacks
                        for (int stack_num = 0; stack_num < goal_state->_state.size(); stack_num++) {
                            if (stack_num == i) continue;
                            block_pos = goal_state->_state[stack_num].find(current_block);
                            if (block_pos != std::string::npos) {
                                // block in this stack number
                                score += abs(block_pos - _state[stack_num].size());
                            }
                        
                        }
                    }
                }
            }
            return score;
    }
    
    return score;
}
