# Task

Your primary task in this assignment is to build a network-based application that supports the text splitting and merging operations described above.


- The client end of your application will first initiate a TCP connection to the server to establish a session. Within this interactive session, the client can choose what commands to perform (e.g., split, merge, quit), enter the text that is to be processed, view the results from the server, etc.

- The server end of your application will passively listen on a TCP port number of your own choosing for connection attempts from one or more clients. For each new client, it establishes an interactive session, perhaps indicating a simple menu of commands that the server supports. Incoming data from the client(s) is then either split or merged, depending on the operations requested, and the results sent back to the requesting client. Note that some of this work involves TCP, and some of it UDP.

Two different versions of split/merge operations should be supported.
1. In the simple case, incoming text like "Hello there!" is split into two parts "H ll th r !" (on TCP) and " e o e e " (on UDP), with blank spaces as placeholders to keep both text strings the same length, and easy to merge later. 
    - Note also that any punctuation, numbers, or real blank spaces in the original text should be preserved (i.e., in the non-vowel string), so that you can put things back together properly later. 
2. In the advanced case, incoming text like "Hello there!" is split into two parts "Hll thr!" (on TCP) and "1e2o3e1e" (on UDP), with (single-digit, non-negative) integer values denoting how many letters to skip before placing the next indicated vowel when merging the text. You can assume that the relative displacement value in normal English text never exceeds 9. Note that this advanced encoding for the text may have different lengths for the TCP and UDP data. Also, the punctuation, numbers, and real blank spaces appear only in the TCP text data, so that the UDP carries vowel information only.
