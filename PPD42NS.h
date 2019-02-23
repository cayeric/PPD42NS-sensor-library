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

// define mass for particle according to the following parameters and assumptions
// according to a paper from Mark Uva, Robert Falcone, Anthony McClellan, 
// Edward Ostapowicz: "Preliminary Screening System for Ambient Air Quality 
// in Southeast Philadelphia"
// --------------8<--------------------------------------------------
// All particles are spherical, with a density of 1.65E12 μg/m3 [12]
// The radius of a particle in the PM2.5 channel is 0.44 μm [20]
// The radius of a particle in the PM10 channel is 2.60 μm [20]
// ...
// [12] Tittarelli, T. et al., “Estimation of particle mass concentration in 
//		ambient air using a particle counter,” Atmospheric Environment, vol. 42, 
//		pp. 8543-8548, 2008.
// [20] Lee, J. et al., “Seasonal variations of particle size distributions of 
//		PAHs at Seoul, South Korea,” Air Quality Atmospheric Health, vol. 1, 
//		pp. 57-68, 2008.
// --------------8<--------------------------------------------------
// density = 1.65 g/cm3 = 1650000 g/m3
//
// for PM10 particles
// radius = 2.6 μm = 0,0000026 m
// volume = (4/3) * PI * radius^3 = 7,3622176639 * 10^(-17) m3  (17)
// mass = density * volume = 0,0001214766 μg
//
// for PM2.5 particles
// radius = 0.44 μm = 0.00000044 m
// volume = (4/3) * PI * radius^3 = 3,568179048 * 10^(-19) m3 (19)
// mass = density * volume = 0,0000005887 μg 
//
// the formula for particle count returns its result in pieces / cent cubic feet
// this can be converted into mass per cubic meter using a particle specific 
// constant and the volume conversion constant:
// 
// 1 m^3 = 35,3147 ft^3 = 3531,47 cft^3
#define CONCENTRATION_CONSTANT_P10 0.4289909686 
#define CONCENTRATION_CONSTANT_P25 0.0020789764 

class PPD42NS
{
	public:
	
	PPD42NS();
	void begin(uint8_t pin_P1, uint8_t pin_P2, uint8_t interval);
	void begin(uint8_t pin_P1, uint8_t pin_P2);
	void end();
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
