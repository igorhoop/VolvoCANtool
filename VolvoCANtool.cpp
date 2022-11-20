/* ===========================

ПРОГРАММА ДЛЯ ВЗАИМОДЕЙСТВИЯ С CAN

============================= */

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h> // расположение /usr/include/x86_64-linux-gnu/sys/
#include <netdb.h> // этот хедер ради типа sockaddr_in, потому что в socket.h тлько обычный sockaddr
#include <arpa/inet.h> // это ради функции inet_addr(), которая преобразует символьный IP-адрес в сетевой с нужным порядком байт
#include <cstring>
#include <linux/can.h>        // этот хедер ради возможности создать сокет с семейством протокола PF_CAN, ради типов frame_can, sockaddr_can
#include <fstream>
#include <cstdio>
#include <net/if.h>           // этот хедер для того чтоб можно было юзать структуру ifreq
#include <sys/ioctl.h>        // этот хедер нужен для SIOCGIFINDEX


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


    int cansock;              // дескриптор сокета
    struct sockaddr_can addr; // адресная структура для сокета
    struct ifreq ifr;         // хуй знает что за структура. Похоже что она нужна для получения и хранения индекса интерфейса 





    cansock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(cansock < 0)
    {
        perror("Какая-то проблема при создании CAN-сокета (!) (дескриптор сокета меньше нуля)");
        exit(1);
    }

    strcpy(ifr.ifr_name, "can0" );          // готовим объект ifr под заполнение ??
    ioctl(cansock, SIOCGIFINDEX, &ifr);     // заполняем объект ift на основе текстового имени can0 ???

    addr.can_family = AF_CAN;               // заполнение адресной структуры. Здесь мы можем получить индекс интерфейса благодаря тому 
    addr.can_ifindex = ifr.ifr_ifindex;     // что объект ifr уже заполнен. Индекс интерфейса может быть 0, если нужно привязаться ко всем CAN-интерфейсам


    bind(cansock, (sockaddr *) &addr, sizeof(addr));
    printf("Индекс интерфейса для привязки: %x \n", ifr.ifr_ifindex);



    struct can_frame myframe;
    myframe.can_id = 0xAA;
    myframe.can_dlc = 8;
    myframe.__pad = 34;
    myframe.__res0 = 228;
    myframe.data[0] = 100;
    myframe.data[1] = 100;
    myframe.data[2] = 100;
    myframe.data[3] = 100;
    myframe.data[4] = 100;
    myframe.data[5] = 100;
    myframe.data[6] = 100;
    myframe.data[7] = 100;
    myframe.data[8] = 100;

    
    
    printf("Can_id: %d \n", myframe.can_id);
    printf("can_dlc: %d \n", myframe.can_dlc);
    printf("data[0]: %d \n", myframe.data[0]);
    printf("data[1]: %d \n", myframe.data[1]);
    printf("data[2]: %d \n", myframe.data[2]);
    printf("data[3]: %d \n", myframe.data[3]);
    printf("data[4]: %d \n", myframe.data[4]);
    printf("data[5]: %d \n", myframe.data[5]);
    printf("data[6]: %d \n", myframe.data[6]);
    printf("data[7]: %d \n", myframe.data[7]);
    printf("Размер: %d \t", (int) sizeof(canid_t));


    int nbytes = write(cansock, &myframe, sizeof(struct can_frame));
    printf("Число отправленных в интерфейс байт: %d \n", nbytes);





    exit(1);
    


    
    while(1)
    {
        //sendto(sock, myPacket, sizeof(myPacket), 0, (struct sockaddr *)&addr, sizeof(addr));
        //sleep(1);


        //bytes_read = recvfrom(sock_stm, recvbuf, 1024, 0, NULL, NULL);
        //printf("Кол-во принятых байт: %d \t", bytes_read);

        //sendbuf = recvbuf;

        

        //std::ifstream f1("../switch_pack_brakosha", std::ios::binary);
        //f1.write(recvbuf, 820);
        //f1.read(readfilebuf, 129);
        //f1.close();

        //sendto(sock, readfilebuf, 129, 0, (struct sockaddr *)&addr_upboard, sizeof(addr_upboard));


    }
    //connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    //send(sock, msg2, sizeof(msg2), 0);
    


    close(cansock);
    return 0;
} 