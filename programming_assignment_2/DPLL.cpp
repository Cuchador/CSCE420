#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

const char* DIMACS_FLAG = " -DIMACS";
const char* UCH_FLAG = "+UCH";
bool UNIT_CLAUSE = false;
bool PURE_SYMBOL = false;
int num_calls = 0;
bool ONLY_FINAL_PRINT = false;

using namespace std;

int populateKB(string cnf_filename, vector<vector<string>> &kb, unordered_map<string, int> &symbols) {
    fstream file;
    file.open(cnf_filename);
    // Check if the file is open
    if (!file.is_open()) {
        return -1;
    }

    // Read the file line by line
    string line;
    while (getline(file, line)) {
        istringstream symbolStream(line);
        vector<string> clause;
        string symbol;
        while (symbolStream >> symbol) {
            if (!symbol.empty()) {
                clause.push_back(symbol);
                if (symbol[0] == '-') { symbol.erase(0, 1); }
                symbols[symbol] = 0;
            }
        }
        kb.push_back(clause);
    }
    file.close();
    return 0;
}

void removeFactsFromSymbols(vector<string> &symbols, vector<string> facts) {
    for (string fact : facts) {
        for (int i = 0; i < symbols.size(); i++) {
            fact = (fact[0] == '-') ? fact.substr(1) : fact;
            if (symbols[i] == fact) {
                symbols.erase(symbols.begin()+i);
            }
        }
    }
}

void printCurrentModel(unordered_map<string, int> symbols) {
    std::cout << "Model = {";
    for (auto it = symbols.begin(); it != symbols.end(); it++) {
        std::cout << it->first << " : " << it-> second << ", ";
    }
    std::cout << "}" << std::endl;
}

void finalPrint(unordered_map<string, int> model) {
    cout << "Satisfied (true) propositions:" << endl;
    for (auto it = model.begin(); it != model.end(); it++) {
        if (it->second == 1) {
            cout << it->first << " ";
        }
    }
    cout << endl;
    cout << "Total number of DPLL calls: " << num_calls << endl;
    string uch = UNIT_CLAUSE ? "UCH=True" : "UCH=False";
    cout << uch << endl;
}

// Function to check if a clause is satisfied in the current model
bool isClauseSatisfied(const vector<string>& clause, const unordered_map<string, int>& model) {
    for (const string& symbol : clause) {
        if (symbol[0] == '-') {
            if (model.at(symbol.substr(1)) == -1) {
                return true;  // The clause is true in the current model
            }
        } else {
            if (model.at(symbol) == 1) {
                return true;  // The clause is true in the current model
            }
        }
    }
    return false;  // The clause is not satisfied in the current model
}

bool checkClauseUnassigned(const vector<string>& clause, const unordered_map<string, int>& model) {
    for (const string& symbol : clause) {
        if (symbol[0] == '-') {
            if (model.at(symbol.substr(1)) == 0) {
                return true;  // The clause is true in the current model
            }
        } else {
            if (model.at(symbol) == 0) {
                return true;  // The clause is true in the current model
            }
        }
    }
    return false;
}

pair<string, int> findUnitClause(const vector<vector<string>>& clauses, const unordered_map<string, int>& model) {
    for (const vector<string>& clause : clauses) {
        int unassignedCount = 0;
        string unassignedSymbol = "";
        bool negate = false;
        for (const string& symbol : clause) {
            negate = (symbol[0] == '-');
            int value = negate ? model.at(symbol.substr(1)) : model.at(symbol);
            if (value == 0) {
                unassignedCount++;
                unassignedSymbol = symbol;
            } else if (value == 1) {
                unassignedCount = -1;  // The clause is already satisfied
                break;
            }
        }
        if (unassignedCount == 1) {
            return make_pair(negate ? unassignedSymbol.substr(1) : unassignedSymbol, negate ? -1 : 1);
        }
    }
    return make_pair("", 0);  // No unit clause found
}



