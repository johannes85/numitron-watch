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

#ifndef _SCREENSETTIME_h
#define _SCREENSETTIME_h

#include "Arduino.h"
#include "screen.h"
#include "timeout.h"
#include "rtc.h"

class ScreenSettimeClass: public Screen
{
	public:
		virtual void init();
		virtual void loop();
		uint8_t timeSet;
	private:
		TimeoutCounter timeoutBlink;
		TimeoutCounter timeoutClose;
		TimeoutCounter timeoutButtonValue;
		Time currentTime;
		uint8_t currentOption;
		uint8_t displayVisible;
		uint8_t currentValue;
		uint8_t oldButtonValueLeft;
		uint8_t oldButtonValueRight;
};

extern ScreenSettimeClass ScreenSettime;

#endif

