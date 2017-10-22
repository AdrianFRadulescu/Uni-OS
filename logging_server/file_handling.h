//
// Created by Radulescu Adrian on 23/10/2017.
//

#ifndef LOGGING_SERVER_FILE_HANDLING_H
#define LOGGING_SERVER_FILE_HANDLING_H

#endif //LOGGING_SERVER_FILE_HANDLING_H

#include <stdio.h>

FILE* log_desc_;

void write_to_file(FILE* file_desc_, char* text_){

    fprintf(file_desc_, "%s", text_);
}

