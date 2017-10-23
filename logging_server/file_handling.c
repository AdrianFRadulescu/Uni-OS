//
// Created by Radulescu Adrian on 23/10/2017.
//

#include "file_handling.h"


void init_server_log(char* file_path_, char index_file_path_[]){

    server_log_ = malloc(sizeof(log_file_t));

    server_log_ ->log_desc_ = fopen(file_path_,"a");

    FILE* aux_fin = fopen(index_file_path_, "r");
    fscanf(aux_fin, "%d", &server_log_ ->line_index);
}

void close_server_log(){

    fclose(server_log_ ->log_desc_);
    free(server_log_);
}

void write_message_to_file(log_file_t* log_file_, char* text_){

    fprintf(log_file_ ->log_desc_, "%d %s", log_file_ ->line_index ++, text_);

    if ( strlen(text_) == 0 || !strchr(text_, '\n'))
        fprintf(log_file_ ->log_desc_, "\n");

    fflush(log_file_ ->log_desc_);
}

void update_log_index_file(char* file_path_){

    FILE* index_file_ = fopen(file_path_, "w");
    fprintf(index_file_, "%d", server_log_ ->line_index);
}
