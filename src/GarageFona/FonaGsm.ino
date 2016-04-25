#include "Adafruit_FONA.h"
#include <SoftwareSerial.h>

char sim_pin[] =  "8073";
char domain[] = "sogetiparking.azurewebsites.net";
uint8_t response[200];
byte ConnectionErrorCounter = 0;

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4
#define FONA_PWR 5

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

void TogglePowerFona()
{
  Serial.println("Power toogle FonaGsmModule");
  digitalWrite(FONA_PWR, LOW);
  delay(2500);
  digitalWrite(FONA_PWR, HIGH);
  delay(2500);
}

void FonaHardReset()
{  
  Serial.println("Resetting FonaGsmModule");
  digitalWrite(FONA_RST, LOW);
  delay(200);
  digitalWrite(FONA_RST, HIGH);

  TogglePowerFona();
}

void InitFonaGsm()
{  
  pinMode(FONA_RST, OUTPUT);
  digitalWrite(FONA_RST, HIGH);

  pinMode(FONA_PWR , OUTPUT);
  digitalWrite(FONA_PWR, HIGH);

  TogglePowerFona();
  
  fonaSerial->begin(9600);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    TogglePowerFona();
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
    fona.setGPRSNetworkSettings(F("online.telia.se"), F(""), F(""));
    fona.enableGPRS(true);
}

bool ConnectToServer()
{
  fona.sendCheckReply(F("AT+SAPBR=1,1"), F("OK"), 10000);
  if(fona.TCPconnect(domain, 80))
  { ConnectionErrorCounter = 0;
    return true;
  }
  else
  {
    if(ConnectionErrorCounter++ > 3);
    {       
       FonaHardReset();
       InitFonaGsm();
       return false;
    }
  }
}

bool DisconnectFromServer()
{
  if (! fona.TCPclose()) return false;
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
