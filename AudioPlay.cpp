#include "AudioPlay.h"

#define TTS_MODE 0
#define GOOUT_MODE 1

#define PLAY_MODE TTS_MODE

void audioPlay(VS1053 &player, TCPSocket &socket)
{
	int audio_length;
	int play_length;

	if (reqAudio(socket) && extractVoiceData(socket, audio_length)) {
		play_length = player.sendDataBlock(socket, audio_length);
	}

	printf("play audio length: %d\n", play_length);
}

void audioPlay(TCPSocket &socket)
{
	int audio_length;
	int play_length;

	if (reqAudio(socket) && extractVoiceData(socket, audio_length)) {
		play_length = printAudio(socket, audio_length);
	}

	printf("play audio length: %d\n", play_length);
}

int printAudio(TCPSocket &socket, uint16_t audio_length) {
	char rbuffer[256];
	int sum_recv = 0;
	uint16_t response;

	do {
		response = socket.recv(&rbuffer, sizeof rbuffer);
		sum_recv += response;
	} while (response > 0 && sum_recv < audio_length);

	// Close the socket to return its memory and bring down the network interface
	socket.close();

	return sum_recv;
}

bool reqAudio(TCPSocket &socket)
{
	nsapi_error_t response;

	printf("Sending HTTP request to %s...\n", GOOUT_IP);

	//response = socket.connect(GOOUT_IP, GOOUT_PORT);
	response = socket.connect(TTS_IP, TTS_PORT);
	if (0 != response) {
		printf("Error connecting: %d\n", response);
		socket.close();
		return false;
	}

	// Send a simple http request
#if PLAY_MODE == GOOUT_MODE
	char sbuffer[] = "GET /speech?deviceId=ggg&hl=en-us&f=12khz_8bit_mono HTTP/1.1\r\nHost: 13.124.228.81:8080\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";
#else
	char *sbuffer = (char*)calloc(512, sizeof(char));
	char *text = (char*)calloc(256, sizeof(char));
	url_encode(text, "Hello world");
	sprintf(sbuffer, "GET /?key=fb82492d22b541ce8983bbea7d0d984b&hl=en-us&src=%s&c=mp3&f=8khz_8bit_mono&r=1 HTTP/1.1\nHost: api.voicerss.org\n\n", text);
	free(text);
#endif
	nsapi_size_t size = strlen(sbuffer);
	response = 0;
	while (size)
	{
		response = socket.send(sbuffer + response, size);
		if (response < 0) {
			printf("Error sending data: %d\n", response);
			socket.close();
			return false;
		}
		else {
			size -= response;
			// Check if entire message was sent or not
			printf("sent %d\n%s\n", response, sbuffer);
		}
	}


	return true;
}

bool extractVoiceData(TCPSocket &socket, int &content_length) {
	bool isGetLength = false;
	char c;
	content_length = 0;
	char buf[MAX_NUMBER_SIZE] = "";
	while (socket.recv(&c, sizeof(c)) >= 0) {
		if (isGetLength == false && c == '\n') {
			for (int i = 0; i < MAX_HEADER_SIZE; i++) {
				if (socket.recv(&c, sizeof(c)) >= 0 && c == 'C' && socket.recv(&c, sizeof(c)) >= 0 && c == 'o' && socket.recv(&c, sizeof(c)) >= 0 && c == 'n' &&
					socket.recv(&c, sizeof(c)) >= 0 && c == 't' && socket.recv(&c, sizeof(c)) >= 0 && c == 'e' && socket.recv(&c, sizeof(c)) >= 0 && c == 'n' &&
					socket.recv(&c, sizeof(c)) >= 0 && c == 't' && socket.recv(&c, sizeof(c)) >= 0 && c == '-' && socket.recv(&c, sizeof(c)) >= 0 && c == 'L' &&
					socket.recv(&c, sizeof(c)) >= 0 && c == 'e' && socket.recv(&c, sizeof(c)) >= 0 && c == 'n' && socket.recv(&c, sizeof(c)) >= 0 && c == 'g' &&
					socket.recv(&c, sizeof(c)) >= 0 && c == 't' && socket.recv(&c, sizeof(c)) >= 0 && c == 'h' && socket.recv(&c, sizeof(c)) >= 0 && c == ':' &&
					socket.recv(&c, sizeof(c)) >= 0 && c == ' ') {
					for (int j = 0; j < MAX_NUMBER_SIZE; j++) {
						if (socket.recv(&c, sizeof(c)) >= 0 && c == '\n') {
							buf[j] = 0;
							content_length = strToUint16(buf, sizeof(buf));
							isGetLength = true;
							break;
						}
						buf[j] = c;
					}
				}
				else {
					break;
				}
			}
		}

		if (isGetLength == true && c == '\n') {
			for (int i = 0; i < 3; i++) {
				if (socket.recv(&c, sizeof(c)) >= 0 && c == '\n') {
					printf("content_length: %d\n", content_length);
					return true;
				}
			}
		}
	}

	return false;
}

uint16_t strToUint16(char* str, int size)
{
	uint16_t result = 0;
	for (int i = 0; i < size; i++) {
		if (str[i] > '9' || str[i] < '0') {
			break;
		}

		result *= 10;
		result += str[i] - '0';
	}

	return result;
}

void url_encode(char *dst, const char* src) {
	int i, j = 0, len;
	char* tmp;

	len = strlen(src);
	tmp = (char*)malloc((sizeof(char) * 3 * len) + 1);

	for (i = 0; i < len; i++) {
		if (IS_ALNUM(src[i])) {
			tmp[j] = src[i];
		}
		else {
			snprintf(&tmp[j], 4, "%%%02X\n", (unsigned char)src[i]);
			j += 2;
		}
		j++;
	}

	tmp[j] = 0;
	strcpy(dst, tmp);
}