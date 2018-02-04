 /* WiFi Example
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "TCPSocket.h"
#include "TCPServer.h"
#include "AudioPlay.h"
#include "VS1053.h"
#include "WiFiInfo.h"
#include "setting.h"

#define WIFI_ESP8266    1
#define WIFI_IDW0XX1    2

#if TARGET_UBLOX_EVK_ODIN_W2
#include "OdinWiFiInterface.h"
OdinWiFiInterface wifi;

#elif TARGET_REALTEK_RTL8195AM
#include "RTWInterface.h"
RTWInterface wifi;

#else // External WiFi modules

#if MBED_CONF_APP_WIFI_SHIELD == WIFI_ESP8266
#include "ESP8266Interface.h"
ESP8266Interface wifi(MBED_CONF_APP_WIFI_TX, MBED_CONF_APP_WIFI_RX);
#elif MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1
#include "SpwfSAInterface.h"
SpwfSAInterface wifi(MBED_CONF_APP_WIFI_TX, MBED_CONF_APP_WIFI_RX);
#endif // MBED_CONF_APP_WIFI_SHIELD == WIFI_IDW0XX1

#endif

#define TEXT_MODE 0
#define AUDIO_MODE 1
#define STA_MODE 0
#define AP_MODE 1

#define WIFI_MODE AP_MODE
#define MODE TEXT_MODE

int main()
{
	printf("start main\n");
#if MODE == AUDIO_MODE
	VS1053 player(PA_08, PA_07, PA_06, PA_00, PA_05, PA_01, PA_02);
	player.hardwareReset();
	player.modeSwitch();
#endif
	int ret;

#if WIFI_MODE == AP_MODE
	TCPServer srv;
	TCPSocket clt_sock;
	char id[MAX_ID_SIZE] = "";
	char pwd[MAX_PWD_SIZE] = "";
	char *buf = (char*)calloc(MAX_BUF_SIZE, sizeof(char));
	ret = wifi.set_network(AP_IP, AP_MASK, AP_GATEWAY);
	ret = wifi.connectAP(AP_SSID, AP_PASSWORD, AP_SECURITY, 11);

	srv.open(&wifi);
	srv.set_blocking(true);
	srv.bind(8080);
	srv.listen();
	srv.accept(&clt_sock);

	printf("ap server ready\n");

	//Wifi AP Mode Server recv Wifi config info
	while (true)
	{
		int n = clt_sock.recv(buf, MAX_BUF_SIZE);
		//printf("%s", buf);
		if (n < 0)
		{
			clt_sock.close();
			srv.close();
		}
		if (n > 0)
		{
			buf[n] = '\0';
			printf("recv!\n");
			printf("%s\n", buf);
			clt_sock.send(buf, n);
			break;
		}
	}
	printf("recv data!\n");
	//Wifi AP Mode Server response Wifi config info
	sendMacResponse(wifi.get_mac_address(), clt_sock);
	processWiFiJson(buf, pwd, id);

	//free wifi AP Server
	clt_sock.close();
	srv.close();
	free(buf);
#endif

    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    ret = wifi.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error\n");
        return -1;
    }

    printf("Success\n\n");
    printf("MAC: %s\n", wifi.get_mac_address());
    printf("IP: %s\n", wifi.get_ip_address());
    printf("Netmask: %s\n", wifi.get_netmask());
    printf("Gateway: %s\n", wifi.get_gateway());
    printf("RSSI: %d\n\n", wifi.get_rssi());

    //http_demo(&wifi);
	TCPSocket socket;

	for (int i = 0; i < 3; i++) {
		socket.open(&wifi);
#if MODE == TEXT_MODE
		audioPlay(socket);
#endif

#if MODE == AUDIO_MODE
		audioPlay(player, socket);
#endif
	}

    wifi.disconnect();

    printf("\nDone\n");
}
