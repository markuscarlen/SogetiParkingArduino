#include <RH_NRF24.h>
#include <SPI.h>

#define PROTOCOL_VERSION 1

RH_NRF24 nrf24;

void InitNrf24()
{
  if (!nrf24.init())Serial.println("init failed");

  if (!nrf24.setChannel(1))Serial.println("setChannel failed");
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) Serial.println("setRF failed");   
  //if(!nrf24.sleep()) Serial.println("setSleep failed");
}

bool Nrf24SendData(unsigned int parkingSpace, bool isFree, byte errorCode)
{
  byte data[8];
  byte timeOut = 0; 
  
  data[0] = PROTOCOL_VERSION;
  data[1] = APP_VERSION(APP_MAJOR, APP_MINOR);
  data[2] = parkingSpace >> 8;
  data[3] = parkingSpace %256;
  data[4] = isFree;
  data[5] = 100; //Reserved
  data[6] = 100;
  data[7] = 100; //Battery Voltage

  Serial.println("Sending nrf24...");
  while(1)
  {
    nrf24.send(data, sizeof(data));
    
    nrf24.waitPacketSent() ? Serial.println("Message sent.") : Serial.println("Message not set.");
  
    Serial.println("Waiting for Response...");
    if (nrf24.waitAvailableTimeout(500))
    { Serial.println("is available...");
      // Should be a reply message for us now 
      uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (nrf24.recv(buf, &len))
      {
        Serial.print("got reply: ");
        Serial.println((char*)buf);
        SendToSerialPort(buf, sizeof(buf));
        break;
      }
      else
      {
        Serial.println("recv failed");
      }
    }
    Serial.println("Resending data...");
  }
  nrf24.sleep();
  return true;
}

void SendToSerialPort(byte* b, byte len)
{
  while(len > 0)
  {
    Serial.print(len);
    Serial.print(": ");
    Serial.print(b[len--]);
    Serial.print(" | ");
  }
  Serial.println("");
}
