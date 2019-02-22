/* PPD42NS library
 
 MIT license
 written by Cay-Eric Schimanski
 */

#include "Ticker.h"

#ifndef PPD42NS_H
#define PPD42NS_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


// Uncomment to enable printing out nice debug messages.
//#define PPD42NS_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef PPD42NS_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

#define DEFAULT_SAMPLE_INTERVAL 30

class PPD42NS
{
	public:
	
	PPD42NS();
	void begin(uint8_t pin_P1, uint8_t pin_P2, uint8_t interval);
	void begin(uint8_t pin_P1, uint8_t pin_P2);
	void end();
	float ratioPM10(void);
	float ratioPM25(void);
	float countPM10(void);
	float countPM25(void);
	float concentrationPM10();
	float concentrationPM25();

private:
  uint8_t data[5];
	float _interval;
	
	Ticker _intervalTrigger;
};

void highP1();
void lowP1();
void highP2();
void lowP2();

#endif
