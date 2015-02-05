/*
 *   ________               __     __      __         __         .__
 *  /  _____/  ____   ____ |  | __/  \    /  \_____ _/  |_  ____ |  |__
 * /   \  ____/ __ \_/ __ \|  |/ /\   \/\/   /\__  \\   __\/ ___\|  |  \
 * \    \_\  \  ___/\  ___/|    <  \        /  / __ \|  | \  \___|   Y  \
 *  \______  /\___  >\___  >__|_ \  \__/\  /  (____  /__|  \___  >___|  /
 *         \/     \/     \/     \/       \/        \/          \/     \/
 *    Numitron Geekwatch
 *    v0.2
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
	B00111111,  //10 - a
	B01111010,  //11 - b
	B01010011,  //12 - c
	B01111100,  //13 - d
	B01011011,  //14 - e
	B00011011,  //15 - f
	B01110011,  //16 - g
	B00111110,  //17 - h
	B00010010,  //18 - i
	B01110100,  //19 - j
	B00111011,  //20 - k
	B01010010,  //21 - l
	B00110001,  //22 - m
	B00110111,  //23 - n
	B01111000,  //24 - o
	B00011111,  //25 - p
	B00101111,  //26 - q
	B00010111,  //27 - r
	B01101011,  //28 - s
	B01011010,  //29 - t
	B01110110,  //30 - u
	B01100110,  //31 - v
	B01000110,  //32 - w
	B00111110,  //33 - x
	B01101110,  //34 - y
	B01001101,  //35 - z
	B00001000,  //36 - minus
	B00000000,  //37 - OFF
};

void DisplayClass::init()
{
	currentSymbol1 = 255;
	currentSymbol2 = 255;
	currentDot1 = 0;
	currentDot2 = 0;
	hexMode = 0;
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

void DisplayClass::show(int8_t symbol1, int8_t symbol2)
{
	show(symbol1, symbol2, currentDot1, currentDot2);
}

void DisplayClass::show(int8_t symbol1, int8_t symbol2, uint8_t dot1, uint8_t dot2)
{
	if (hexMode == 1) {
		if (symbol2 < 0) {
			symbol2 = symbol2 * -1;
			symbol1 = 36;
		} else {
			symbol1 = 255;
		}
	}
	
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
