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

#ifndef _DISPLAY_h
#define _DISPLAY_h

#include "Arduino.h"
#include "settings.h"

class DisplayClass
{
	private:
		uint8_t currentSymbol1;
		uint8_t currentSymbol2;
		uint8_t currentDot1;
		uint8_t currentDot2;
		uint8_t getShiftData(uint8_t charnum, uint8_t symbol, uint8_t dot);
	public:
		uint8_t hexMode;
		void init();
		void show(int8_t symbol1, int8_t symbol2, uint8_t dot1, uint8_t dot2);
		void show(int8_t symbol1, int8_t symbol2);
		void showDot(uint8_t dot1, uint8_t dot2);
		void setEnabled(uint8_t enabled);
};

extern DisplayClass Display;

#endif
