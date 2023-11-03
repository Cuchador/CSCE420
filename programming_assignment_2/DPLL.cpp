#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>

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

int populateKB(std::string cnf_filename, std::unordered_map<std::string, int> &kb) {
    std::fstream file;
    file.open(cnf_filename);
    // Check if the file is open
    if (!file.is_open()) {
        return -1;
    }

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        kb[line] = 0; //initial value unknown
        // std::cout << line << std::endl;
    }
    file.close();
    return 0;
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
    std::unordered_map<std::string, int> kb; // 1 = true, -1 = false, 0 = unknown 
    if (populateKB(cnf_filename, kb) < 0) {
        std::cerr << "Failed to open the CNF file." << std::endl;
        return 0;
    }

    return 0;
}