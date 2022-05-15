/*
* CPSC 441 Assignment 1
* Athena McNeil-Roberts
*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <regex>

#define PORT_NUM 6971
#define HTTP_PORT 80
#define MESSAGE_SIZE 40000
#define SIZE 256

/*
* Server / Client Socket Programming code from tutorials authored by: William Black
*/
int main(int argc, char *argv[]){

	int port;
    if(argc > 1){
        if(atoi(argv[1]) >= 1100 && atoi(argv[1]) <= 49000){ 
            port = atoi(argv[1]); // use port number specified by user
        }else{
            port = PORT_NUM;
        }
    }else{
        port = PORT_NUM; // defult to port num marco
    }
    printf("Using port %d...\n", port);

    char message_in[MESSAGE_SIZE], message_out[MESSAGE_SIZE];
	int connection_fd;

    // socket creation for incomming TCP connections
    int listener_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(listener_fd == -1){ 
        perror("Failed to create listener socket"); 
        return EXIT_FAILURE; 
    }
    printf("socket created..\n");

	//Initialize the socket take to allow connection from any IP on portNum
	struct sockaddr_in listener_sa;
	memset(&listener_sa, 0, sizeof(struct sockaddr_in));
	listener_sa.sin_family = AF_INET;
    listener_sa.sin_port = htons(port);
	listener_sa.sin_addr.s_addr = htonl(INADDR_ANY);

	// ask that socket be used on portNum 
	if(bind(listener_fd,(struct sockaddr*) &listener_sa, sizeof(struct sockaddr_in)) == -1){
		perror("Failed to bind listener socket\n");
		return EXIT_FAILURE;
	}
    printf("socket binded.. \n");
	
	// mark listener_fd as one that will accept connections
	// Set the queue length of incoming connections to be 5
	if(listen(listener_fd, 5) == -1) { return EXIT_FAILURE; }

    // connection_fd used for the incoming socket
	while((connection_fd = accept(listener_fd, NULL, NULL)) != -1){
        printf("Connection accepted...\n");

        int pid = fork();
        if(pid == 0){ // child 
            close(listener_fd);

            if(recv(connection_fd, message_in, sizeof(message_in), 0) == -1){
                fprintf(stderr, "connection_fd error in receiving!\n");
                exit(1);
            }
            printf("CLIENT says: %s\n", message_in);

            char host[SIZE];
            char url[SIZE];

            strcpy(message_out, message_in); // copy msg received to msg to send
            strcat(message_out, "\r\n\r\n"); // add carriage return new line to end out msg to send
            // char '~' is sometimes changed to 'ü' for unknown reason 

            sscanf(message_in, "GET http://%s", url); // get the url
            printf("URL: %s\n", url);

            // to parse the url to get the host:
            // https://stackoverflow.com/questions/3217629/how-do-i-find-the-index-of-a-character-within-a-string-in-c
            int i = (int)(strchr(url, '/') - url);
            strncpy(host, url, i); // get the host
            host[i] = '\0';
            printf("Host: %s\n", host);

            struct sockaddr_in serv_addr;
            struct hostent *address;
            address = gethostbyname(host); // get IP

            // address initialization 
            memset(&serv_addr, 0, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_port = htons(HTTP_PORT); // use port 80
            bcopy((char *)address->h_addr, (char *)&serv_addr.sin_addr.s_addr, address->h_length);
            
            // socket creation
            int web_fd = socket(AF_INET, SOCK_STREAM, 0);
            if(web_fd == -1){
                perror("Failed to create web_fd socket"); return 1;
            }

            // connection request
            if(connect(web_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1){
                perror("Failed to connect web_fd"); return 1;
            }
            
            if((strstr(message_out, ".jpg")) != NULL){ // if url is to a jpg change to clown photo 2
                std::string temp = message_out;
                srand(time(NULL));
                if(rand() % 2){
                    temp = std::regex_replace(temp, std::regex("([^\\s]*?).(jpg|png)"), "https://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/clown1.png");
                }else{
                    temp = std::regex_replace(temp, std::regex("([^\\s]*?).(jpg|png)"), "https://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/clown2.png");
                }

                char new_msg[MESSAGE_SIZE];
                bzero(new_msg, sizeof(new_msg));
                strcat(new_msg, temp.c_str());
                
                // send request to web server
                if(send(web_fd, new_msg, sizeof(new_msg), 0) == -1){
                    perror("Failed to send web_fd - new_msg"); return 1;
                }
            }else{
                // send request to web server
                if (send(web_fd, message_out, sizeof(message_out),0) == -1){
                    perror("Failed to send web_fd"); return 1;
                }
            }

            memset(message_in, '\0', sizeof(message_in)); // clear the received
            int bytes; 
            while((bytes = recv(web_fd, message_in, sizeof(message_in), MSG_WAITALL)) > 0){
                printf("WEB says: %s\n", message_in);
                
                char *response = strstr(message_in, "200 OK"); 
                if(response != NULL){ // response is OK
                
                    char type[SIZE];
                    char* type_ptr = strstr(message_in, "Content-Type: ");
                    if (type_ptr != NULL) {
                        sscanf(type_ptr, "Content-Type: %s", type); // get the content type
                    }

                    char* type_check;
                    if((type_check = strstr(type, "text")) != NULL){ 

                        std::string temp = message_in;
                        std::regex happy("Happy*", std::regex_constants::icase); // all instances of happy regardless of case
                        temp = std::regex_replace(temp, happy, "Silly"); // replace with Silly
            
                        srand(time(NULL));
                        int x = rand() % 2; // random number (1 or 0) to choose clown pic  
                        if(x){ 
                            temp = std::regex_replace(temp, std::regex("(img+( src+)+)=\"(.*?).(jpg|png)\" alt=\"(.*?)\""), "img src=\"https://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/clown1.png\" alt=\"Clown Photo\"");
                            temp = std::regex_replace(temp, std::regex("href=\"(.*?).(jpg|png)\">(.*?)<"), "href=\"https://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/clown1.png\">Clown Photo<");
                            // change all instances of ".jpg" or ".png" to photo of clown 1
                            temp = std::regex_replace(temp, std::regex("=\"([^\\s]*?).(jpg|png)\""), "=\"https://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/clown1.png\"");
                        }else{
                            temp = std::regex_replace(temp, std::regex("(img+( src+)+)=\"(.*?).(jpg|png)\" alt=\"(.*?)\""), "img src=\"https://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/clown2.png\" alt=\"Clown Photo\"");
                            temp = std::regex_replace(temp, std::regex("href=\"(.*?).(jpg|png)\">(.*?)<"), "href=\"https://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/clown2.png\">Clown Photo<");
                            // change all instances of ".jpg" or ".png" to photo of clown 2
                            temp = std::regex_replace(temp, std::regex("=\"([^\\s]*?).(jpg|png)\""), "=\"https://pages.cpsc.ucalgary.ca/~carey/CPSC441/ass1/clown2.png\"");
                        }

                        char new_msg[MESSAGE_SIZE];
                        bzero(new_msg, sizeof(new_msg));
                        strcat(new_msg, temp.c_str());
                        printf("NEW: %s\n", new_msg);

                        if(send(connection_fd, new_msg, sizeof(new_msg), 0) == -1){
                            perror("Error sending new_msg\n"); return 1;
                        }

                    }else{ // if Content-Type is not text ie image or other
                        if (send(connection_fd, message_in, bytes, 0) == -1){ // send the org contents to client - in bytes
                            perror("Failed to send connection_fd"); return 1;
                        }
                    }
                }

            }
            close(web_fd);
            close(connection_fd);
        }else if(pid != 0){ // parent
            close(connection_fd);
        }else{ // error
            return EXIT_FAILURE;
        }
	}
    close(listener_fd);
	return EXIT_SUCCESS;
}
