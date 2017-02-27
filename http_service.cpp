//
//  http_service.cpp
//  PigLatinVicarious
//
//  Created by Priyanka Chatterjee on 2/26/17.

#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "pig_latin.h"

#define REQUEST_MAX 4096
#define CONNECTION_MAX 100

using namespace std;

const char PORT[] = "8000";
int listenfd;
int connections[CONNECTION_MAX];

// Sets up a socket.
bool init(const char *port) {
    struct addrinfo hints, *res, *p;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, port, &hints, &res) != 0) {
        printf("Failed to get address info");
        return false;
    }
    
    for (p = res; p != NULL; p = p->ai_next) {
        listenfd = socket(p->ai_family, p->ai_socktype, 0);
        if (listenfd == -1)
            continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
            break;
    }
    if (p == NULL) {
        printf("Failed to bind to socket.");
        return false;
    }
    freeaddrinfo(res);
    if (listen(listenfd, CONNECTION_MAX) != 0) {
        printf("listen failed.");
        return false;
    }
    return true;
}

// Receives a http request, converts input to pig latin and returns it via http.
void processRequest(int slot) {
    char request[REQUEST_MAX], *start, *end;
    const char start_token[] = "<start>", end_token[] = "<end>";
    
    memset((void*)request, (int)'\0', REQUEST_MAX);
    
    int rcvd = (int) recv(connections[slot], request, REQUEST_MAX, 0);
    
    if (rcvd < 0)
        printf("Failed to receive request.");
    else if (rcvd == 0)
        printf("Client disconnected.");
    else {
        printf("\nConnection %d received input: %s\n", slot, request);
        
        // Request is of the form: GET <input> banana <end> HTTP/1.1
        if (strstr(request, "GET") != request ||
            (start = strstr(request, start_token)) == NULL ||
            (end = strstr(start, end_token)) == NULL ||
             strstr(end, "HTTP") == NULL) {
            // Malformed request.
            write(connections[slot], "HTTP/1.0 400 Bad Request\n", 25);
        } else {
            // Extract the input string and convert it to Pig Latin.
            string input(start + strlen(start_token), end - start - strlen(start_token));
            printf("Input string: %s", input.c_str());
            string response = convInputSent(input);
            send(connections[slot], "HTTP/1.0 200 OK\n\n", 17, 0);
            write(connections[slot], response.c_str(), response.size());
        }
    }
    
    shutdown(connections[slot], SHUT_RDWR);
    close(connections[slot]);
    connections[slot] = -1;
}

int main(int argc, char* argv[])
{
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    int slot = 0;
    
    printf("Server started at port no. %s\n",PORT);
    int i;
    for (i = 0; i < CONNECTION_MAX; i++)
        connections[i] = -1;
    
    init(PORT);
    while (true) {
        addrlen = sizeof(clientaddr);
        connections[slot] = accept(listenfd, (struct sockaddr *) &clientaddr, &addrlen);
        
        if (connections[slot] < 0)
            printf("\nConnection %d failed to accept connection.\n", slot);
        else {
            if (fork() == 0) {
                processRequest(slot);
                exit(0);
            }
        }
        while (connections[slot] != -1)
            slot = (slot + 1) % CONNECTION_MAX;
    }
    return 0;
}
