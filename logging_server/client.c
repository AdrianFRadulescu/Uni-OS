

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>

#define BUFFER_LENGTH 512


typedef struct mem_register{
    void* elem;
} mem_reg_t;

char* get_line(FILE *fp_, size_t size, int *ok){

    char* line_ = realloc(NULL, sizeof(char) * size), ch;
    size_t len = 0;
    while(EOF != (ch = (char) fgetc(fp_)) && ch != '\n'){
        line_[len ++] = ch;
        if(len == size){
            line_ = realloc(line_, sizeof(char) * (size += 16));
            if (!line_) exit(EXIT_FAILURE);
        }
    }

    if(ch == EOF) {
        *ok = 0;
    }
    line_[len ++] = '\n';
    line_[len ++] = '\0';
    return realloc(line_, sizeof(char) * len);
}

void sig_handling(int sig_no){

    if (sig_no == SIGINT){

        //free memory if still allocated

        _exit(EXIT_SUCCESS);
    } else if (sig_no == SIGPIPE){

        _exit(EXIT_SUCCESS);
    }

}

int main(int argc, char *argv[]) {
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server_;

    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd <0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_ = gethostbyname(argv[1]);
    if (!server_){
        perror("could not find server");
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = PF_INET;

    bcopy(server_ ->h_addr_list[0],(char*) &serv_addr.sin_addr.s_addr, (size_t) server_ ->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        shutdown(sockfd, 2);
        exit(EXIT_FAILURE);
    }

    // read message from stdin

    int keep_reading = 1;
    char* line_;

    while(keep_reading){
        line_ = get_line(stdin, 16, &keep_reading);
        printf("%zu\n", strlen(line_));
        send(sockfd, line_, sizeof(char) * strlen(line_) , 0);
        //read(sockfd, buffer, BUFFER_LENGTH);

        free(line_);
        line_ = NULL;
    }
    printf("eof reached\n");
    shutdown(sockfd, 2);
    return 0;
}