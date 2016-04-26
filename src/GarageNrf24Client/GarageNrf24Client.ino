#include "ParkingSpace.h"

#define LED 2
#define POWER_SAVING_PIN 5
#define APP_VERSION(Major, Minor) Major << 4 + Minor

#define APP_MAJOR 1
#define APP_MINOR 0

char stringToSend[160];

ParkingSpace parkingSpace(99, 3, 4, 150);

unsigned long ul_PreviousMillis = 0UL;
unsigned long ul_Interval = 600000UL; //Interval 10min (10*60*1000)

byte ErrorCode = 0;

void setup()
{
  pinMode(LED, OUTPUT); //LED for showing sending to server.

  InitSerialPort();
  InitNrf24();
  parkingSpace.EnablePowerSaving(POWER_SAVING_PIN);
  SendTimeIntervalStatusUpdate();
  ul_PreviousMillis = millis();
}

void loop()
{ 
  delay(2000);
  if(parkingSpace.PingAndReturnStatusChanged()) SendData(parkingSpace.GetSpaceNumber(), parkingSpace.IsFree());
  if(IntervalTimerElapsed()) SendTimeIntervalStatusUpdate();
}

void SendData(int spaceNr, bool isFree)
{
  Nrf24SendData(spaceNr, isFree, ErrorCode);
  ErrorCode = 0;
}

void SendTimeIntervalStatusUpdate()
{
  if(parkingSpace.PingAndReturnStatusChanged()) SendData(parkingSpace.GetSpaceNumber(), parkingSpace.IsFree());
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




