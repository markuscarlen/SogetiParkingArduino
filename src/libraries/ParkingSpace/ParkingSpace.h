#ifndef ParkingSpace_h
#define ParkingSpace_h

#include <NewPing.h>
#include "Arduino.h"
#define DEBUG 1
class ParkingSpace
{
public:
	ParkingSpace(unsigned int parkingLotNumber, uint8_t trigPin, uint8_t echoPin, unsigned int maxOccupiedDistance_cm);
	bool PingAndReturnStatusChanged();
	bool PingAndReturnFreeStatus();
	bool IsFree() { return _lastIsFree; };
	unsigned int GetSpaceNumber() { return _parkingSpaceNumber; };
	void EnablePowerSaving(unsigned int powerDownPin);
	void DisablePowerSaving();

private:
	NewPing _distanceSensor;
	unsigned int _parkingSpaceNumber;
	bool _lastIsFree;
	unsigned int _maxOccupiedDistance_cm;
	bool _powerSavingEnabled;
	unsigned int _powerDownPin;
	bool EvaluateIfFree(int distance);
	int PingMedian(unsigned int median);
};

#endif
