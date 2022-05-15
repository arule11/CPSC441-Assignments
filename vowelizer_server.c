/*
* CPSC 441 Assignment 2
* Athena McNeil-Roberts
* ID# 30042085
*/

#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define PORT_NUM 6971 // must match client port
#define MESSAGE_SIZE 256
#define MAXLINE 1024
#define SIMPLE 1
#define ADVANCED 0

/*
* Server Socket Programming code from tutorials authored by: William Black
*/

int main(int argc, char *argv[]){

    /* TCP setup */ 
    // socket creation for incomming TCP connections
    int listener_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(listener_fd == -1){ 
        perror("Failed to create listener_fd socket"); 
        return EXIT_FAILURE; 
    }
    printf("socket created..\n");
    
    // Initialize the socket take to allow connection from any IP on PORT_NUM
	struct sockaddr_in listener_sa;
	memset(&listener_sa, 0, sizeof(struct sockaddr_in));
	listener_sa.sin_family = AF_INET;
    listener_sa.sin_port = htons(PORT_NUM);
	listener_sa.sin_addr.s_addr = htonl(INADDR_ANY);

    // ask that socket be used on PORT_NUM 
	if(bind(listener_fd,(struct sockaddr*) &listener_sa, sizeof(struct sockaddr_in)) == -1){
		perror("Failed to bind listener socket\n");
		return EXIT_FAILURE;
	}
    printf("socket binded.. \n");
	
    // mark listener_fd as one that will accept connections
	// Set the queue length of incoming connections to be 5
	if(listen(listener_fd, 5) == -1) { return EXIT_FAILURE; }

    /* UDP setup */ 
    // socket creation for UDP connections
    ssize_t sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd == -1){
	    perror("Failed to create sockfd socket"); 
	    return EXIT_FAILURE; 
    }

    // Initialize the socket
    struct sockaddr_in sock;
    memset(&sock, 0, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PORT_NUM);
    sock.sin_addr.s_addr = INADDR_ANY;

    // ask that socket be used on PORT_NUM 
    if(bind(sockfd, (struct sockaddr*) &sock, sizeof(struct sockaddr_in))  == -1){
	    perror("Failed to bind sockfd socket\n");
		return EXIT_FAILURE;
    }

    struct sockaddr_in client;
    memset(&client, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(struct sockaddr_in);

	
    char buff[MESSAGE_SIZE];
    char consonants[MESSAGE_SIZE], vowels[MESSAGE_SIZE];
    int count, i, j, k;
	int connection_fd;

	while((connection_fd = accept(listener_fd, NULL, NULL)) != -1){
    
        bzero(buff, sizeof(buff));
        bzero(vowels, sizeof(vowels));
        bzero(consonants, sizeof(consonants)); 
        printf("Connection accepted...\n");

		int pid = fork();
		if(pid == 0){ // Child
		    close(listener_fd);

            while(recvfrom(sockfd, (char *)buff, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &client, (socklen_t*)&len) > 0){

                int option = atoi(&buff[0]); // option is the first char of the message
                bzero(buff, sizeof(buff)); 

                if(option == 1){ // split
                    
                    if(recv(connection_fd, buff, sizeof(buff), 0) == -1){ // receive the message TCP
                        fprintf(stderr, "connection_fd error in receiving!\n");
                        exit(1);
                    }

                    count = 0, i = 0, j = 0, k = 0;
                    bzero(vowels, sizeof(vowels));
                    bzero(consonants, sizeof(consonants));

                    while(buff[i] != '\0'){
                        if (buff[i] == 'a' || buff[i] == 'e' || buff[i] == 'i' || buff[i] == 'o' || buff[i] == 'u' ||
                        buff[i] == 'A' || buff[i] == 'E' || buff[i] == 'I' || buff[i] == 'O' || buff[i] == 'U'){
                            // char is a vowel
                            #if SIMPLE
                                vowels[j] = buff[i];
                                consonants[j] = ' ';
                            #endif

                            #if ADVANCED
                                vowels[j] = count + '0';
                                j++;
                                vowels[j] = buff[i];
                                count = 0;
                                j++;
                            #endif

                        }else{
                            // char is a consonant
                            #if SIMPLE
                                consonants[j] = buff[i];
                                vowels[j] = ' ';
                            #endif

                            #if ADVANCED
                                consonants[k] = buff[i];
                                count++;
                                k++;
                            #endif
                        }
                        #if SIMPLE
                            j++; 
                        #endif
                        i++;
                    }
                    #if SIMPLE
                        consonants[j] = '\0';
                    #endif
                    #if ADVANCED
                        consonants[k] = '\0';
                    #endif
                    vowels[j] = '\0';

                    send(connection_fd, consonants, MESSAGE_SIZE, 0); // send split consonants part TCP
                    printf("Sent %lu bytes %s to client using TCP \n", strlen(consonants), consonants);
                    sendto(sockfd, (char *)vowels, strlen(vowels), 0, (const struct sockaddr *) &client, len); // send split vowels part UDP
                    printf("Sent %lu bytes %s to client using UDP \n\n", strlen(vowels), vowels);
        
                }else if(option == 2){ // merge
                    
                    if(recv(connection_fd, consonants, sizeof(consonants), 0) == -1){ // receive split consonants part TCP
                        fprintf(stderr, "connection_fd error in receiving!\n");
                        exit(1);
                    }

                    // receive split vowels part UDP
                    recvfrom(sockfd, (char *)vowels, MAXLINE, MSG_WAITALL, (struct sockaddr *) &client, (socklen_t*)&len);
        
                    int count = atoi(&vowels[0]);
                    int j = 1, k = 0;

                    #if SIMPLE
                        int len = sizeof(consonants) > sizeof(vowels) ? sizeof(consonants) : sizeof(vowels);
                    #endif
                    #if ADVANCED
                        int len =  strlen(consonants) + (strlen(vowels) / 2) + 1;
                    #endif
                    
                    for(int i = 0; i < len; i++){
                        #if SIMPLE
                            if(consonants[i] != ' '){
                                buff[i] = consonants[i];
                            }else{
                                buff[i] = vowels[i];
                            }
                        #endif

                        #if ADVANCED
                            if(count == 0 && (vowels[j - 1] != '\0' || vowels[j] != '\0')){
                                buff[i] = vowels[j];
                                count = atoi(&vowels[j + 1]);
                                j = j + 2;
                            }else{
                                count--;
                                buff[i] = consonants[k];
                                k++;
                            }
                        #endif
                    }
                    send(connection_fd, buff, MESSAGE_SIZE, 0); // send merged message TCP
                    printf("Sent %lu bytes: %s to client using UDP \n\n", strlen(buff), buff);
                
                }else if(option == 0){
                    break; // exit while and close
                }
                // clear all after each operation
                bzero(buff, sizeof(buff));
                bzero(vowels, sizeof(vowels));
                bzero(consonants, sizeof(consonants)); 
            }
            close(connection_fd);
            return EXIT_SUCCESS;

		}else if(pid != 0){ // Parent
			close(connection_fd);
		}else{ // Error
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
