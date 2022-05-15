
## CPSC441 Assignment 2
### Athena McNeil-Roberts 30042085

This program was tested primarily on my personal Macintosh computer as well as the university 
Linux servers through ssh, using random text and sample sentences listed below. 

- prompts user to select a program option: 1 split, 2 merge, and 0 to quit
    - if user enters a number that is not one of the three options, they will be warned and reprompted
- when SIMPLE flag is 1 (and ADVANCED is 0) the split and merge are done with blank spaces
- when ADVANCED flag is 1 (and SIMPLE is 0) the split and merge are done with integer values
- user is reprompted until they select 0 and exit the program

### Compile:
1. Download files to desired folder
    - change SERVER_IP (in client) depending where running server program ("127.0.0.1" if on local)
2. Open a terminal and change directory to location where server file is saved
3. Enter command:   "gcc vowelizer_server.c -o server"
4. Open another terminal and change directory to location where client file is saved
5. Enter command:   "gcc vowelizer_client.c -o client"

### To Run:
1. After successfully compiled, in first (server) terminal, enter command: "./server"
2. Once server is running, and client successfully compiled, in second (client) terminal, 
enter command: "./client"
3. In second (client) terminal, select merge/split option and enter any of the sample sentences


### Sample Sentences:

abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ

Grocery list: milk, bread, and eggs.

The quick brown fox jumped over the lazy dog.

The Calgary Flames fortuitously won their game 6 to 5 in overtime last week. Cool!

The postal code for the University of Calgary is T2N 1N4.

In the teen song "Jenny I Need Your Number", her number is 7-5309 if I recall correctly.

It was the best of times; it was the worst of times.

My favourite winter Olympic sports are curling, skiing, and bobsledding.

Facetiously, I said "Curling rocks!".

Roses are red, Violets are blue, If you can do this test case, You'll score more than 2.

When cleaning the garage, I screamed "Eeeeeeeeek! I see a mouse!!!"

I wonder if there is a DNS entry for www.goooooooogle.com?