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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "Arduino.h"

#define DEBUG 1

#define BTN_LEFT  2
#define BTN_RIGHT 3

/*
 * Segment Mapping
 * A=1, F=2, B=3, G=4, E=5, C=6, D=7, DOT=8
 *
 *    1    a
 *   2 3  f b
 *    4    g
 *   5 6  e c
 *    7    d
 *       8   DOT
 */

#define DISPLAY_CHAR1_BIT_A 4
#define DISPLAY_CHAR1_BIT_B 6
#define DISPLAY_CHAR1_BIT_C 5
#define DISPLAY_CHAR1_BIT_D 1
#define DISPLAY_CHAR1_BIT_E 0
#define DISPLAY_CHAR1_BIT_F 3
#define DISPLAY_CHAR1_BIT_G 2
#define DISPLAY_CHAR1_BIT_DOT 7

#define DISPLAY_CHAR2_BIT_A 5
#define DISPLAY_CHAR2_BIT_B 7
#define DISPLAY_CHAR2_BIT_C 6
#define DISPLAY_CHAR2_BIT_D 2
#define DISPLAY_CHAR2_BIT_E 1
#define DISPLAY_CHAR2_BIT_F 4
#define DISPLAY_CHAR2_BIT_G 3
#define DISPLAY_CHAR2_BIT_DOT 0

#define DISPLAY_PIN_CLOCK 9
#define DISPLAY_PIN_LATCH 8
#define DISPLAY_PIN_DATA 7
#define DISPLAY_PIN_ENABLE 10

#define SCREEN_TIME 1
#define SCREEN_DATE 2
#define SCREEN_OTP 3
#define SCREEN_SETTIME 10

#define SETTINGS_ADDR_OFFSET 0

class EpromSettingsClass
{
	public:
		int8_t readTimezoneOffset();
		void writeTimezoneOffset(int8_t offset);
};

extern EpromSettingsClass EpromSettings;

#endif
