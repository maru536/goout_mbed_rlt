#define AUDIO_BUF_SIZE 32

#define STA_SSID "U+Net81F3" // Input SSID
#define STA_PASSWORD "40000056191" // Input PW
#define STA_SECURITY NSAPI_SECURITY_WPA2

#define AP_SSID "Wiznet_TestAP" // Input SSID
#define AP_PASSWORD "12345678" // Input PW
#define AP_SECURITY NSAPI_SECURITY_WPA2
#define AP_IP "192.168.12.101"
#define AP_MASK "255.255.255.0"
#define AP_GATEWAY "192.168.12.1"
#define SBUFSIZE 1024
#define MAX_HEADER_SIZE 512
#define MAX_NUMBER_SIZE 20
#define MAX_ID_SIZE (160)
#define MAX_BUF_SIZE 512
#define REQ_DELAY 10000
#define MAC_ADDR_SIZE 18
#define MAX_SSID_SIZE (20)
#define MAX_PWD_SIZE (15)
#define MAX_ID_SIZE (160)


#define IS_ALNUM(ch) \
        ( ch >= 'a' && ch <= 'z' ) || \
        ( ch >= 'A' && ch <= 'Z' ) || \
        ( ch >= '0' && ch <= '9' ) || \
        ( ch >= '-' && ch <= '.' ) 


#define DETECT_SIGNAL (0x1)
#define GOOUT_IP "13.124.228.81"
#define GOOUT_PORT 8080
#define TTS_IP "api.voicerss.org"
#define TTS_PORT 80
