//
//  server_bandwidth.cpp
//  Network_measurement
//
//  Created by Kun on 5/31/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include "server_bandwidth.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>

#include <fstream>
#include <iostream>

#define MAX_PACKAGE_SIZE (1024*1024*128)

using namespace std;

typedef struct CLIENT {
    int fd;
    struct sockaddr_in addr;
}CLIENT;

/***************************
 **server for multi-client
 **PF_SETSIZE=1024
 ****************************/
int main(int argc, char** argv)
{
    int i,maxi = -1;
    long n;
    int nready;
    int slisten,sockfd,maxfd=-1,connectfd;
    
    unsigned int myport,lisnum;
    
    struct sockaddr_in  my_addr,addr;
    struct timeval tv;
    
    socklen_t len;
    fd_set rset,allset;
    
    char *buf = (char*)malloc(sizeof(char)*MAX_PACKAGE_SIZE);
    CLIENT client[FD_SETSIZE];
    
    if(argv[1])
        myport = atoi(argv[1]);
    else
        myport = 9999;
    
    if(argv[2])
        lisnum = atoi(argv[2]);
    else
        lisnum = FD_SETSIZE;
    
    if((slisten = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    
    //set socket NO TIME_WAIT
    int on=1;
    if((setsockopt(slisten,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)
        
    {
        perror("setsocketopt fail");
        std::cout<<std::endl;
        exit(1);
    }
    
    bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    
    if(bind(slisten, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        exit(1);
    }
    
    if (listen(slisten, lisnum) == -1) {
        perror("listen");
        exit(1);
    }
    
    for(i=0;i<FD_SETSIZE;i++)
    {
        client[i].fd = -1;
    }
    
    FD_ZERO(&allset);
    FD_SET(slisten, &allset);
    maxfd = slisten;
    
    printf("Waiting for connections and data...\n");
    while (1)
    {
        rset = allset;
        
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        
        nready = select(maxfd + 1, &rset, NULL, NULL, &tv);
        
        if(nready == 0)
            continue;
        else if(nready < 0)
        {
            printf("select failed!\n");
            break;
        }
        else
        {
            if(FD_ISSET(slisten,&rset)) // new connection
            {
                len = sizeof(struct sockaddr);
                if((connectfd = accept(slisten, (struct sockaddr*)&addr,&len)) == -1)
                {
                    perror("accept() failed\n");
                    continue;
                }
                for(i = 0; i < FD_SETSIZE; i++)
                {
                    if(client[i].fd < 0)
                    {
                        client[i].fd = connectfd;
                        client[i].addr = addr;
                        printf("Yout got a connection from %s.\n",
                               inet_ntoa(client[i].addr.sin_addr));
                        break;
                    }
                }
                if(i == FD_SETSIZE)
                    printf("too many connections");
                FD_SET(connectfd,&allset);
                if(connectfd > maxfd)
                    maxfd = connectfd;
                if(i > maxi)
                    maxi = i;
            }
            else
            {
                for(i = 0;i <= maxi;i++)
                {
                    if((sockfd = client[i].fd)<0)
                        continue;
                    if(FD_ISSET(sockfd,&rset))
                    {
                        long received_n = 0;
                        int package_size = 0;
                        if ((n = recv(sockfd, buf, MAX_PACKAGE_SIZE, 0)) == sizeof(package_size))
                        {
                            package_size = *(int*)buf;
                            cout << "Got package size: " << package_size << endl;
                            send(sockfd, "OK", strlen("OK"), 0);
                        }
                        else
                        {
                            cout << "Unable to get package info" << endl;
                            send(sockfd, "failed", strlen("failed"), 0);
                            continue;
                        }
                        while (received_n < package_size) {
                            if((n = recv(sockfd, buf, package_size, 0)) > 0) {
                                received_n += n;
                                printf("received data from %s size: %lu\n, total size: %lu\n",inet_ntoa(client[i].addr.sin_addr), n, received_n);
                                
                            }
                            else {
                                break;
                            }
                        }
                        if(received_n == package_size) {
                            cout<<"received whole package"<<endl;
                            write(sockfd, "OK", strlen("OK"));
                        }
                        else {
                            cout<<"received " << double(received_n)/package_size << "% package"<<endl;
                            write(sockfd, "Failed", strlen("Failed"));
                            printf("disconnected by client!\n");
                            close(sockfd);
                            FD_CLR(sockfd,&allset);
                            client[i].fd = -1;
                        }
                    }
                }
            }
        }
    }
    close(slisten);
}