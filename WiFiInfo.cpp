#include "WiFiInfo.h"


void recvInfo(TCPSocket *socket, char* buf, char* id, ShiftRegisterControlClass led) 
{
    socket->connect(GOOUT_IP, GOOUT_PORT);
    sprintf(buf, "GET /led_sentence?deviceId=%s HTTP/1.1\nHost: 1\n\n", id);

    socket->send(buf, MAX_BUF_SIZE);
    getInfoState(socket, buf);
    processInfo(buf, led);
    printf("\n%s\n", buf);
}


void getInfoState(TCPSocket *socket, char *buf) {
  char c;
  bool isStartContent = false;
  bool isEndContent = false;
  int bracketCount = 0;
  int pos = 0;
  while (socket->recv(&c, sizeof(c)) >= 0 && isEndContent == false) {

    if (!isStartContent) {
      if (c == '{') {
        bracketCount++;
        isStartContent = true;
        buf[pos++] = c;
      }
    }
    else {
      // you got two newline characters in a row
      // that's the end of the HTTP request, so send a response
      if (bracketCount != 0) {
        if (c == '{') {
          bracketCount++; 
        } 
        else if (c == '}') {
          bracketCount--;
        }
        
        buf[pos++] = c;
        if (bracketCount == 0) {
          buf[pos++] = 0;
          isEndContent = true;
        }
      }
    }
  }
}

uint8_t processInfo(char *buf, ShiftRegisterControlClass led)
{
  bool first, second, third;
  uint8_t ledByte = 0;
  
  led.TurnOffLed(led.kIndiAByte);
  led.TurnOffLed(led.kIndiBByte);
  led.TurnOffLed(led.kIndiCByte);

  processJson(&first, &second, &third, buf);

  if (first) {
    led.TurnOnLed(led.kIndiAByte);
    //Serial.println(F("First On"));
    ledByte |= 1 << 5;
  }

  if (second) {
    led.TurnOnLed(led.kIndiBByte);
    //Serial.println(F("Second On"));
    ledByte |= 1 << 4;
  }

  if (third) {
    led.TurnOnLed(led.kIndiCByte);
    //Serial.println(F("Third On"));
    ledByte |= 1 << 3;
  }

  //Serial.println(buf);
  
  return ledByte;
}


void processJson(bool* first, bool* second, bool* third, char *buf) {
  char id[MAX_ID_SIZE] = "";
  int len = strlen(buf);
  int i, j, k;
  for (i = 0; i < strlen(buf); i++) {
    if (buf[i] == '"') {
      if (buf[i + 1] == 'f' && buf[i + 2] == 'i' && buf[i + 3] == 'r'
       && buf[i + 4] == 's' && buf[i + 5] == 't' && buf[i + 6] == 'L'
        && buf[i + 7] == 'E' && buf[i + 8] == 'D' && buf[i + 9] == '"') {
        i += 10;
        for (j = i; j < i + 5; j++) {
          if (buf[j] == 't') {
            *first = true;
            i = j + 4;
            break;
          }
          else if (buf[j] == 'f') {
            *first = false;
            i = j + 5;
            break;
          }
        }
      } 
      else if (buf[i + 1] == 's' && buf[i + 2] == 'e' && buf[i + 3] == 'c'
       && buf[i + 4] == 'o' && buf[i + 5] == 'n' && buf[i + 6] == 'd'
        && buf[i + 7] == 'L' && buf[i + 8] == 'E' && buf[i + 9] == 'D' && buf[i + 10] == '"') {
        i += 11;
        for (j = i; j < i + 5; j++) {
          if (buf[j] == 't') {
            *second = true;
            i = j + 4;
            break;
          }
          else if (buf[j] == 'f') {
            *second = false;
            i = j + 5;
            break;
          }
        }
      } 
      else if (buf[i + 1] == 't' && buf[i + 2] == 'h' && buf[i + 3] == 'i'
       && buf[i + 4] == 'r' && buf[i + 5] == 'd' && buf[i + 6] == 'L'
        && buf[i + 7] == 'E' && buf[i + 8] == 'D' && buf[i + 9] == '"') {
        i += 10;
        for (j = i; j < i + 5; j++) {
          if (buf[j] == 't') {
            *third = true;
            i = j + 4;
            break;
          }
          else if (buf[j] == 'f') {
            *third = false;
            i = j + 5;
            break;
          }
        }
      } 
      else if (buf[i + 1] == 's' && buf[i + 2] == 'e' && buf[i + 3] == 'n'
       && buf[i + 4] == 't' && buf[i + 5] == 'e' && buf[i + 6] == 'n'
        && buf[i + 7] == 'c' && buf[i + 8] == 'e' && buf[i + 9] == '"') {
        i += 10;
        for (j = i; j < i + 5; j++) {
          if (buf[j] == '"') {
            for (k = j + 1; k < len; k++) {
              if (buf[k] == '"') {
                break;
              }
              if (buf[k] == ',') {
                id[k - j - 1] = ' ';
              }
              else {
                id[k - j - 1] = buf[k];
              }
            }
            id[k - j - 1] = 0;
            break;
          }
        }
      }
    }
  }

  strcpy(buf, id);
}