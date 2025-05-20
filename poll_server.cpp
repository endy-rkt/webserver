#include "header.hpp"

#define MAXLINE 2048
#define OPEN_MAX _SC_OPEN_MAX
#define LISTENQ 10

int runServer()
{
    int i, maxi, connfd, sockfd;
    int nready;
    ssize_t n;
    char buf[MAXLINE];
    socklen_t clilen;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in serverAddr, cliaddr;

    // Create socket
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        perror("socket error");
        exit(1);
    }

    // Set socket options (optional but recommended)
    int opt = 1;
    if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt error");
        exit(1);
    }

    // Set up address settings
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7000);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind socket and listen
    
    if (bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(1);
    }

    if (listen(serverSock, LISTENQ) < 0) {
        perror("listen error");
        exit(1);
    }

    /* begin poll */
    client[0].fd = serverSock;
    client[0].events = POLLRDNORM;
    for (i = 1; i < OPEN_MAX; i++)
        client[i].fd = -1;
    maxi = 0;

    for (;;) {
        nready = poll(client, maxi + 1, -1); // Wait indefinitely

        if (nready < 0) {
            perror("poll error");
            exit(1);
        }

        // New connection
        // POLLRDNORM = Poll Read normal we have also POLLWRNORM
        if (client[0].revents & POLLRDNORM) {
            clilen = sizeof(cliaddr);
            connfd = accept(serverSock, (struct sockaddr *)&cliaddr, &clilen);
            if (connfd < 0) {
                perror("accept error");
                continue;
            }
            for (i = 1; i < OPEN_MAX; i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    client[i].events = POLLRDNORM;
                    break;
                }
            }
            if (i == OPEN_MAX) {
                fprintf(stderr, "too many clients\n");
                close(connfd);
            }
            if (i > maxi)
                maxi = i;
            if (--nready <= 0)
                continue;
        }
        // Check all clients for data
        for (i = 1; i <= maxi; i++) {
            if ((sockfd = client[i].fd) < 0)
                continue;
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(sockfd, buf, MAXLINE)) < 0) {
                    if (errno == ECONNRESET) {
                        printf("client [%d] aborted connection\n", i);
                        close(sockfd);
                        client[i].fd = -1;
                    } else {
                        perror("read error");
                    }
                } else if (n == 0) {
                    printf("client[%d] close connection\n", i);
                    close(sockfd);
                    client[i].fd = -1;
                } else {
                    // Echo received data back to client
                    if (write(sockfd, buf, n) != n) {
                        perror("write error");
                        close(sockfd);
                        client[i].fd = -1;
                    }
                    // Echo received data in the server
                    if (write(1, buf, n) != n)
                    {
                        perror("write error");
                    }
                }
                if (--nready <= 0)
                    break;
            }
        }
    }

    return 0;
}

int main()
{
    std::cout << "webserver" << std::endl;
    runServer();
    return (0);
}