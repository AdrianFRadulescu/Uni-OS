#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <math.h>

#include "listening_socket.h"
#include "connection_handling.h"

int server_socket_desc;
thread_master_t* th_master_;


/**
 * Free the resources corresponding to the active nodes in the threads
 */

void free_thread_resources(){

    printf("freeing memory");
    thread_master_t* aux_;

    while(th_master_){

        aux_ = th_master_;
        th_master_ = th_master_ ->next_;

        if (aux_ ->thc_block_) {

            if (aux_ ->thc_block_ ->client_desc != -1){
                shutdown(aux_ ->thc_block_ ->client_desc, 2);
                aux_ ->thc_block_ ->client_desc = -1;
            }

            free(aux_ ->thc_block_);
            aux_ ->thc_block_ = NULL;
        }

        aux_ ->prev_ = NULL;
        aux_ ->next_ = NULL;
        free(aux_);
    }


}

void INT_handling(int sig_no) {

    if (sig_no == SIGINT){

        fprintf(stderr, "shutting server down");

        // free memory
        pthread_mutex_lock(&mutex);
        free_thread_resources();
        pthread_mutex_unlock(&mutex);

        // shutdown active client sockets and free memory
        shutdown(server_socket_desc, 2);
        update_log_index_file("log_file_index");
        close_server_log();
        _exit(EXIT_SUCCESS);

    }
}

int main(int argc, char* argv[]) {

    char* end;
    uint16_t port;
    if (argc == 3) {
        port = (uint16_t) strtol(argv[1], &end, 10);
    } else {
        port = 3349;
    }

    if (1024 >= port || port > pow(2, 31)) {
        perror("Incorrect port number");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, INT_handling) == SIG_ERR){
        perror("can't catch signal\n");
        exit(EXIT_FAILURE);
    }

    // init the server_log structure
    init_server_log(argv[2], "log_file_line_index");

    server_socket_desc = create_listening_socket(port, 2000);

    th_master_ = malloc(sizeof(thread_master_t));
    th_master_ ->thc_block_ = NULL;
    th_master_ ->prev_ = NULL;
    th_master_ ->next_ = NULL;

    start_multi_threaded_server(server_socket_desc, th_master_);

    shutdown(server_socket_desc, 2);

    if (th_master_ == NULL)
        exit(EXIT_FAILURE);

    update_log_index_file("log_file_index");
    close_server_log();
    free(th_master_);
    return 0;
}

