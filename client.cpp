#include "header.hpp"

int runClient()
{
    //socket
    int clientSock = socket(AF_INET, SOCK_STREAM, 0);

    //config addr
    sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(7000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    //connect
    connect(clientSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    
    //send
    const char * msg = "Hello , server!";
    send(clientSock, msg, strlen(msg), 0);

    close(clientSock);
    return (0);
}

int main()
{
    std::cout << "webserver" << std::endl;
    runClient();
    return (0);
}