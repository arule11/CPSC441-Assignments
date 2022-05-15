
# Background

The Bozons are curious but friendly creatures who spend all of their lives alternating between two states: sleeping and yodeling. Bozons live in colonies of size M.

The durations of the sleep periods are independent and identically distributed (iid) random variables drawn from an exponential distribution with a mean of S time units (e.g., S = 8.5 hours). The durations of the yodeling periods are iid random variables drawn from an exponential distribution with a mean of Y time units (e.g., Y = 1.2 hours).

# Task

Your task in this assignment is to study the performance of the Bozon channel access protocol.

Specifically, write a computer simulation program (in C or C++) that can model (i.e., simulate) the channel access protocol used in a Bozon colony, and determine the average channel performance. 
- For assessing "channel performance", you should identify the (average) percentage of the time that the channel is silent (i.e., all Bozons are sleeping), melodious (i.e., single Bozon is yodeling), and screechy (i.e., more than one Bozon yodels at the same time). 
- Your program should also report the number of attempted yodels, and the percentage of attempted yodels that were perfect. A "perfect yodel" is one that was melodious from start to finish, without any interruption by other Bozons.

Your program should be easily parameterizable for different values of M, S, and Y (either on the command line, or by prompting the user, or as manifest constants in the source code). You will use these parameters to explore several Bozon configuration scenarios listed below. You may assume that all the Bozons are initially asleep at time zero
