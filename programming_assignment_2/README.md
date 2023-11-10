STEPS FOR RUNNING

1. Run 'make' in the command line
2. You can now run the code via the command line.
    - usage: ./DPLL <filename> <literal> [+UCH] [-FP]
    - the literal(s) and the +UCH flag are optional
    - I have also included another optional flag, -FP, that can be used if you only want the final model information (only the true symbols). It is set to false by default, as is the unit clause heuristic.

Extra notes:
1. If you have a .kb file, you must first convert it to cnf via the python script.
    - python3 convCNF.py <filename.kb> -DIMACS mapcolor.cnf
2. The program only parses -DIMACS format.
3. For the n-queens problem, I wrote a python script to generate the knowledge base. It works up to 6 queens, after which I cannot verify it's correctness. 