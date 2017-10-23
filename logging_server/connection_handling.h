//
// Created by Radulescu Adrian on 19/10/2017.
//


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include <memory.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <ctype.h>

#include "file_handling.h"

#ifndef LOGGING_SERVER_CONNECTION_HANDLING_H
#define LOGGING_SERVER_CONNECTION_HANDLING_H

#endif //LOGGING_SERVER_CONNECTION_HANDLING_H


pthread_mutex_t mutex;

/**
 * Control block for a thread handling a client
 * @member client_desc      the client socket descriptor
 * @member client_addr      the IPv6 address of the client
 * @member client_addr_size th size in bytes of the client address
 */

typedef struct thread_control_block{
    int client_desc;
    struct sockaddr_in6 client_addr;
    socklen_t client_addr_size;
} thread_control_block_t;

/**
 *  A  list of all the threads that are currently active.
 */

typedef struct thread_master{
    thread_control_block_t* thc_block_;
    struct thread_master* prev_;
    struct thread_master* next_;
} thread_master_t;

size_t m_size(thread_master_t*);

void* thread_process(void*);

int create_client_thread(int, thread_master_t*);

void start_multi_threaded_server(int, thread_master_t*);
