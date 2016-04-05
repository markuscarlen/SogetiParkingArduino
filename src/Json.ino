#include "avr/pgmspace.h"
const char header[] PROGMEM = "PUT /api/sensor HTTP/1.1\r\nHost: sogetiparking.azurewebsites.net\r\nContent-Type: application/json\r\nContent-Length: ";

void CreateJsonRequest(int parkingSpaceNumber, bool IsFree)
{
  stringToSend[0] = 0;
  char body[38];
  sprintf(body, "{'SpaceNumber':'%d','IsFree':",parkingSpaceNumber);
  
  if(IsFree) strcat(body, "'true'}");
  else strcat(body, "'false'}");
  
  strcpy_P(stringToSend, header);
  char s[10];
  sprintf(s, "%d", strlen(body));
  
  strcat(stringToSend, s);
  strcat(stringToSend, "\r\n\r\n");
  strcat(stringToSend, body);
}
  
