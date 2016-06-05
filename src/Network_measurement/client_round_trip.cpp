//
//  main.cpp
//  Network_measurement
//
//  Created by Kun on 5/29/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

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

#include "helper.h"

#define FREQUENCE 2.4e9
#define MAXBUF 1024
#define MAX_PACKAGE_SIZE 1024
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
    char buf[MAXBUF + 1];
    char recBuf [MAXBUF +1];
    
    double  start;
    double  end;
    double  rawTime;
    
    double minTime = 1000000;
    double maxTime = -100000;
    
    ofstream fout;
    fout.open ("result/round_trip.csv");
    fout << "Package size, min(ms), max(ms), avg(ms), std(ms)" << endl;
    
    bzero(buf, MAXBUF + 1);
    for (int i = 0; i < MAX_PACKAGE_SIZE ; i ++)
        buf[i] = 'x';
    
    if (argc != 3) {
        cout << "Please input IP number and port number" << endl;
        exit(-1);
    }
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(errno);
    }
    
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0) {
        perror(argv[1]);
        exit(errno);
    }
    
    if(connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
        perror("Connection failed");
        exit(errno);
    }
    
    printf("connect to server...\n");
    
    double times[SAMPLES];
    double totalTime = 0;
    for (int package_size = 1; package_size <= MAX_PACKAGE_SIZE; package_size *= 2)
    {
        for (int i = 0; i < SAMPLES; i++) {
            //start
            start = rdtsc();
            len = send(sockfd, buf, package_size, 0);
//            if (len > 0)
//                printf("msg send successful Totalbytes: %ld\n", len);
//            else {
//                printf("msg failed!\n");
//                continue;
//            }
//            
//            printf("Start receiving echo message\n");
            
            len = recv(sockfd, recBuf, MAXBUF, 0);
//            if (len > 0)   {
//                printf("recv total: %ld \n", len);
//            }
            end = rdtsc();
            
            rawTime = (end - start)/FREQUENCE*1000;
            totalTime += rawTime;
            if(rawTime > maxTime)
                maxTime = rawTime;
            if(rawTime < minTime)
                minTime = rawTime;
            times[i] = rawTime;
            cout << "Time: " << rawTime << endl;
        }
        fout << package_size << ", " << min(times, SAMPLES) << ", " << max(times, SAMPLES) << ", " << average(times, SAMPLES) << ", " << standard_deviation(times, SAMPLES) << endl;
    }
    
    close(sockfd);
    return 0;
}
