#pragma once
#include "TCPSocket.h"
#include "resource.h"

void sendMacResponse(const char* mac_addr, TCPSocket &socket);
void processWiFiJson(char *buf, char *pwd, char *id);