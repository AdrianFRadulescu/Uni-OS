//
// Created by Radulescu Adrian on 23/10/2017.
//

#ifndef LOGGING_SERVER_FILE_HANDLING_H
#define LOGGING_SERVER_FILE_HANDLING_H

#endif //LOGGING_SERVER_FILE_HANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct log_file{
    FILE* log_desc_;
    int line_index;
} log_file_t;

log_file_t* server_log_;


void init_server_log(char*, char*);

void close_server_log();

void write_message_to_file(log_file_t*, char*);

void update_log_index_file(char*);
