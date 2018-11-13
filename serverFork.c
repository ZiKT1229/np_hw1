#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>

char webPage[] =
"HTTP/1.1 200 Ok\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html><head><title>Test Title</title>\r\n"
"<body>Hello World <img src='coding.png'/></body>"
"</html>";

char imageheader[] = 
"HTTP/1.1 200 Ok\r\n"
"Content-Type: image/jpeg\r\n\r\n";

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int server_fd, client_fd;
    char buf[2048];
    int img_fd;
    
	struct addrinfo *res, *p;
    struct addrinfo hints;
	memset (&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

	if (getaddrinfo(NULL, "8080", &hints, &res) != 0) {
		perror ("getaddrinfo() error");
		exit(1);
	}
    
    for (p = res; p != NULL; p = p->ai_next) {
		server_fd = socket (p->ai_family, p->ai_socktype, 0);
		if (server_fd == -1) 
            continue;
		if (bind(server_fd, p->ai_addr, p->ai_addrlen) == 0) 
            break;
	}

    if (p == NULL) {
		perror ("socket() or bind() eror");
		exit(1);
	}
    
    freeaddrinfo(res);

    if (listen(server_fd, 10) == -1) {
        perror("listen");
        close(server_fd);
        exit(1);
    }
    
    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &addrlen);

        if (fork() == 0) {
            close(server_fd);
            memset(buf, 0, 2048);
            read(client_fd, buf, 2047);

            printf("%s\n", buf);

            char data_to_send[1024];
            int bytes_read;
            if (!strncmp(buf, "GET /coding.png", 15)) {
                img_fd = open("coding.png", O_RDONLY);
                write(client_fd, imageheader, sizeof(imageheader) - 1);
                int sent = sendfile(client_fd, img_fd, NULL, 60000);
                printf("sent: %d\n", sent);
                close(img_fd);
            } else {
                write(client_fd, webPage, sizeof(webPage) - 1);
            }

            close(client_fd);
            printf("closing...\n");
            exit(0);
        }
        close(client_fd);
    }
    return 0;
}
