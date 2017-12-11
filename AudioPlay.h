#pragma once
#include "TCPSocket.h"
#include "VS1053.h"
#include "resource.h"

uint16_t strToUint16(char* str, int size);
bool extractVoiceData(TCPSocket &socket, int &content_length);
void audioPlay(VS1053 &player, TCPSocket &socket);
void audioPlay(TCPSocket &socket);
int printAudio(TCPSocket &socket, uint16_t content_length);
bool reqAudio(TCPSocket &socket);
void url_encode(char *dst, const char* src);