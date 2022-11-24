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
int testbuffer[15];

int nbytes;


int main()
{



//    memcpy(&myPacket[252], &mycrc, 4);


    int cansock;              // дескриптор сокета
    struct sockaddr_can addr; // адресная структура для сокета
    struct ifreq ifr;         // хуй знает что за структура. Похоже что она нужна для получения и хранения индекса интерфейса 


    testbuffer[10] = 25;
    testbuffer[250] = 100;


    int * testbuffer2[100];
    testbuffer2[38] = &testbuffer[10];

    int myvar = *testbuffer2[38];

    printf("test=%d\n", myvar);


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
    myframe.can_id = 0xFFFFE;
    myframe.can_dlc = 8;
    myframe.__pad = 34;
    myframe.__res0 = 228;
    myframe.data[0] = 0xFF;
    myframe.data[1] = 0x86;
    myframe.data[2] = 0x00;
    myframe.data[3] = 0x00;
    myframe.data[4] = 0x00;
    myframe.data[5] = 0x00;
    myframe.data[6] = 0x00;
    myframe.data[7] = 0x00;
    

    struct can_frame out_frame;


    
/*
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
*/

    

    nbytes = write(cansock, &myframe, sizeof(struct can_frame));
    if(nbytes < 0)
    {
        perror("Ошибка при записи в сокет (CAN).");
        return 1;
    }
    else
    printf("Число отправленных в интерфейс байт: %d \n", nbytes);
    printf("Будем получать...\n\n");


    while(1)
    {
        printf("Получение...\n");
        nbytes = read(cansock, &out_frame, sizeof(struct can_frame));
        if(nbytes < 0)
        {
            perror("Ошибка при отправке в сокет (CAN).");
            return 1;
        }

        if(nbytes < sizeof(struct can_frame))
        {
            fprintf(stderr, "Ошибка. Неполный кадр принятого сообщения.\n");
            return 1;
        }



        // вывод принятого кадра
        printf("CAN_ID: %x \n", out_frame.can_id);
        for(int i=0; i<8; i++)
        {
            printf("data[%d]: %x \n", i, out_frame.data[i]);
        }

    }
    


    close(cansock);
    return 0;
} 