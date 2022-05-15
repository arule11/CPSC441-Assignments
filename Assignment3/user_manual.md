
## CPSC441 Assignment 3
### Athena McNeil-Roberts 30042085

This program was run primarily on my personal Macintosh computer as well as the university 
Linux servers through ssh

- computes the following values for a given trace:
    - the incoming traffic, delivered/outgoing traffic, discarded/lost traffic, buffered traffic
    - packet loss percentage and queueing delay 

### Compile:
1. Download file as well as desired trace file to desired folder(s) 
2. Open a terminal and change directory to location where file is saved
3. Enter command:   "g++ router_buffer.cpp -o buffer"

### To Run:
1. After successfully compiled, enter command: "./buffer <file_path_to_trace>"
    - example: "./buffer soccer.txt"
