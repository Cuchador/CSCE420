#ifndef NODE_H
#define NODE_H
#include "State.h"
#include <cstddef>

class Node {
private:
    State* pParent;
    size_t tree_depth;
public:
    Node();
};

#endif // NODE_H