bool DPLL(vector<vector<string>> clauses, vector<string> symbols, unordered_map<string, int> model) {
    num_calls++;
    printCurrentModel(model);
    // Check if every clause is true in the current model
    bool all_clauses_satisfied = true;
    bool unassigned_variables = false;
    for (const string& symbol : symbols) {
        if (model[symbol] == 0) {
            unassigned_variables = true;
        }
    }
    for (const vector<string>& clause : clauses) {
        if (!isClauseSatisfied(clause, model)) {
            all_clauses_satisfied = false;
            break;
        }
    }
    if (all_clauses_satisfied) {
        std::cout << endl << "### DPLL SUCCESSFUL ###" << endl;
        if (!ONLY_FINAL_PRINT) {
            std::cout << "Total number of DPLL calls: " << num_calls << endl;
            std::cout << "Final model: " << endl;
            printCurrentModel(model);
        }
        cout << endl;
        finalPrint(model);
        return true;
    } 
    // Check if some clause is false in the current model
    for (const vector<string>& clause : clauses) {
        if (isClauseSatisfied(clause, model)) {
            continue;
        } else {
            if (checkClauseUnassigned(clause, model)) {
                continue;
            }
        }
        if (!ONLY_FINAL_PRINT) { cout << "backtracking" << endl; }
        return false;
    }    
    
    if (UNIT_CLAUSE) {
        // P, value ← FIND-UNIT-CLAUSE(clauses, model)
        // if P is non-null then return DPLL(clauses, symbols – P, model ∪ {P=value})
        pair<string, int> unit_clause = findUnitClause(clauses, model);
        if (!unit_clause.first.empty()) {
            cout << unit_clause.first << endl;
            return true;
            string assignment = (unit_clause.second == 1) ? "true" : "false";
            if (!ONLY_FINAL_PRINT) { cout << endl <<"forcing " << unit_clause.first << "=" <<  assignment << endl; }

            vector<string> rest(symbols);
            for (size_t i = 0; i < rest.size(); i++) {
                if (rest[i] == unit_clause.first) {
                    rest.erase(rest.begin()+i);
                }
            }
            model[unit_clause.first] = unit_clause.second;
            return DPLL(clauses, rest, model);
        }

        //if (symbols.empty()) {
        //    return false;  // All symbols assigned, but not all clauses satisfied
        //}
    }
    // P ← FIRST(symbols); rest ← REST(symbols)
    string P = symbols[0];
    vector<string> rest(symbols.begin()+1, symbols.end());
    
    // return DPLL(clauses, rest, model ∪ {P=true}) or
    // DPLL(clauses, rest, model ∪ {P=false}))
    model[P] = 1;
    if (!ONLY_FINAL_PRINT) {
        std::cout << endl;
        std::cout << "trying " << P << "=" << "T" << endl; 
    }
    if (DPLL(clauses, rest, model)) {
        return true;
    } else {
        model[P] = -1;
        if (!ONLY_FINAL_PRINT) {
            std::cout << endl;
            std::cout << "trying " << P << "=" << "F" << endl;
        }
        return DPLL(clauses, rest, model);
    }
}


int main(int argc, char* argv[]) {
    // Handle all command line input
    if (argc < 2) {
        std::cout << "Usage: DPLL <filename> <literal>* [+UCH]" << endl;
        std::cout << "*literal is optional" << std::endl;
        return 0;
    }
    const char* filename = argv[1];
    bool DIMACS = true;
    int arg_num = 2;
    std::vector<std::string> facts;
    while (arg_num < argc) {
        if (strcmp(argv[arg_num], UCH_FLAG) == 0) {
            UNIT_CLAUSE = true;
        } else {
            facts.push_back(argv[arg_num]);
        }
        arg_num++;
    }
    
    // Populate KB using the CNF format, including new facts
    vector<vector<string>> kb; // 1 = true, -1 = false, 0 = unknown 
    unordered_map<string, int> model;
    if (populateKB(filename, kb, model) < 0) {
        cerr << "Failed to open the CNF file." << endl;
        return 0;
    }
    
    // Get a vector the unique symbols, excluding known facts
    vector<string> symbols;
    for (auto symbol : model) {
        symbols.push_back(symbol.first);
    }
    

    for (std::string fact : facts) {
        if (fact[0] == '-') {
            model[fact.substr(1)] = -1;
        } else {
            model[fact] = 1;
        }
    }
    removeFactsFromSymbols(symbols, facts);
    
    // Perform DPLL
    if (!DPLL(kb, symbols, model)) {
        std::cout << "No solution possible (problem is unsatisfiable)" << endl;
        std::cout << "Total number of DPLL calls: " << num_calls << endl;
    }
    
    return 0;
}