#include "ParkingSpace.h"

#define LED 13

char stringToSend[160];

ParkingSpace space51(51, 7, 8, 150);
ParkingSpace space52(52, 9, 10, 150);
ParkingSpace space53(53, 11, 12, 150);

unsigned long ul_PreviousMillis = 0UL;
unsigned long ul_Interval = 600000UL; //Interval 10min (10*60*1000)

void setup()
{
  pinMode(LED, OUTPUT); //LED for showing sending to server.

  InitSerialPort();
  InitFonaGsm();

  SendTimeIntervalStatusUpdate();
  ul_PreviousMillis = millis();
}

void loop()
{ 
  delay(500);
  Serial.println("Space 51:");
  if(space51.PingAndReturnStatusChanged()) SendData(space51.GetSpaceNumber(), space51.IsFree());
  Serial.println("Space 52:");
  if(space52.PingAndReturnStatusChanged()) SendData(space52.GetSpaceNumber(), space52.IsFree());
  Serial.println("Space 53:");
  if(space53.PingAndReturnStatusChanged()) SendData(space53.GetSpaceNumber(), space53.IsFree());

  if(IntervalTimerElapsed()) SendTimeIntervalStatusUpdate();

}

void SendData(int spaceNr, bool isFree)
{
  if(ConnectToServer()) Serial.println(F("Con OK"));
  else Serial.println(F("Con not OK"));
  CreateJsonRequest(spaceNr, isFree);
  SendDataToServer();
  DisconnectFromServer();    
}

void SendTimeIntervalStatusUpdate()
{
  ConnectToServer();
  Serial.println("Space 51:");
  CreateJsonRequest(space51.GetSpaceNumber(), space51.PingAndReturnFreeStatus());
  SendDataToServer();
  Serial.println("Space 52:");
  CreateJsonRequest(space52.GetSpaceNumber(), space52.PingAndReturnFreeStatus());
  SendDataToServer();
  Serial.println("Space 53:");
  CreateJsonRequest(space53.GetSpaceNumber(), space53.PingAndReturnFreeStatus());
  SendDataToServer();
  
  DisconnectFromServer();
}

bool IntervalTimerElapsed()
{  unsigned long ul_CurrentMillis = millis();
  if( ul_CurrentMillis - ul_PreviousMillis > ul_Interval)
  {
    ul_PreviousMillis += ul_Interval;
    return true;
  }
  return false;    
}


void InitSerialPort()
{
    Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println(F("Starting..."));
}




