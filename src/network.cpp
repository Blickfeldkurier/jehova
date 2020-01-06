#include "network.h"

void Network::init(){
    int opt = 1;

    if( (listenSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if( setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( this->port );

    if (bind(listenSocket, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Listen on port: " << this->port << "\n";

    if (listen(listenSocket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    addrlen = sizeof(address);
}

Network::Network(int port)
{
    this->port = port;
    this->init();
}

void Network::processMsg(std::string message, Canvas *canvas){

    std::vector<std::string> tokens;
    std::istringstream iss(message);
    std::string token;

    while(getline(iss, token, ' ')){
        tokens.push_back(token);
    }

    if(tokens.size() > 0){
        std::string cmd = tokens.at(0);
        if(cmd.compare("PX") == 0 || cmd.compare("px") == 0){
            if(tokens.size() == 4){
                int x = atoi(tokens[1].c_str());
                int y = atoi(tokens[2].c_str());
                std::string rgbColor = tokens[3];
                int value = (int)strtol(rgbColor.c_str(), nullptr, 16);
                int r = (value >> 16) & 0xff;
                int g = (value >> 8) & 0xff;
                int b = (value >> 0) & 0xff;
                canvas->insert(x,y,r,g,b);
            }
        }
    }

}

void Network::tick(Canvas *canvas){
    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(listenSocket, &readfds);
    maxSd = listenSocket;

    //add child sockets to set
    for (unsigned int i = 0 ; i < clientSocket.size() ; i++){
        //socket descriptor
        sd = clientSocket[i];

        //if valid socket descriptor then add to read list
        if(sd > 0){
            FD_SET( sd , &readfds);
        }

        //highest file descriptor number, need it for the select function
        if(sd > maxSd){
            maxSd = sd;
        }

    }

    //wait for an activity on one of the sockets
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    activity = select( maxSd + 1 , &readfds , NULL , NULL , &timeout);

    if ((activity < 0) && (errno!=EINTR))
    {
        std::cout << "select error\n";
    }

    //If something happened on the master socket , then its an incoming connection
    if (FD_ISSET(listenSocket, &readfds)){
        if ((newSocket = accept(listenSocket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
            perror("error on accept");
            exit(EXIT_FAILURE);
        }

        //printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
        //send(new_socket, message, strlen(message), 0) != strlen(message)

        clientSocket.push_back(newSocket);
    }

    //else its some IO operation on some other socket :)
    for (unsigned int i = 0; i < clientSocket.size(); i++){
        sd = clientSocket[i];
        if (FD_ISSET( sd , &readfds)){
            //Check if it was for closing , and also read the incoming message
            char buffer[1025];
            if ((valread = read( sd , buffer, 1024)) == 0){
                //Somebody disconnected , get his details
                //getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);

                //Close the socket and mark as 0 in list for reuse
                close( sd );
                clientSocket.erase(clientSocket.begin() + i);
            }else{ //Process message
                //example: send(sd , buffer , strlen(buffer) , 0 );
                this->processMsg(std::string(buffer), canvas);
            }
        }
    }
}
