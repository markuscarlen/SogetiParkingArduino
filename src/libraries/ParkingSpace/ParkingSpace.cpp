#include "ParkingSpace.h"
#include "NewPing.h"
#include "Arduino.h"
#define MinSensorDistance 5
#define MaxSensorDistance 200

ParkingSpace::ParkingSpace(unsigned int parkingSpaceNumber, uint8_t trigPin, uint8_t echoPin, unsigned int maxOccupiedDistance_cm) : _distanceSensor(trigPin, echoPin)
{
	_parkingSpaceNumber = parkingSpaceNumber;
	_maxOccupiedDistance_cm = maxOccupiedDistance_cm;
}

void ParkingSpace::EnablePowerSaving(unsigned int powerDownPin)
{
	_powerSavingEnabled = true;
	_powerDownPin = powerDownPin;
	pinMode(_powerDownPin, OUTPUT);
	digitalWrite(_powerDownPin, LOW);
}

void ParkingSpace::DisablePowerSaving()
{
	if (_powerSavingEnabled)
	{
		digitalWrite(_powerDownPin, HIGH);
	}
	_powerSavingEnabled = false;
}

bool ParkingSpace::PingAndReturnStatusChanged()
{
	int uS = PingMedian(19);

	int distance = (uS / US_ROUNDTRIP_CM);
	
	bool isFree = EvaluateIfFree(distance);

	if (isFree != _lastIsFree)
	{
		_lastIsFree = isFree;
		return true;
	}
	
	return false;
}

bool ParkingSpace::PingAndReturnFreeStatus()
{
	int uS = PingMedian(15);
	int distance = (uS / US_ROUNDTRIP_CM);

	bool isFree = EvaluateIfFree(distance);

	_lastIsFree = isFree;

	return _lastIsFree;
}

int ParkingSpace::PingMedian(unsigned int median)
{
	if (_powerSavingEnabled)
	{
		digitalWrite(_powerDownPin, HIGH);
		delay(10);
	}

	int uS = _distanceSensor.ping_median(median);

	if (_powerSavingEnabled)
	{
		digitalWrite(_powerDownPin, LOW);
	}

	return uS;
}

bool ParkingSpace::EvaluateIfFree(int distance)
{
#if DEBUG
Serial.print("Distance is: ");
Serial.print(distance);
Serial.print("cm. Parking space is ");
#endif

	if (distance < MinSensorDistance)
	{
#if DEBUG
		Serial.println("free.");
#endif
		return true;
	}
	else if (distance < _maxOccupiedDistance_cm)
	{
#if DEBUG
		Serial.println("occupied.");
#endif
		return false;
	}
	else
	{
#if DEBUG
		Serial.println("free.");
#endif
		return true;
	}
}



