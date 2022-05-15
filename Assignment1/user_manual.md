
## CPSC441 Assignment 1
### Athena McNeil-Roberts

This program was tested using a Firefox browser primarily on my personal Macintosh computer as well as the university 
Linux servers through ssh. All webpages used to test are those listed below. 

- Proxy takes user port as input otherwise is ran using port 6971
- fork processes allowing more than browser/tab to run
- changes all instances of the word "Happy" (regardless of capitalizations) to the word "Silly"
- changes all instances of jpeg/png to a photo of one of two clowns
       - clown pic is chosen using a random value of either 1 or 0
- loads other contents such as text or gifs as the browser normally would

### Configuring Firefox Browser:
1. Open Firebox web browser, and go into settings
2. Under General > Network Settings > Settings, select "Manual proxy configuration" with the following info:
       HTTP Proxy: 127.0.0.1
       Port: 6971
3. Under Privacy & Security > Cookies and Site Data, select "Clear Data.." 

### Compile:
1. Download file to desired folder
2. Open terminal and change directory to location where file is saved
3. Enter command:   "g++ clown_proxy.cpp -o proxy"

### To Run:
4. After successfully compiled, to run enter command:   "./proxy <port>" or "./proxy" with defult port of 6971
    - user can enter a desired port (between 1100-49000) as a second param when running, however this
        port must then be the value entered as "Port" when configuring the web browser (step 2. configuring browser)
5. Open Firefox browser, that was previously configured, and load any of the following test pages:
    - clearing the cache (step 3. in configuring browser) each time the program is restarted

http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test1.html
http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test2.html
http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test3.html
http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test4.html
http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/test5.html
http://pages.cpsc.ucalgary.ca/~carey/
http://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/Floppy.jpg
