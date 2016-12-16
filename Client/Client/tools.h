#ifndef TOOL_H
#define TOOL_H
#include <fcntl.h>
#include<iostream>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include "Communication.h"

#define PORTNAMESIZE 30
#define THREADCOMSIZE 6
#define NONE "none"
#define PRESSED "pres"
#define READY "ready"
#define QUIT "quit"
#define FIFONAME "fifo"
#define EXEADDRESS "3dsim.exe"
#define Packet_format "{ %3.2lf %3.2lf %3d %3d}"

static bool isButtonPressed; // uses Unreal Button pressed
/*
            Thread (grafik çizme)
              ^
        pipe  |
              |
              |    Serial
    Host-> Thread -----> Arduiono
              |
              |pipe
              |
             \ /    fifo
            Thread -----> Unreal Exe
                            */
namespace Tool {

    //TODO:
    /*Maybe Missing something*/
    struct threadMessageArdu{
        char portname[PORTNAMESIZE];
    };

    //TODO:
    /*Maybe Missing something*/
    struct threadMessage3D{
        int pipefd[2];
    };

    //TODO:
    /*Maybe Missing something*/
    struct threadMessageGrafik {
        int pipefd[2];
    };

    struct InfoPacket{
        int ballX;
        int ballY;
        float motorXangle;
        float motorYangle;
    };

    void* CommunicateWithArduino(void* message);
    void* CommunicateWith3DSim(void* message);
    void* CommunicateWithGrafic(void* message);
}
#endif // TOOL_H
