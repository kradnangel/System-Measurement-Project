//
//  client_connection_overhead.cpp
//  Network_measurement
//
//  Created by Kun on 5/31/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include "client_connection_overhead.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

#define FREQUENCE 2.4e9
#define PACKAGE_SIZE (1024*1024*1)
#define MAXBUF 1024
#define SAMPLES 100

using namespace std;

static inline uint64_t rdtsc(){
    //    unsigned int lo,hi;
    //    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    //    return ((uint64_t)hi << 32) | lo;
    
    uint64_t tsc;
    __asm__ __volatile__("rdtscp; "         // serializing read of tsc
                         "shl $32,%%rdx; "  // shift higher 32 bits stored in rdx up
                         "or %%rdx,%%rax"   // and or onto rax
                         : "=a"(tsc)        // output to tsc variable
                         :
                         : "%rcx", "%rdx"); // rcx and rdx are clobbered
    return tsc;
}

int main(int argc, char **argv)
{
    int sockfd;
    long len;
    struct sockaddr_in dest;
    char *buf = (char*)malloc(sizeof(char)*PACKAGE_SIZE);
    char recBuf [MAXBUF +1];
    
    double  start;
    double  end;
    double  setup_time, teardown_time;

    ofstream fout;
    fout.open ("result/connection.csv");
    fout << "Set up(ms), Tear down(ms)" << endl;

    if (argc != 3) {
        cout << "Please input IP number and port number" << endl;
        exit(-1);
    }
    
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(argv[2]));

    if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0) {
        perror(argv[1]);
        exit(errno);
    }

    for (int i = 0; i < SAMPLES; i++) {
        start = rdtsc();
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Socket creation failed");
            exit(errno);
        }
        if(connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
            perror("Connection failed");
            exit(errno);
        }
        end = rdtsc();
        setup_time = (end - start)/FREQUENCE*1000;
        
        start = rdtsc();
        close(sockfd);
        end = rdtsc();
        teardown_time = (end - start)/FREQUENCE*1000;
        
        cout << "Setup time: " << setup_time << endl;
        cout << "Teardown time: " << teardown_time << endl;
        
        fout << setup_time << ", " << teardown_time << endl;
    }
    return 0;
}
