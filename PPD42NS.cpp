/* PPD42NS library
 
 MIT license
 written by Cay-Eric Schimanski
 */

#include "PPD42NS.h"
#include <math.h>
// contains LOW-PULSE-OCCUPANCY in milliseconds
volatile uint32_t LPO1, LPO2;
// contains LOW-PULSE-BEGIN timestamp in milliseconds
volatile uint32_t LPB1, LPB2;
// pin configuration
volatile uint8_t _pin_P1, _pin_P2;
volatile float sampleP1, sampleP2;

PPD42NS::PPD42NS()
{
}

void PPD42NS::begin(uint8_t pin_P1, uint8_t pin_P2) 
{
	begin(pin_P1, pin_P2, DEFAULT_SAMPLE_INTERVAL);
}

void processSampleData()
{
	sampleP1=LPO1;
	sampleP2=LPO2;
	LPO1=0;
	LPO2=0;
}

void lowP1()
{
	detachInterrupt(digitalPinToInterrupt(_pin_P1));
	LPB1=millis();
	attachInterrupt(digitalPinToInterrupt(_pin_P1), highP1, RISING);
}

void lowP2()
{
	detachInterrupt(digitalPinToInterrupt(_pin_P2));
	LPB2=millis();
	attachInterrupt(digitalPinToInterrupt(_pin_P2), highP2, RISING);
}

void highP1()
{
	detachInterrupt(digitalPinToInterrupt(_pin_P1));
	LPO1=LPO1+(millis()-LPB1);
	attachInterrupt(digitalPinToInterrupt(_pin_P1), lowP1, FALLING);
}

void highP2()
{
	detachInterrupt(digitalPinToInterrupt(_pin_P2));
	LPO2=LPO2+(millis()-LPB2);
	attachInterrupt(digitalPinToInterrupt(_pin_P2), lowP2, FALLING);
}

// concentration returned as μg/m^3
float PPD42NS::concentrationPM10()
{
	return countPM10() * CONCENTRATION_CONSTANT_P10;
}

float PPD42NS::concentrationPM25()
{
	return countPM25() * CONCENTRATION_CONSTANT_P25;
}

// the result from count function returns pcs/0.01 ft^3 (=pcs/cft^3)
float PPD42NS::countPM25()
{
	float ratio = sampleP1 / (_interval * 10.0);
	float count = ((1.1*pow(ratio,3))-(3.8*pow(ratio,2))+(520*ratio)+0.62) - countPM10();
	// return zero if there is an error when subtracting PM10 particle count
	return (count<0) ? 0 : count;
}

float PPD42NS::countPM10()
{
	float ratio = sampleP2 / (_interval * 10.0);
	return (1.1*pow(ratio,3))-(3.8*pow(ratio,2))+(520*ratio)+0.62;
}

void PPD42NS::begin(uint8_t pin_P1, uint8_t pin_P2, uint8_t interval) 
{
	if (interval == 0)
	{
		interval = DEFAULT_SAMPLE_INTERVAL;
	}

	_pin_P1 = pin_P1;
	_pin_P2 = pin_P2;
	_interval = interval;
	
	// set up the pins
	pinMode(_pin_P1, INPUT);
	pinMode(_pin_P1, INPUT);
	DEBUG_PRINT("Set measure window interval ticker: "); DEBUG_PRINTLN(_interval, DEC);
	
	// initial concentration is zero
	sampleP1=0;
	sampleP2=0;
	
	// set measuring interval
	LPO1=0;
	LPO2=0;
	_intervalTrigger.attach(_interval, processSampleData);
	
	// set up interrupts
	attachInterrupt(digitalPinToInterrupt(_pin_P1), lowP1, FALLING);
	attachInterrupt(digitalPinToInterrupt(_pin_P2), lowP2, FALLING);
}

void PPD42NS::end()
{
	detachInterrupt(digitalPinToInterrupt(_pin_P1));
	detachInterrupt(digitalPinToInterrupt(_pin_P2));
}
