#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <unordered_map>
#include <queue>
#include "State.h"
#include "Node.h"

using namespace std;
int MAX_ITERS = 100000;
string H_FLAG = "-H";

pair<std::vector<string>, std::vector<string>> parseFile(string filename) {
    vector<string> start_state;
    vector<string> goal_state;
    fstream f;
    f.open(filename);
    if (!f.is_open()) {
        cout << "Invalid Filename" << endl;
        return make_pair(vector<string>(), vector<string>());
    }
    string line;
    // Extract important numbers for start state
    getline(f, line);
    stringstream s(line);
    int num_stacks = 0;
    int num_blocks = 0;
    s >> num_stacks;
    s >> num_blocks;
    getline(f, line); // ignore separator
    // Extract block start;
    for (int i = 0; i < num_stacks; i++) {
        getline(f, line);
        start_state.push_back(line);
    }
    getline(f, line); // Ignore separator
    for (int i = 0; i < num_stacks; i++) {
        getline(f, line);
        goal_state.push_back(line);
    }
    return make_pair(start_state, goal_state);
}

class Compare {
    public:
        State* goal_state;
       bool operator()(Node* a, Node*b) {
           if(a->score > b->score) {
               return true;
           }
           return false;
        }
};

void printStats(int iter, int depth, int pathcost, int Qsize) {
    std::cout << "iter=" << iter << ", depth=" << depth << ", pathcost=" << pathcost << ", Qsize=" << Qsize << std::endl;
}

void printFinal(string filename, int method, int planlen, int iter, int qsize) {
    cout << "problem: " << filename << " | method: Astar with H" << method << " | planlen: " << planlen << " | iters: " << iter << " | maxq: " << qsize << endl; 
}

Node* aStarSearch(State* start_state, State* goal_state, int heuristic, string filename) {
    // Initialize necessary data structuress
    Node* node = new Node(start_state);
    node->f(goal_state, heuristic);
    int iter = 0;
    priority_queue<Node*, vector<Node*>, Compare> frontier;
    frontier.push(node);
    unordered_map<State*, Node*, State::StateHasher> reached;
    reached.insert(make_pair(start_state, node));
    // Begin search
    while (!frontier.empty()) {
        Node* node = frontier.top();
        node->f(goal_state, heuristic);
        frontier.pop();
        if (iter % 1000 == 0 && iter != 0) printStats(iter, node->tree_depth, node->path_cost(), frontier.size());
        // Check if the current node is equal to the solution
        bool done = true;
        for (int i = 0; i < node->curr_state->_state.size(); i++) {
            if (node->curr_state->_state[i] != goal_state->_state[i]) {
                done = false;
            }
        }
        if (done) { 
            cout << "Success!" << endl;
            printFinal(filename, heuristic, node->tree_depth, iter, frontier.size());
            cout << "Success state path: \n";
            node->print_successors();
            return node; 
        }
        // Generate successor nodes from the current state node
        for (Node* child : node->successors()) {
            State* s = child->curr_state;
            if (reached.find(s) == reached.end() || child->path_cost() < reached[s]->path_cost()) {
                child->f(goal_state, heuristic);
                reached[s] = child;
                frontier.push(child);
            }
        }
        iter++;
        if (iter > MAX_ITERS) {break;}
    }
    cout << "problem: " << filename << " | method: Astar with H" << heuristic << " | planlen: " << "FAILED" << " | iters: " << iter << " | maxq: " << frontier.size() << endl; 
    return nullptr;
}


int main(int argc, char* argv[]) {
    // Parse Command Line
    string filename = argv[1];
    string h;
    int heuristic = 0;
    for (int i = 2; i < argc; i++) {
        if (argv[i] == H_FLAG) {
            i++;
            heuristic = stoi(argv[i]);
        }
    }

    // Read in problem file and create objects representing the inital and goal states
    pair<vector<string>, vector<string>> start_goal_state = parseFile(filename);
    State* start_state = new State(start_goal_state.first);
    if (start_state->_state.empty()) { return 0; }
    State* goal_state = new State(start_goal_state.second);
    // Perform A* Search (using State and Node classes)
    Node* solution = aStarSearch(start_state, goal_state, heuristic, filename);
    if (solution == nullptr) {
        std::cout << "Failed! Exceeded maximum iterations" << std::endl;
    }
    return 0;
}