/**********************************************************************
@file       Goldboard4.h

@brief      Driver for Goldboard4 (w. Atmega32). Contains also access to the compass and sonar sensor.

-----------------------------------------------------------------------
@author  2015 Christoph Rothermel
         Christoph.Rothermel@gmx.de
-----------------------------------------------------------------------
@History

1.00 28.01.2015 First working version

@end
***********************************************************************/

#ifndef __GOLDBOARD4_H__
#define __GOLDBOARD4_H__

#include "global.h"
#include "i2c.h"
#include "Motor.h"
#include "Serial.h"
#include "SRF08.h"
#include "CMPS03.h"


class Goldboard4
{
//variables
public:

protected:
private:
	Motor _motors[4];
	PCF8574A _pcf8574;
	
	SoftwareSerial _serial;
	CMPS03 _cmps03;
	SRF08 _srf08[4];
	
	
	bool _isLED[2];
	uint8_t _analogPulseInputConfiguration;
	

//functions
public:
	Goldboard4();
	
	
	/** initializes the compass sensor. It is usable after this method is called
	*/
	void initCompass();
	
	/** initialize the sonar with the given id (0..3) and the given address
	*/
	void initSonar(uint8_t id, uint8_t address);
	
	/** returns the compass, if it is initialized
	*/
	CMPS03 getCompass();
	
	/**  returns the given sonar, if it is initialized
	*/
	SRF08 getSonar(uint8_t id);
	
	/**  returns the given motor
	*/
	Motor getMotor(uint8_t id);
	
	/**  returns the serial interface
	*/
	SoftwareSerial getSerial();
	
	/** Puts off all motors.
	*/
	void setMotorsOff();
	
	/** Sets a motor offset
	*/
	void setMotorsOffset(int16_t value);
	
	/** sets the given led id as led (NOTE: Then this pin cannot be used as button anymore)
	*/
	void initLED(uint8_t i);

	/** Puts LED i on if state is true, else off
	*/
	void setLed(uint8_t i, bool state);

	/** Puts the power output i on if state is true, else off
	*/
	void setPower(uint8_t i, bool state);

	/** Checks the state of button i. If it is pressed, true is returned,
	*  else false.
	*/
	bool getButton(uint8_t i);
	
	/** Waits until button i is pressed and released again.
	*/
	void waitForButton(uint8_t i);

	/** returns the value of the analog port i. 0 <= value <= 255
	*/
	uint8_t getAnalog(uint8_t i);

	/** returns true if the pwm port is logical high, else false.
	*/
	bool getPWM(uint8_t i);
	
	/** returns true if the digital port is logical high, else false.
	*/
	bool getDigital(uint8_t i);

	/** returns pwm value
	*/
	//uint8_t getPWM(uint8_t i);

	/** returns the registered pulse length of the digital port i. 0 <= value <= 255
	*/
	uint8_t getDigitalPulsedLight(uint8_t i);

	/** returns the registered pulse length of the pwm port i. 0 <= value <= 255
	*/
	uint8_t getPWMPulsedLight(uint8_t i);
protected:
private:
	Goldboard4( const Goldboard4 &c );
	Goldboard4& operator=( const Goldboard4 &c );
	
	uint8_t calcPulsedValue(unsigned long* val);

}; //Goldboard4

extern Goldboard4 gb;

#endif //__GOLDBOARD4_H__
