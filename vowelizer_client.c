/*
* CPSC 441 Assignment 2
* Athena McNeil-Roberts
* ID# 30042085
*/

#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h> 
#include<string.h> 
#include<unistd.h> 
#include<stdlib.h> 

#define PORT_NUM 6971 // must match server port
#define MESSAGE_SIZE 256
// #define SERVER_IP "127.0.0.1" // local
#define SERVER_IP "136.159.5.25" // cpsc servers 25 26 or 27

/*
* Client Socket Programming code from tutorials authored by: William Black
*/

void printMenu(){
    printf("\nPlease choose from the following selections:\n");
    printf(" 1 - Devowel a message \n 2 - Envowel a message \n 0 - Exit program \n");    
    printf("Your desired menu selection? ");
}

int main(){

    /* TCP setup */ 
    // socket creation for TCP connections
	int connection_fd = socket(AF_INET,SOCK_STREAM,0);
	if(connection_fd == -1){
        perror("Failed to create connection_fd socket"); return 1;
    }

	struct sockaddr_in connection_sa;
	memset(&connection_sa,0,sizeof(struct sockaddr_in));
	connection_sa.sin_family = AF_INET;
	connection_sa.sin_port = htons(PORT_NUM);
	connection_sa.sin_addr.s_addr = inet_addr(SERVER_IP);

    // connection request
    if(connect(connection_fd, (struct sockaddr *)&connection_sa, sizeof(struct sockaddr_in)) == -1){
        perror("Failed to connect connection_fd"); return 1;
    }

    /* UDP setup */ 
    // socket creation for UDP connections
    ssize_t sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd == -1) {
        perror("Failed to create socket file discriptor");
        return 1;
    }
    
    struct sockaddr_in sock;
    memset(&sock, 0, sizeof(struct sockaddr_in));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(PORT_NUM);
    inet_pton(AF_INET, SERVER_IP, &sock.sin_addr);
    int len = sizeof(sock);

    // set timer for sockfd
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

	char buff[MESSAGE_SIZE];
    int i = 0;
    
    while(1){
        // get user function choice
        bzero(buff, sizeof(buff));
        printMenu();
        scanf("%s", buff);
        int option = atoi(&buff[0]);
        
        if(option == 1 || option == 2 || option == 0){
            
            sendto(sockfd, (char *)buff, strlen(buff), 0, (const struct sockaddr *) &sock, sizeof(sock)); // send selection option to server
            bzero(buff, sizeof(buff)); 

            if(option == 1){ // split
                 
                char ch = getchar(); 
                printf("\nEnter your message to devowel: ");
        
                i = 0;
                while((ch = getchar()) != '\n') {
                    buff[i] = ch;
                    i++;
                }
                buff[i] = '\0';
                send(connection_fd, buff, sizeof(buff), 0); // send message to server
            
                bzero(buff, sizeof(buff));
                if(recv(connection_fd, buff, MESSAGE_SIZE, 0) == -1){ // receive split consonants part of message TCP
                    fprintf(stderr, "connection_fd error in receiving!\n");
                    return 1;
                }
                printf("\nServer sent %lu btyes of non-vowels on TCP: %s\n", strlen(buff), buff);
                
                bzero(buff, sizeof(buff));

                // receive split vowels part of message UDP
                if(recvfrom(sockfd, (char *)buff, MESSAGE_SIZE, MSG_WAITALL, (struct sockaddr *) &sock, (socklen_t *)&len) < 0){
                    printf("timed out..\n");
                    return 1;
                }
                printf("Server sent %lu  btyes of vowels on UDP   : %s\n", strlen(buff), buff); 

            }else if(option == 2){ // merge

                char ch = getchar();
                printf("\nEnter non-vowel part of message to envowel: ");
                i = 0;
                while((ch = getchar()) != '\n') {
                    buff[i] = ch;
                    i++;
                }
                buff[i] = '\0';
                send(connection_fd, buff, sizeof(buff), 0); // send split consonants part TCP

                bzero(buff, sizeof(buff));
                printf("Enter vowel part of message to envowel: ");
                i = 0;
                while((ch = getchar()) != '\n') {
                    buff[i] = ch;
                    i++;
                }
                buff[i] = '\0';
                
                // send split vowels part UDP
                sendto(sockfd, (const char *)buff, strlen(buff), 0, (const struct sockaddr *) &sock, sizeof(sock)); 
                
                bzero(buff, sizeof(buff));  
                if(recv(connection_fd, buff, MESSAGE_SIZE, 0) == -1){ // receive merged message 
                    fprintf(stderr, "connection_fd error in receiving!\n");
                    return 1;
                }
                printf("\nServer sent %lu btyes of message using TCP: %s\n", strlen(buff), buff);

            }else{ // exit
                printf("\nExiting..\n");
                break; // exit while and close
            }

        }else{ // option not a valid menu choice
            printf("\nInvalid option selection. Choose another.\n"); // reprompt for another input
        }
    }
	close(connection_fd);	
	return 0;
}
