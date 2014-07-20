/*
 *   ________               __     __      __         __         .__
 *  /  _____/  ____   ____ |  | __/  \    /  \_____ _/  |_  ____ |  |__
 * /   \  ____/ __ \_/ __ \|  |/ /\   \/\/   /\__  \\   __\/ ___\|  |  \
 * \    \_\  \  ___/\  ___/|    <  \        /  / __ \|  | \  \___|   Y  \
 *  \______  /\___  >\___  >__|_ \  \__/\  /  (____  /__|  \___  >___|  /
 *         \/     \/     \/     \/       \/        \/          \/     \/
 *    Numitron Geekwatch
 *    v0.1
 *  
 * by DomesticHacks
 * http://domestichacks.info/
 * http://www.youtube.com/DomesticHacks
 *
 * Author: Johannes Zinnau (johannes@johnimedia.de)
 * 
 * License:
 * Creative Commons: Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
 * http://creativecommons.org/licenses/by-nc-sa/4.0/
 *
 */

#include "display.h"
#include <avr/pgmspace.h>

/*
* Segment Mapping
* A=1, F=2, B=3, G=4, E=5, C=6, D=7, DOT=8
*
*    1      a
*   2 3    f b
*    4      g
*   5 6    e c
*    7      d
*      8      DOT
*/

PROGMEM prog_uint8_t displayFont[38] = {
	B01110111,  //0
	B00100100,  //1
	B01011101,  //2
	B01101101,  //3
	B00101110,  //4
	B01101011,  //5
	B01111010,  //6
	B00100101,  //7
	B01111111,  //8
	B01101111,  //9
	B00000000,  //10 - OFF
	B00111111,  //11 - a
	B01111010,  //12 - b
	B01010011,  //13 - c
	B01111100,  //14 - d
	B01011011,  //15 - e
	B00011011,  //16 - f
	B01110011,  //17 - g
	B00111110,  //18 - h
	B00010010,  //19 - i
	B01110100,  //20 - j
	B00111011,  //21 - k
	B01010010,  //22 - l
	B00110001,  //23 - m
	B00110111,  //24 - n
	B01111000,  //25 - o
	B00011111,  //26 - p
	B00101111,  //27 - q
	B00010111,  //28 - r
	B01101011,  //29 - s
	B01011010,  //30 - t
	B01110110,  //31 - u
	B01100110,  //32 - v
	B01000110,  //33 - w
	B00111110,  //34 - x
	B01101110,  //35 - y
	B01001101,  //36 - z
	B00001000   //37 - minus
};

void DisplayClass::init()
{
	currentSymbol1 = 255;
	currentSymbol2 = 255;
	currentDot1 = 0;
	currentDot2 = 0;
	pinMode(DISPLAY_PIN_CLOCK, OUTPUT);
	pinMode(DISPLAY_PIN_DATA, OUTPUT);
	pinMode(DISPLAY_PIN_LATCH, OUTPUT);
	pinMode(DISPLAY_PIN_ENABLE, OUTPUT);
	show(currentSymbol1, currentSymbol2, currentDot1, currentDot2);
	setEnabled(1);
}

uint8_t DisplayClass::getShiftData(uint8_t charnum, uint8_t symbol, uint8_t dot)
{
	uint8_t data = 0;
	if (symbol != 255) {
		uint8_t font = pgm_read_byte_near(displayFont + symbol);
		
		if (charnum == 2) {
			if ((font & 1) != 0) {
				data |= 1 << DISPLAY_CHAR2_BIT_A;
			}
			if ((font & 1 << 2) != 0) {
				data |= 1 << DISPLAY_CHAR2_BIT_B;
			}
			if ((font & 1 << 5) != 0) {
				data |= 1 << DISPLAY_CHAR2_BIT_C;
			}
			if ((font & 1 << 6) != 0) {
				data |= 1 << DISPLAY_CHAR2_BIT_D;
			}
			if ((font & 1 << 4) != 0) {
				data |= 1 << DISPLAY_CHAR2_BIT_E;
			}
			if ((font & 1 << 1) != 0) {
				data |= 1 << DISPLAY_CHAR2_BIT_F;
			}
			if ((font & 1 << 3) != 0) {
				data |= 1 << DISPLAY_CHAR2_BIT_G;
			}
			} else {
			if ((font & 1) != 0) {
				data |= 1 << DISPLAY_CHAR1_BIT_A;
			}
			if ((font & 1 << 2) != 0) {
				data |= 1 << DISPLAY_CHAR1_BIT_B;
			}
			if ((font & 1 << 5) != 0) {
				data |= 1 << DISPLAY_CHAR1_BIT_C;
			}
			if ((font & 1 << 6) != 0) {
				data |= 1 << DISPLAY_CHAR1_BIT_D;
			}
			if ((font & 1 << 4) != 0) {
				data |= 1 << DISPLAY_CHAR1_BIT_E;
			}
			if ((font & 1 << 1) != 0) {
				data |= 1 << DISPLAY_CHAR1_BIT_F;
			}
			if ((font & 1 << 3) != 0) {
				data |= 1 << DISPLAY_CHAR1_BIT_G;
			}
		}
	}
	
	if (dot == 1) {
		if (charnum == 2) {
			data |= 1 << DISPLAY_CHAR2_BIT_DOT;
		} else {
			data |= 1 << DISPLAY_CHAR1_BIT_DOT;
		}
	}
	
	return(data);
}

void DisplayClass::show(uint8_t symbol1, uint8_t symbol2)
{
	show(symbol1, symbol2, currentDot1, currentDot2);
}

void DisplayClass::show(uint8_t symbol1, uint8_t symbol2, uint8_t dot1, uint8_t dot2)
{
	digitalWrite(DISPLAY_PIN_LATCH, LOW);
	shiftOut(DISPLAY_PIN_DATA, DISPLAY_PIN_CLOCK, MSBFIRST,getShiftData(2, symbol2, dot2));
	shiftOut(DISPLAY_PIN_DATA, DISPLAY_PIN_CLOCK, MSBFIRST,getShiftData(1, symbol1, dot1));
	digitalWrite(DISPLAY_PIN_LATCH, HIGH);
	
	currentDot1 = dot1;
	currentDot2 = dot2;
	currentSymbol1 = symbol1;
	currentSymbol2 = symbol2;
}

void DisplayClass::showDot(uint8_t dot1, uint8_t dot2)
{
	show(currentSymbol1, currentSymbol2, dot1, dot2);
}

void DisplayClass::setEnabled(uint8_t enabled)
{
	if (enabled == 0) {
		digitalWrite(DISPLAY_PIN_ENABLE, HIGH);
	} else {
		digitalWrite(DISPLAY_PIN_ENABLE, LOW);
	}
}

DisplayClass Display;
