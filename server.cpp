#include "header.hpp"

int runServer()
{
    //create socket
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);

    //set up address settings
    sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    //bind socket and listen
    bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSock, 10);

    //accept loop
    while (true)
    {
        int clientSock = accept(serverSock, NULL, NULL);

        char buff[4096] = {0};
        recv(clientSock, buff, sizeof(buff), 0);
        std::cout << "Message from client: " << buff << std::endl;
        close(clientSock);
    }

    close(serverSock);
    return (0);
}