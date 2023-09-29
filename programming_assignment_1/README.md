Enter instructions and requirements for running your code here.

1. Run 'make' to compile the code
2. It is possible to run the code simply by executing via ./blocksworld <filepath>
    - filepath is the only required command line argument
        - It should follow the format of probs/probXXX.bwp
    - You can add an optional command line flag, -H <h> to specify what heuristic you want to use
        - Valid options of h are 0, 1, 2, and 3. 0 is used by default, though H2 performs the best.
    - There is also an optional maximum limitations flag, -I <i>
        - You can input any number for i, as long as it is positive. The default is 100,000.
3. Using H2, my program solves all A problems, and all B problems up to probB12.bwp, but exceeds the default iteration number of 100,000 without solving after B12. 
    - H3 performs marginally better for simple problems, but much worse for more complicated problems.
  
- Example command line runs:
-     ./blocksworld probs/A03.bwp
-     ./blocksworld probs/A03.bwp -H 2 -I 100000
-     ./blocksworld probs/A03.bwp -H 3
-     ./blocksworld probs/A03.bwp -I 10000
