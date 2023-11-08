#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

const char* DIMACS_FLAG = " -DIMACS";
bool UNIT_CLAUSE = false;
bool PURE_SYMBOL = false;

using namespace std;

int convertFileToCNF(const char* filename) {
    // Construct the Python command string with the dynamic filename
    string python_script = "python3 convCNF.py ";
    python_script += filename;
    python_script += DIMACS_FLAG;

    // Call the Python script
    int result = system(python_script.c_str());

    if (result != 0) {
        return -1;
    }

    return 0;
}

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

void printUniqueSymbols(unordered_map<string, int> symbols) {
    std::cout << "Model = {";
    for (auto it = symbols.begin(); it != symbols.end(); it++) {
        cout << it->first << " : " << it-> second << ", ";
    }
    std::cout << "}" << std::endl;
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

bool DPLL(vector<vector<string>> clauses, vector<string> symbols, unordered_map<string, int> model) {
    std::cout << "Trying..." << std::endl;
    printUniqueSymbols(model);
    std::cout << std::endl;
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
        return false;
    }    
    
    if (UNIT_CLAUSE) {
        // P, value ← FIND-UNIT-CLAUSE(clauses, model)
        cout << "unit clause" << endl;
        // if P is non-null then return DPLL(clauses, symbols – P, model ∪ {P=value})
    }
    // P ← FIRST(symbols); rest ← REST(symbols)
    string P = symbols[0];
    vector<string> rest(symbols.begin()+1, symbols.end());
    
    // return DPLL(clauses, rest, model ∪ {P=true}) or
    // DPLL(clauses, rest, model ∪ {P=false}))
    model[P] = 1;
    if (DPLL(clauses, rest, model)) {
        return true;
    } else {
        model[P] = -1;
        return DPLL(clauses, rest, model);
    }
}


int main(int argc, char* argv[]) {
    // Handle all command line input
    if (argc < 2) {
        cout << "Usage: DPLL <filename> <literal>* [+UCH]" << endl;
        std::cout << "*literal is optional" << std::endl;
        return 0;
    }
    const char* filename = argv[1];
    bool DIMACS = true;
    bool UCH = false;
    int arg_num = 2;
    std::vector<std::string> facts;
    while (arg_num < argc) {
        if (argv[arg_num] == "-UCH") {
            UCH = true;
        } else {
            facts.push_back(argv[arg_num]);
        }
        arg_num++;
    }

    // Read in original file, convert to CNF, and store in new file
    if (convertFileToCNF(filename) < 0) {
        cerr << "Error converting file KB to CNF." << endl;
        return 0;
    }

    // Convert filename to new cnf file
    string cnf_filename = string(filename);
    cnf_filename.replace(cnf_filename.find(".kb"), 3, "CNF.kb");

    // Populate KB using the CNF format
    vector<vector<string>> kb; // 1 = true, -1 = false, 0 = unknown 
    unordered_map<string, int> model;
    if (populateKB(cnf_filename, kb, model) < 0) {
        cerr << "Failed to open the CNF file." << endl;
        return 0;
    }
    
    // Get a vector the unique symbols, excluding known facts
    vector<string> symbols;
    for (auto symbol : model) {
        if (!facts.empty()) {
            for (std::string fact : facts) {
            if (fact[0] == '-') {
                if (fact.substr(1) != symbol.first) {
                    symbols.push_back(symbol.first);
                }
            } else {
                if (fact != symbol.first) {
                    symbols.push_back(symbol.first);
                }
            }
            }
        } else {
            symbols.push_back(symbol.first);
        }
    }

    for (std::string fact : facts) {
        if (fact[0] == '-') {
            model[fact.substr(1)] = -1;
        } else {
            model[fact] = 1;
        }
    }
    
    // Perform DPLL
    if (DPLL(kb, symbols, model)) {
        cout << "Success!" << endl;
        //printUniqueSymbols(model);
    } else {
        cout << "No solution possible, exiting..." << endl;
    }
    
    return 0;
}