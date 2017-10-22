//
// Created by Radulescu Adrian on 19/10/2017.
//

#include "listening_socket.h"

int create_listening_socket (const uint16_t port, const int max_conn_number) {
    int socket_desc;
    struct sockaddr_in name;

    //zero the address
    memset(&name, 0, sizeof(name));

    // Create the socket.
    socket_desc = socket (PF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        perror ("socket");
        exit(EXIT_FAILURE);
    }

    // Give the socket a name.
    name.sin_family = AF_INET;
    name.sin_port = htons (port);
    name.sin_addr.s_addr = htonl (INADDR_ANY);

    if (bind (socket_desc, (struct sockaddr *) &name, sizeof (name)) < 0) {
        perror ("bind");
        exit (EXIT_FAILURE);
    }

    if (listen(socket_desc, max_conn_number)){
        perror("Error on listen");
        exit(EXIT_FAILURE);
    }

    return socket_desc;
}


