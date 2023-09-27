#ifndef NODE_H
#define NODE_H
#include "State.h"
#include <cstddef>

class Node {
public:
    Node* pParent;
    size_t tree_depth;
    State* curr_state;
    int score;

    Node();
    Node(State* newState);
    std::vector<Node*> successors();
    int path_cost();
    void print_successors();
    int h(State* goal_state, int heuristic);
    void f(State* goal_state, int heuristic);
};

#endif // NODE_H
