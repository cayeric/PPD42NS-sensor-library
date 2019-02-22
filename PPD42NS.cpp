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

volatile uint8_t _pin_P1, _pin_P2;
volatile float samplePM10, samplePM25;

PPD42NS::PPD42NS()
{
}

void PPD42NS::begin(uint8_t pin_P1, uint8_t pin_P2) 
{
	begin(pin_P1, pin_P2, DEFAULT_SAMPLE_INTERVAL);
}

void processSampleData()
{
	samplePM10=LPO1;
	samplePM25=LPO2;
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

/*
float PPD42NS::concentrationPM10()
{
	return 1.1*pow(ratioPM10,3)-3.8*pow(ratioPM10,2)+520*ratioPM10+0.62;;
}

float PPD42NS::concentrationPM25()
{
	return 1.1*pow(ratioPM25,3)-3.8*pow(ratioPM25,2)+520*ratioPM25+0.62;;
}
*/
float PPD42NS::countPM10()
{
	return 1.1*pow(samplePM10,3)-3.8*pow(samplePM10,2)+520*samplePM10+0.62;;
}

float PPD42NS::countPM25()
{
	return 1.1*pow(samplePM25,3)-3.8*pow(samplePM25,2)+520*samplePM25+0.62;;
}

float PPD42NS::ratioPM10(void)
{
	return samplePM10 / (_interval * 10.0);
}

float PPD42NS::ratioPM25(void)
{
	return samplePM25 / (_interval * 10.0);
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
	samplePM10=0;
	samplePM25=0;
	
	// set measuring interval
	LPO1=0;
	LPO2=0;
	_intervalTrigger.attach(_interval, processSampleData);
	
	// set up interrupts
	attachInterrupt(digitalPinToInterrupt(_pin_P1), lowP1, FALLING);
	attachInterrupt(digitalPinToInterrupt(_pin_P2), lowP2, FALLING);
	
//	attachInterrupt(digitalPinToInterrupt(_pin_P1), highP1, RISING);
//	attachInterrupt(digitalPinToInterrupt(_pin_P2), highP2, RISING);
}

void PPD42NS::end()
{
	detachInterrupt(digitalPinToInterrupt(_pin_P1));
	detachInterrupt(digitalPinToInterrupt(_pin_P2));
}
