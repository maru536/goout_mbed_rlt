#pragma once
#include "TCPServer.h"
#include "resource.h"
#include "ShiftRegisterControlClass.h"

void getInfoState(TCPSocket *socket, char *buf);
uint8_t processInfo(char *buf, ShiftRegisterControlClass led);
void processJson(bool* first, bool* second, bool* third, char *buf);
void recvInfo(TCPSocket *socket, char* buf, char* id, ShiftRegisterControlClass led);