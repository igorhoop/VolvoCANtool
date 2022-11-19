/*
UDP-клиент
*/

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h> // расположение /usr/include/x86_64-linux-gnu/sys/
#include <netdb.h> // этот хедер ради типа sockaddr_in, потому что в socket.h тлько обычный sockaddr
#include <arpa/inet.h> // это ради функции inet_addr(), которая преобразует символьный IP-адрес в сетевой с нужным порядком байт
#include <cstring>

#include <fstream>
#include <cstdio>

char * recvbuf = new char[1024];
char * sendbuf;

char * readfilebuf = new char[1024];

int bytes_read; // количество принятый байт
char msg1[] = "Hello there!\n";
char msg2[] = "Bye bye!\n";

int timer = 0;

int main()
{

//    memcpy(&myPacket[252], &mycrc, 4);

    char myPacket2[] = {};


    int sock, sock_upboard, sock_stm;
    struct sockaddr_in addr;
    struct sockaddr_in addr_upboard;
    struct sockaddr_in addr_stm;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }



    addr.sin_family = AF_INET;
    addr.sin_port = htons(8007);
    addr.sin_addr.s_addr = inet_addr("192.168.123.111");
    bind(sock, (sockaddr *) &addr, sizeof(addr));

    addr_stm.sin_family = AF_INET;
    addr_stm.sin_port = htons(8007);
    addr_stm.sin_addr.s_addr = inet_addr("192.168.123.10");

    sock_stm = socket(AF_INET, SOCK_DGRAM, 0);


    // перехватываем трафик на себя отправкой хуйни
    //sendto(sock_stm, "ttt", 3, 0, (struct sockaddr *)&addr_stm, sizeof(addr_stm));



    sock_upboard = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        perror("socket upboard");
        exit(2);
    }


    addr_upboard.sin_family = AF_INET;
    addr_upboard.sin_port = htons(8082);
    addr_upboard.sin_addr.s_addr = inet_addr("192.168.123.161");

    //sendto(sock, myPacket, sizeof(myPacket), 0, (struct sockaddr *)&addr, sizeof(addr));
    
    while(1)
    {
        //sendto(sock, myPacket, sizeof(myPacket), 0, (struct sockaddr *)&addr, sizeof(addr));
        //sleep(1);


        //bytes_read = recvfrom(sock_stm, recvbuf, 1024, 0, NULL, NULL);
        //printf("Кол-во принятых байт: %d \t", bytes_read);

        //sendbuf = recvbuf;

        

        std::ifstream f1("../switch_pack_brakosha", std::ios::binary);
        //f1.write(recvbuf, 820);
        f1.read(readfilebuf, 129);
        f1.close();

        sendto(sock, readfilebuf, 129, 0, (struct sockaddr *)&addr_upboard, sizeof(addr_upboard));


    }
    //connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    //send(sock, msg2, sizeof(msg2), 0);
    
    close(sock);

    return 0;
} 