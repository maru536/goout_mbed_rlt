#include "setting.h"

void sendMacResponse(const char* mac_addr, TCPSocket &socket)
{
	socket.send(mac_addr, MAC_ADDR_SIZE);
}

void processWiFiJson(char *buf, char *pwd, char *id) {
	char ssid[MAX_SSID_SIZE] = "";
	int len = strlen(buf);
	int i, j, k;
	for (i = 0; i < strlen(buf); i++) {
		if (buf[i] == '"') {
			if (buf[i + 1] == 'd' && buf[i + 2] == 'e' && buf[i + 3] == 'v'
				&& buf[i + 4] == 'i' && buf[i + 5] == 'c' && buf[i + 6] == 'e'
				&& buf[i + 7] == 'I' && buf[i + 8] == 'd' && buf[i + 9] == '"') {
				i += 10;
				for (j = i; j < i + 5; j++) {
					if (buf[j] == '"') {
						for (k = j + 1; k < len; k++) {
							if (buf[k] == '"') {
								break;
							}

							id[k - j - 1] = buf[k];
						}
						id[k - j - 1] = 0;
						i = k;
						break;
					}
				}
			}
			else if (buf[i + 1] == 'p' && buf[i + 2] == 'w' && buf[i + 3] == 'd' && buf[i + 4] == '"') {
				i += 5;
				for (j = i; j < i + 5; j++) {
					if (buf[j] == '"') {
						for (k = j + 1; k < len; k++) {
							if (buf[k] == '"') {
								break;
							}
							pwd[k - j - 1] = buf[k];
						}
						pwd[k - j - 1] = 0;
						i = k;
						break;
					}
				}
			}
			else if (buf[i + 1] == 's' && buf[i + 2] == 's' && buf[i + 3] == 'i'
				&& buf[i + 4] == 'd' && buf[i + 5] == '"') {
				i += 6;
				for (j = i; j < i + 5; j++) {
					if (buf[j] == '"') {
						for (k = j + 1; k < len; k++) {
							if (buf[k] == '"') {
								break;
							}
							ssid[k - j - 1] = buf[k];
						}
						ssid[k - j - 1] = 0;
						i = k;
						break;
					}
				}
			}
		}
	}

	strcpy(buf, ssid);
}