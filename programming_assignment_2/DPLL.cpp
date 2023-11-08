#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

const char* DIMACS_FLAG = " -DIMACS";
bool UNIT_CLAUSE = false;

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
    for (auto it = symbols.begin(); it != symbols.end(); it++) {
        cout << it->first << " : " << it-> second << endl;
    }
}

int evaluateClause(vector<string> clause, unordered_map<string, int> model) {
    bool unknown = false;
    bool curr_truth = false;
    for (string symbol : clause) {
        // Find if the symbol should be negated
        bool negate = (symbol[0] == '-');
        if (negate) {
            symbol.erase(0, 1);
        }
        if (model[symbol] == -1) {
            if (negate) {
                return 1;
            }
        } else if (model[symbol] == 1) {
            if (!negate) {
                return 1;
            }
        } else { // there is an unknown symbol in the clause, we are not done
            unknown = true;
        }
    }
    if (unknown) { return 0; }
    return -1;
}


int getSymbolValue(const string& symbol, const unordered_map<string, int>& model) {
    auto it = model.find(symbol);
    if (it != model.end()) {
        return it->second;
    }
    return 0; // Default to unknown if not found in the model.
}

pair<string, int> findPureSymbol(const vector<vector<string>>& clauses, const vector<string>& symbols, const unordered_map<string, int>& model) {
    for (const string& symbol : symbols) {
        int symbolValue = getSymbolValue(symbol, model);

        if (symbolValue == 0) { // If the symbol's value is unknown in the model
            bool isPure = true;
            bool foundPos = false;
            bool foundNeg = false;

            for (const vector<string>& clause : clauses) {
                for (const string& literal : clause) {
                    if (literal == symbol) {
                        foundPos = true;
                        if (foundNeg) {
                            isPure = false;
                            break;
                        }
                    } else if (literal == '-' + symbol) {
                        foundNeg = true;
                        if (foundPos) {
                            isPure = false;
                            break;
                        }
                    }
                }

                if (!isPure) {
                    break;
                }
            }

            if (isPure) {
                // You can return the symbol along with its truth assignment:
                if (foundPos) {
                    return pair<string, int>(symbol, 1);
                } else {
                    return pair<string, int>(symbol, -1);
                }
                // Or you can simply return the literal as it appears in the clauses:
                // return symbol;
            }
        }
    }
    
    return pair<string, int>("", 0);;
}


bool DPLL(vector<vector<string>> clauses, vector<string> symbols, unordered_map<string, int> &model) {
    for (size_t i = 0; i < clauses.size(); i++) {
        // 1: true clause
        // 0: clause truth value unknown (all symbols are 0)
        // -1: false clause, return false
        int clause_truth =  evaluateClause(clauses[i], model);
        // if every clause in clauses is true in model then return true
        if (clause_truth == 1 && i == clauses.size() - 1) { return true; }
        // if some clause in clauses is false in model then return false
        if (clause_truth == -1) {
            //cout << "here" << endl;
            return false; 
        }
        // if there is an unknown clause, not done
        if (clause_truth == 0) { break; }
    }
    // P, value ← FIND-PURE-SYMBOL(symbols, clauses, model)
    //pair<string, bool> pure_symbol_value_pair = findPureSymbol(clauses, symbols, model);
    pair<string, int> pure_symbol = findPureSymbol(clauses, symbols, model);
    // if P is non-null then return DPLL(clauses, symbols – P, model ∪ {P=value})
    if (pure_symbol.first != "") {
        model[pure_symbol.first] = pure_symbol.second;
        for (auto it = symbols.begin(); it != symbols.end(); it++) {
            if (*it == pure_symbol.first) {
                //cout << pure_symbol.first << endl;
                symbols.erase(it);
                break;
            }
        }
        return DPLL(clauses, symbols, model);
    }
    
    if (UNIT_CLAUSE) {
        // P, value ← FIND-UNIT-CLAUSE(clauses, model)
        cout << "unit clause" << endl;
        // if P is non-null then return DPLL(clauses, symbols – P, model ∪ {P=value})
    }
    // P ← FIRST(symbols); rest ← REST(symbols)
    // return DPLL(clauses, rest, model ∪ {P=true}) or
    // DPLL(clauses, rest, model ∪ {P=false}))
    string P = symbols.back();
    symbols.pop_back();
    
    model[P] = 1;
    if (DPLL(clauses, symbols, model)) {
        return true;
    } else {
        model[P] = -1;
        return DPLL(clauses, symbols, model);
    }
    return false;
}


int main(int argc, char* argv[]) {
    // Handle all command line input
    if (argc < 2) {
        cout << "Usage: DPLL <filename> <literal>* [+UCH]" << endl;
        return 0;
    }
    const char* filename = argv[1];
    bool DIMACS = true;

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
    
    // Get a vector the unique symbols
    vector<string> symbols;
    for (auto symbol : model) {
        symbols.push_back(symbol.first);
    }

    // Perform DPLL
    if (DPLL(kb, symbols, model)) {
        cout << "Success!" << endl;
        printUniqueSymbols(model);
    } else {
        cout << "No solution possible, exiting..." << endl;
    }
    return 0;
}