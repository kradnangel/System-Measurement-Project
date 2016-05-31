//
//  main.cpp
//  Network_measurement
//
//  Created by Kun on 5/29/16.
//  Copyright (c) 2016 Kun. All rights reserved.
//

#include <iostream>
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
#define MAXBUF 1024
#define PACKAGE_SIZE 56
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

double standard_deviation(double data[], int n)
{
    float mean=0.0, sum_deviation=0.0;
    int i;
    for(i=0; i<n;++i)
    {
        mean+=data[i];
    }
    mean=mean/n;
    for(i=0; i<n;++i)
        sum_deviation+=(data[i]-mean)*(data[i]-mean);
    return sqrt(sum_deviation/n);
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
    
    bzero(buf, MAXBUF + 1);
    
    for ( int i = 0; i < PACKAGE_SIZE ; i ++) {
        buf[i] = '0' + i%10;
        if ( (i + 1) % 10 ==0) {
            buf[i] = 'x';
        }
    }
    
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
    
    double dataPoint[SAMPLES];
    double totalTime = 0;
    for (int i = 0; i < SAMPLES; i++) {
        //start
        start = rdtsc();
        len = send(sockfd, buf, strlen(buf), 0);
        if (len > 0)
            printf("msg:%s send successful Totalbytes: %ld\n", buf, len);
        else {
            printf("msg:'%s  failed!\n", buf);
            continue;
        }
        
        printf("Start receiving echo message\n");
        bzero(recBuf, MAXBUF + 1);
        
        long tempRec = 0;
        while (tempRec != PACKAGE_SIZE) {
            len = recv(sockfd, recBuf, MAXBUF, 0);
            if (len > 0)   {
                printf("recv:'%s, total: %ld \n", buf, len);
                tempRec = tempRec + len;
            }
            else
            {
                if (len < 0)
                    printf("recv failed rrno:%d error msg: '%s'\n", errno, strerror(errno));
                else
                    printf("other exit erminal chat\n");
                
                continue;
            }
        }
        
        end = rdtsc();
        
        rawTime = (end - start)/FREQUENCE*1000;
        totalTime += rawTime;
        if(rawTime > maxTime)
            maxTime = rawTime;
        if(rawTime < minTime)
            minTime = rawTime;
        dataPoint[i] = rawTime;
        cout << "Time: " << rawTime << endl;
    }
    
    printf("\nTCP round-trip min/avg/max/stddev avgTime is  %f/%f/%f/%f ms\n",minTime, (totalTime/SAMPLES),maxTime,standard_deviation(dataPoint,SAMPLES));
    close(sockfd);
    return 0;
}
