
## CPSC441 Assignment 4
### Athena McNeil-Roberts 30042085

This program was run primarily on my personal Macintosh computer as well as the university 
Linux servers through ssh

- simulates a pure aloha, where each bozon alternates between two states: sleeping and yodeling
- determines the average channel performance for specified: sleep duration S, yodel duration Y, 
    and number of bozons M
    - identifies:
        - the (average) percentage of the time that the channel is silent
        - the (average) percentage of the time that the channel is melodious
        - the (average) percentage of the time that the channel is screechy
        - number of attempted yodel
        - the percentage of attempted yodels that were perfect

### Compile:
1. Download file to desired folder(s) 
2. Open a terminal and change directory to location where file is saved
3. Enter command:   "gcc jumping_bozons.c -o bozon"
    - if running on cpsc servers: "gcc jumping_bozons.c -o bozon -lm"

### To Run:
1. After successfully compiled, enter command: "./bozon"
