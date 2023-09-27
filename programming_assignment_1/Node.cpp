#include "Node.h"
#include <iostream>

Node::Node() {
    pParent = nullptr;
    tree_depth = 0;
    curr_state = nullptr;
    score = 0;
}

Node::Node(State* newState) {
    pParent = nullptr;
    tree_depth = 0;
    curr_state = newState;
    score = 0;
}

int Node::path_cost() {
    return tree_depth;
}

std::vector<Node*> Node::successors() {
    std::vector<Node*> succ_nodes;
    for (State* succ_state : curr_state->successors()) {
        Node* new_node = new Node(succ_state);
        new_node->pParent = this;
        new_node->tree_depth = this->tree_depth+1;
        succ_nodes.push_back(new_node);
    }
    return succ_nodes;
}

void Node::print_successors() {
    std::vector<State*> states;
    states.push_back(this->curr_state);
    Node* print_node = this->pParent;
    while (print_node != nullptr) {
        states.push_back(print_node->curr_state);
        print_node = print_node->pParent;
    }

    int move = 0;
    for (int i = states.size()-1; i >= 0; i--) {
        std::cout << "***Move " << move << "***" << std::endl;
        states[i]->print_state();
        move++;
    }
}

int Node::h(State* goal_state, int heuristic) {
    return this->curr_state->h(goal_state, heuristic);
}

void Node::f(State* goal_state, int heuristic) {
    score = h(goal_state, heuristic) + path_cost();
}