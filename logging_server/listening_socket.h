//
// Created by Radulescu Adrian on 19/10/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <stdint.h>

#ifndef LOGGING_SERVER_LISTENING_SOCKET_H
#define LOGGING_SERVER_LISTENING_SOCKET_H

#endif //LOGGING_SERVER_LISTENING_SOCKET_H

int create_listening_socket(uint16_t port, int max_conn_number);
