#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

char sim_pin[] =  "8073";
char domain[] = "sogetiparking.azurewebsites.net";
uint8_t response[200];

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void InitFonaGsm()
{  
  fonaSerial->begin(9600);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
  }

  UnlockSim();
  delay(5000);  
  
  EnableGprs();
}

void GetIpAddress()
{
  fonaSerial->println("AT+CIFSR");
  delay(1000);
  while(fonaSerial->available())
  {
    Serial.write(fonaSerial->read());
  }  
}

void UnlockSim()
{ 
    fona.unlockSIM(sim_pin);
}

bool GetNetworkStatus()
{
  return fona.getNetworkStatus();
}

void EnableGprs()
{
    //fona.sendCheckReply(F("AT+CIPSHUT"), F("SHUT OK"), 20000);
    //fona.sendCheckReply(F("AT+CGATT=1"), F("OK"), 10000);
    //fona.sendCheckReply(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""),F("OK"), 10000);
    //fona.sendCheckReply(F("AT+SAPBR=3,1,\"APN\",\"online.telia.se\""), F("OK"), 10000);
    //fona.sendCheckReply(F("AT+SAPBR=1,1"), F("OK"), 10000);  

    fona.setGPRSNetworkSettings(F("online.telia.se"), F(""), F(""));
    fona.enableGPRS(true);
}

bool ConnectToServer()
{
  fona.sendCheckReply(F("AT+SAPBR=1,1"), F("OK"), 10000);
  return fona.TCPconnect(domain, 80);
}

bool DisconnectFromServer()
{
  if (! fona.TCPclose()) return false;
  //fona.sendCheckReply(F("AT+CIPCLOSE=0"), F("CLOSE OK"), 10000);
  return fona.sendCheckReply(F("AT+SAPBR=0,1"), F("OK"), 10000);

}

bool SendDataToServer()
{
  digitalWrite(LED, HIGH);
  Serial.println(stringToSend);
  fonaSerial->flush();
  fonaSerial->print("AT+CIPSEND=");
  fonaSerial->println(strlen(stringToSend));
  delay(1000);
  while(fonaSerial->available())
  {
    while(fonaSerial->available())
    {
      Serial.write(fonaSerial->read());
    }
    delay(200);
  }
  fonaSerial->write(stringToSend, strlen(stringToSend));
  delay(5000);
  
  fonaSerial->println("AT+CIPRXGET=2, 500");  
  while(fonaSerial->available())
  {
    while(fonaSerial->available())
    {
      Serial.write(fonaSerial->read());
    }
    delay(200);
  }
  digitalWrite(LED, LOW);
}
