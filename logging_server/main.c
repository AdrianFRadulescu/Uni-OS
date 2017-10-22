#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <math.h>

#include "file_handling.h"
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
            free(aux_->thc_block_);
            aux_ ->thc_block_ = NULL;
        }

        aux_ ->prev_ = NULL;
        aux_ ->next_ = NULL;
        free(aux_);
    }


}

void INT_handling(int sig_no) {

    if (sig_no == SIGINT){
        printf("Do you want to shut the server down(y/n)?\n");
        if (fgetc(stdin) == 'y') {

            free_thread_resources();

            //shutdown active client sockets and free memory
            shutdown(server_socket_desc, 2);
            _exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char* argv[]) {

    char* end;
    uint16_t port;
    if (argc == 2) {
        port = (uint16_t) strtol(argv[1], &end, 10);
    } else {
        port = 3349;
    }

    if (1024 >= port || port > pow(2, 31)) {
        perror("Incorrect port number");
        exit(EXIT_FAILURE);
    }

    if (signal(SIGINT, INT_handling) == SIG_ERR){
        printf("can't catch signal\n");
        exit(EXIT_FAILURE);
    }

    log_desc_ = fopen("/Users/adrian_radulescu1997/Documents/Uni-Courses/III/Operating-Systems-Uni-Course/homeworks/logging_server/logs.log", "a");

    server_socket_desc = create_listening_socket(port, 2000);

    th_master_ = malloc(sizeof(thread_master_t));
    th_master_ ->thc_block_ = NULL;
    th_master_ ->prev_ = NULL;
    th_master_ ->next_ = NULL;

    while(create_client_thread(server_socket_desc, th_master_) == EXIT_SUCCESS);

    printf("here\n");

    shutdown(server_socket_desc, 2);

    if (th_master_ == NULL)
        exit(EXIT_FAILURE);

    free(th_master_);

    return 0;
}

