STEPS FOR RUNNING

1. You can now run the code via the command line.
    - usage: python3 <filename> <literal>* [+UCH]
    - the literal(s) and the +UCH flag are optional, and you can as many literals as necessary, separated by a space
    - Output will be in the command line by default
    - All outputs are located in the transcripts folder

Extra notes:
1. If you have a .kb file, you must first convert it to cnf via the python script.
    - python3 convCNF.py <filename.kb> -DIMACS mapcolor.cnf
2. The program only parses -DIMACS format.
3. For the n-queens problem, I wrote a python script to generate the knowledge base. It works up to 6 queens, after which I cannot verify it's correctness. 
4. I also implemented the problem in C++, but it did not work well and I got too anxious the day after the deadline and redid the whole problem in python. Anyways, if you want to try that out (even though it performs much worse), here are the steps:
    - Run 'make' on the command line
    - usage: ./DPLL <filename> <literal>* [+UCH]