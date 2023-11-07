#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

const char* DIMACS_FLAG = " -DIMACS";

int convertFileToCNF(const char* filename) {
    // Construct the Python command string with the dynamic filename
    std::string python_script = "python3 convCNF.py ";
    python_script += filename;
    python_script += DIMACS_FLAG;

    // Call the Python script
    int result = system(python_script.c_str());

    if (result != 0) {
        return -1;
    }

    return 0;
}

int populateKB(std::string cnf_filename, std::vector<std::vector<std::string>> &kb, std::unordered_map<std::string, int> &symbols) {
    std::fstream file;
    file.open(cnf_filename);
    // Check if the file is open
    if (!file.is_open()) {
        return -1;
    }

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream symbolStream(line);
        std::vector<std::string> clause;
        std::string symbol;
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

void printUniqueSymbols(std::unordered_map<std::string, int> symbols) {
    for (auto it = symbols.begin(); it != symbols.end(); it++) {
        std::cout << it->first << std::endl;
    }
}

int evaluateClause(std::vector<std::string> clause, std::unordered_map<std::string, int> model) {
    bool unknown = true;
    bool curr_truth = false;
    for (std::string symbol : clause) {
        // Find if the symbol should be negated
        bool negate = (symbol[0] == '-');
        if (negate) {
            symbol.erase(0, 1);
        }
        std::cout << symbol << std::endl;
        if (model[symbol] == -1) {
            if (negate) {
                return 1;
            } else {
                curr_truth = false;
                unknown = false;
            }
        } else if (model[symbol] == 1) {
            if (!negate) {
                return 1;
            } else {
                curr_truth = false;
                unknown = false;
            }
        } 
    }
    if (unknown) { return 0; }
    return -1;
}

std::pair<std::string, bool> findPureSymbol(std::vector<std::vector<std::string>> clauses, std::vector<std::string> symbols, std::unordered_map<std::string, int> model) {
    std::unordered_map<std::string, int> symbol_purity_values = model; // 3 for last symbol appearance negated, 4 for last appearance normal, 5 for not pure
    for (std::vector<std::string> clause : clauses) {
        if (evaluateClause(clause, model) == 1) { continue; } // skip clauses we know to be true
        for (std::string symbol : clause) {
            // Make sure we have not determined the symbol to be pure already
            if (!std::count(symbols.begin(), symbols.end(), symbol)) {
                continue;
            }

            // Determine sign of symbol
            bool negate = (symbol[0] == '-');
            if (negate) { symbol.erase(0, 1); }

            if (symbol_purity_values[symbol] == 5) { continue; }
            else if (symbol_purity_values[symbol] == 3 && !negate) {
                symbol_purity_values[symbol] = 5;
            } else if (symbol_purity_values[symbol] == 4 && negate) {
                symbol_purity_values[symbol] = 5;
            } else {
                if (negate) {symbol_purity_values[symbol] = 3; }
                else { symbol_purity_values[symbol] = 4; }
            }
        }
    }

    for (auto it = symbol_purity_values.begin(); it != symbol_purity_values.end(); it++) {
        if (it->second == 3) {
            return std::pair<std::string, bool>(it->first, false);
        } else if (it->second == 4) {
            return std::pair<std::string, bool>(it->first, true);
        }
    }
    return std::pair<std::string, bool>("", false);
}


bool DPLL(std::vector<std::vector<std::string>> clauses, std::vector<std::string> symbols, std::unordered_map<std::string, int> model) {
    // P, value ← FIND-UNIT-CLAUSE(clauses, model)
    // if P is non-null then return DPLL(clauses, symbols – P, model ∪ {P=value})
    // P ← FIRST(symbols); rest ← REST(symbols)
    // return DPLL(clauses, rest, model ∪ {P=true}) or
    // DPLL(clauses, rest, model ∪ {P=false}))
    for (size_t i = 0; i < clauses.size(); i++) {
        // 1: true clause
        // 0: clause truth value unknown (all symbols are 0)
        // -1: false clause, return false
        int clause_truth =  evaluateClause(clauses[i], model);
        // if every clause in clauses is true in model then return true
        if (clause_truth == 1 && i == clauses.size() - 1) { return true; }
        // if some clause in clauses is false in model then return false
        if (clause_truth == -1) { return false; }
        // if there is an unknown clause, not done
        if (clause_truth == 0) { break; }
    }
    // P, value ← FIND-PURE-SYMBOL(symbols, clauses, model)
    std::pair<std::string, bool> pure_symbol_value_pair = findPureSymbol(clauses, symbols, model);
    // if P is non-null then return DPLL(clauses, symbols – P, model ∪ {P=value})
    if (!pure_symbol_value_pair.first.empty()) {
        model[pure_symbol_value_pair.first] = pure_symbol_value_pair.second;
        for (size_t i = 0; i < symbols.size(); i++) {
            if (symbols[i] == pure_symbol_value_pair.first) {
                symbols.erase(symbols.begin() + i); // CHECK THIS
            }
        }
    }
    return false;
}


int main(int argc, char* argv[]) {
    // Handle all command line input
    if (argc < 2) {
        std::cout << "Usage: DPLL <filename> <literal>* [+UCH]" << std::endl;
        return 0;
    }
    const char* filename = argv[1];
    bool DIMACS = true;

    // Read in original file, convert to CNF, and store in new file
    if (convertFileToCNF(filename) < 0) {
        std::cerr << "Error converting file KB to CNF." << std::endl;
        return 0;
    }

    // Convert filename to new cnf file
    std::string cnf_filename = std::string(filename);
    cnf_filename.replace(cnf_filename.find(".kb"), 3, "CNF.kb");

    // Populate KB using the CNF format
    std::vector<std::vector<std::string>> kb; // 1 = true, -1 = false, 0 = unknown 
    std::unordered_map<std::string, int> model;
    if (populateKB(cnf_filename, kb, model) < 0) {
        std::cerr << "Failed to open the CNF file." << std::endl;
        return 0;
    }
    
    // Get a vector the unique symbols
    std::vector<std::string> symbols;
    for (auto symbol : model) {
        symbols.push_back(symbol.first);
    }

    // Perform DPLL
    if (DPLL(kb, symbols, model)) {
        std::cout << "Success!" << std::endl;
    } else {
        std::cout << "DPLL Failed. Exiting..." << std::endl;
    }
    return 0;
}