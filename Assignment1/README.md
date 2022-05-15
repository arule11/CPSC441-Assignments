
# Technical Requirements

In this assignment, you will implement your very own clown proxy, in either C or C++. 

The goals of the assignment are to build a properly functioning Web proxy for simple Web pages, and then use your proxy to alter certain content items before they are delivered to the browser.

There are two main pieces of functionality needed in your proxy: 
1. The ability to handle HTTP requests and responses, while still forwarding them between client and server.
2. The ability to parse, and possibly modify, HTTP requests and responses.

Your proxy should be able to do two specific things:
1. First, it should replace all occurrences of the word "Happy" with the word "Silly" in an HTTP response. 
2. Second, it should replace all JPG image files on a given Web page with an image of a happy clown instead.


## Bonus (optional)

Up to 4 bonus marks will be given for a clown proxy that can randomly choose from a small set of different clown images when doing image substitution, resulting in a page that varies every time you load it. 
