//
// Created by emre on 03.12.2016.
//

#ifndef PROJE_COMMUNICATION_H
#define PROJE_COMMUNICATION_H

#include "SerialPort.h"
#include <cstdio>
#include <iostream>
#include <zconf.h>
#include <string>

#define START_CHAR '{'
#define END_CHAR '}'
#define PACKET_SIZE 25
#define X_MOTOR_START 'X'
#define Y_MOTOR_START 'Y'
#define BALL_X_COORDINATE 'x'
#define BALL_Y_COORDINATE 'y'
#define XCOORDMMAX 940
#define XCOORDMIN 140
#define YCOORDMMAX 960
#define YCOORDMIN 130
#define XOUTMAX 400
#define XOUTMIN 0
#define YOUTMAX 400
#define YOUTMIN 0

using namespace std;

/**
 * Serial Communication
 */
class Communication {
private :
    bool communicationReady ;
    int XMotorAngle ;
    int YMotorAngle;
    int ballXCoordinate;
    int ballYCoordinate;
    SerialPort *port;
    bool makeHandShake();
    bool checkConnection();
    inline void setXMotorAngle(int angle){ XMotorAngle = angle; }
    inline void setYMotorAngle(int angle){YMotorAngle = angle; }
    inline void setBallXCoordinate(int x){ballXCoordinate = map(x,XCOORDMIN,XCOORDMMAX,XOUTMIN,XOUTMAX);} //TODO : Map edilmiş olacak
    inline void setBallYCoordinate(int y){ballYCoordinate = map(y,YCOORDMIN,YCOORDMMAX,YOUTMIN,YOUTMAX);} //Arduino dan alınacak
    inline float map(long x, long in_min, long in_max, long out_min, long out_max)
    {
        return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
    }
public:
    /**
     * Constructor
     * str : Serail port name
     * baud : baud rate
     */
    Communication(string str,SerialPort::BaudRate baud);
    /**
     * Attention: received data should be like ("X%3.2lfY%3.2lfx%dy%d!",double ,double,int,int)
     * Reads the serial port and assigns the motor angles
     */
    //bool read();

    bool readUntil();

    bool isCommunicationReady() const {
        return communicationReady;
    }

    int getXMotorAngle() const {
        return XMotorAngle;
    }


    int getYMotorAngle() const {
        return YMotorAngle;
    }

    int getBallXCoordinate() const {
        return ballXCoordinate;
    }

    int getBallYCoordinate() const {
        return ballYCoordinate;
    }

    bool write(char *msg);

    bool write(char ch);

};


#endif //PROJE_COMMUNUCATİON_H
