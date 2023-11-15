import sys
DIMACS_FLAG = "-DIMACS"
UCH_FLAG = "+UCH"
UCH = False
global NUM_CALLS
NUM_CALLS = 0

def populateKB(filename):
    kb = []
    model = {}
    with open(filename) as f:
        for line in f.readlines():
            clause = []
            literal = ""
            pos = -1
            for ch in line:
                pos += 1
                if (ch == " " or pos == len(line)-1): 
                    clause.append(literal)
                    if (literal[0] == '-'):
                        literal = literal[1:]
                    if (literal not in model): 
                        model[literal] = 0
                    literal = ""
                    continue
                literal += ch
            kb.append(clause)
    return kb, model

def finalSuccessPrint(model):
    print(model)
    print()
    print("### DPLL SUCCESSFUL ###")
    print("Number of DPLL Calls:", NUM_CALLS)
    printTrueSymbols(model)
    
    
def findUnitClause(clauses, model):
    for clause in clauses:
        unassigned_count = 0
        uc_symbol = ""
        uc_value = 0
        for literal in clause:
            symbol = literal if literal[0] != '-' else literal[1:] 
            value = 1 if literal[0] != '-' else -1
            if (model[symbol] == 0):
                unassigned_count += 1
                uc_symbol = symbol
                uc_value = value
            elif model[symbol] == value:
                unassigned_count = -1
                break
        if (unassigned_count == 1):
            return uc_symbol, uc_value
    return "", 0

                
def printTrueSymbols(model):
    print("Satisfied (true) propositions:")
    for symbol in model:
        if (model[symbol] == 1):
            print(symbol, end=" ")
    print()

def DPLL(clauses, symbols, model):
    #clauses is a 2D array, where the inner array holds literals in the clause
    #symbols is an array of all unique symbols
    #model is the assignment (1, 0, or -1) of all symbols
    
    #check if all clauses are true
    global NUM_CALLS
    NUM_CALLS += 1
    print(model)
    all_clauses_satisfied = True
    for clause in clauses:
        clause_truth = False
        for literal in clause:
            symbol = literal if literal[0] != '-' else literal[1:]  
            value = 1 if literal[0] != '-' else -1
            if (model[symbol] == value):
                clause_truth = True
                break
        if not clause_truth:
            all_clauses_satisfied = False
            break
    if (all_clauses_satisfied):
        # final print
        print()
        finalSuccessPrint(model)
        return True
    
    #check if some clause is false
    for clause in clauses:
        num_unassigned_symbols = 0
        clause_truth = False
        for literal in clause:
            symbol = literal if literal[0] != '-' else literal[1:]  
            value = 1 if literal[0] != '-' else -1
            if (model[symbol] == 0): 
                num_unassigned_symbols += 1
            elif (model[symbol] == value):
                clause_truth = True
                break
        if (not clause_truth and num_unassigned_symbols == 0):
            print("backtracking")
            return False
    
    if (UCH):
        uc_symbol, uc_value = findUnitClause(clauses, model)
        if (len(uc_symbol) != 0 and model[uc_symbol] == 0):
            print("forcing", uc_symbol, "=", uc_value)
            rest = symbols.copy()
            rest.remove(uc_symbol)
            model_uc = model.copy()
            model_uc[uc_symbol] = uc_value
            if (DPLL(clauses, rest, model_uc)):
                return True
    
    P = symbols[0]
    rest = symbols[1:]
    model_true = model.copy()
    model_false = model.copy()
    model_true[P] = 1
    model_false[P] = -1
    
    print()
    print("trying", P, "=", "T")
    if DPLL(clauses, rest, model_true):
        return True
    else:
        print()
        print("trying", P, "=", "F")
        return DPLL(clauses, rest, model_false)

if __name__ == "__main__":
    if len(sys.argv)<2: 
        sys.stderr.write("usage: python3 <filename> <literal>* [+UCH]")
        exit
    filename = str(sys.argv[1])
    arg_num = 2
    facts = []
    while (arg_num < len(sys.argv)):
        if (sys.argv[arg_num] == UCH_FLAG):
            UCH = True
        else:
            facts.append(sys.argv[arg_num])
        arg_num += 1
    
    clauses, model = populateKB(filename)
    #Put unique symbols into an array
    symbols = []
    for symbol in model:
        symbols.append(symbol)
    
    # remove facts from symbols
    for fact in facts:
        value = 1 if fact[0] != '-' else -1
        symbol = fact if fact[0] != '-' else fact[1:]
        model[symbol] = value
        symbols.remove(symbol)
    # perform DPLL
    if (not DPLL(clauses, symbols, model)):
        print()
        print("Problem is unsatisfiable")
        print("Number of DPLL Calls:", NUM_CALLS)
    #exit
    
                