//
// Created by Radulescu Adrian on 19/10/2017.
//

#include "connection_handling.h"

#define BUFFER_LENGTH 512

/**
 * Get the size of the thread list
 * @param th_master
 * @return
 */

size_t m_size(thread_master_t* th_master){

    size_t size = 0;

    while (th_master){
        size ++;
        th_master = th_master ->next_;
    }
    return size;
}

static char* get_message(int socket_desc, int* ok){

    char* buffer = calloc(BUFFER_LENGTH, sizeof(char));
    size_t len = 0;
    while (1){
        if (read(socket_desc, &buffer[len], 1) != 1){
            // the connection is no longer readable
            *ok = 0;
            break;
        }
        else if (len == BUFFER_LENGTH || buffer[len] == '\n')
            break;
        else
            len ++;
    }

    return realloc(buffer, sizeof(char) * len);
}

/**
 * Reads and parses a request from a client and then sends the appropriate response
 * @param client_desc client descriptor
 * @param client_info contains the
 */

static void handle_client_request(const int client_desc){


    fprintf(stderr, "processing request\n");

    //size_t buffer_index = 0;
    char* message_;
    int keep_reading = 1;

    while (keep_reading) {
        message_ = get_message(client_desc, &keep_reading);
        fprintf(stderr, "client: %d %s", client_desc, message_);

        // write message to file



        free(message_);
    }

    fprintf(stderr, "request processed");
}

static void clear_thread(void* th_data_, thread_master_t* th_master_){

    if (th_master_ ->prev_)
        th_master_ ->prev_ ->next_ = th_master_ ->next_;
    if (th_master_ ->next_)
        th_master_ ->next_ ->prev_ = th_master_ ->prev_;

    th_master_ ->prev_ = NULL;
    th_master_ ->next_ = NULL;
    th_master_ ->thc_block_ = NULL;

    fprintf(stderr, "closing connection");

    free(th_data_);
}

/**
 * Runs the process of a client thread
 * @param th_data_
 * @return
 */

void* thread_process(void* th_data_){

    thread_master_t* th_master_ = (thread_master_t*) th_data_;
    thread_control_block_t *thc_block_ = th_master_ ->thc_block_;


    handle_client_request(thc_block_ ->client_desc);


    // remove thread form active thread list

    clear_thread(th_data_, th_master_);

    pthread_exit(EXIT_SUCCESS);
}

/**
 * Creates a client thread
 * @param server_socket_desc the socket used by the server
 * @return
 */

int create_client_thread(const int server_socket_desc, thread_master_t* th_master ){

    thread_control_block_t* thc_block_ = malloc(sizeof(thread_control_block_t));

    if (!thc_block_){
        perror("malloc");
        return EXIT_FAILURE;
    }

    if ((thc_block_ ->client_desc = accept(server_socket_desc, (struct sockaddr*)&(thc_block_ ->client_addr), &(thc_block_ ->client_addr_size))) < 0){
        perror("accept");
        return EXIT_FAILURE;
    }

    pthread_t server_thread;
    pthread_attr_t server_thread_attr;

    // create separate thread for processing
    if (pthread_attr_init(&server_thread_attr)){
        perror("Creating initial thread attributes failed");
        free(thc_block_);
        return EXIT_FAILURE;
    }

    if (pthread_attr_setdetachstate(&server_thread_attr, PTHREAD_CREATE_DETACHED)){
        perror("Setting thread attributes failed");
        free(thc_block_);
        return EXIT_FAILURE;
    }

    // add the thread control to the list of threads

    th_master ->next_ = malloc(sizeof(thread_master_t));
    th_master ->next_ ->prev_ = th_master;
    th_master ->next_ ->next_ = NULL;
    th_master ->next_ ->thc_block_ = thc_block_;


    if (pthread_create(&server_thread, &server_thread_attr, &thread_process, (void*) th_master ->next_)){
        perror("Thread creation failed");
        free(thc_block_);
        return EXIT_FAILURE;
    }

    printf("thread created\n");

    return EXIT_SUCCESS;
}


