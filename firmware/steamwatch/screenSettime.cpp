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

#include "screenSettime.h"
#include "display.h"
#include "settings.h"

void ScreenSettimeClass::init()
{
	Screen::init();
	
	timeoutBlink.timeoutValue = 200;
	timeoutClose.timeoutValue = 30000;
	
	timeSet = 0;
	displayVisible = 1;
	currentOption = 0;
	Display.hexMode = 1;
	oldButtonValueLeft = 1;
	oldButtonValueRight = 1;
	Rtc.now(currentTime);
	currentValue = currentTime.utcOffset;
	
	timeoutBlink.reset();
	timeoutClose.reset();
}

void ScreenSettimeClass::loop()
{
	if (digitalRead(BTN_LEFT) == 0) {
		if (oldButtonValueLeft != 0) {
			currentValue++;
			oldButtonValueLeft = 0;
			timeoutButtonValue.timeoutValue = 1000;
			timeoutButtonValue.reset();
			delay(40);
		} else {
			if (timeoutButtonValue.isTimedOut()) {
				currentValue++;
				timeoutButtonValue.timeoutValue = 150;
				timeoutButtonValue.reset();
			}
		}
		
		int8_t maxValue = 0;
		int8_t initValue = 0;
		switch (currentOption) {
			case 0:
			// UTC offset
				maxValue = 14;
				initValue = -12;
				break;
			// Day
			case 1:
				maxValue = 31;
				initValue = 1;
				break;
			// Month
			case 2:
				maxValue = 12;
				initValue = 1;
				break;
			// Year
			case 3:
				maxValue = 99;
				break;
			// Hour
			case 4:
				maxValue = 23;
				break;
			// Minutes
			case 5:
			// Seconds
			case 6:
				maxValue = 59;
				break;
		}
		if (currentValue > maxValue) {
			currentValue = initValue;
		}
		
		timeoutClose.reset();
	} else {
		oldButtonValueLeft = 1;
	}
	
	if (digitalRead(BTN_RIGHT) == 0) {
		if (oldButtonValueRight != 0) {
			currentOption++;
			switch (currentOption) {
				case 1:
					currentTime.utcOffset = currentValue;
					currentValue = currentTime.day;
					Display.hexMode = 0;
					break;
				case 2:
					currentTime.day = currentValue;
					currentValue = currentTime.month;
					break;
				case 3:
					currentTime.month = currentValue;
					currentValue = currentTime.year;
					break;
				case 4:
					currentTime.year = currentValue;
					currentValue = currentTime.hours;
					break;
				case 5:
					currentTime.hours = currentValue;
					currentValue = currentTime.minutes;
					break;
				case 6:
					currentTime.minutes = currentValue;
					currentValue = currentTime.seconds;
					break;
				case 7:
					currentTime.seconds = currentValue;
					break;
			}
			
			oldButtonValueRight = 0;
			delay(40);
		}
		
		timeoutClose.reset();
	} else {
		oldButtonValueRight = 1;
	}
	
	if (currentOption < 7) {
		if (Display.hexMode == 1) {
			Display.show(
				0,
				currentValue,
				displayVisible == 1 ? 1 : 0,
				displayVisible == 1 ? 1 : 0
			);
		} else {
			Display.show(
				currentValue / 10,
				currentValue % 10,
				displayVisible == 1 ? 1 : 0,
				displayVisible == 1 ? 1 : 0
			);
		}
		if (timeoutBlink.isTimedOut()) {
			displayVisible = displayVisible == 1 ? 0 : 1;
			timeoutBlink.reset();
		}
		
		if (timeoutClose.isTimedOut()) {
			Display.hexMode = 1;
			screenFinished = 1;
		}
	} else {
		Rtc.set(currentTime);
		EpromSettings.writeTimezoneOffset(currentTime.utcOffset);
		timeSet = 1;
		Display.hexMode = 0;
		screenFinished = 1;
	}
}

ScreenSettimeClass ScreenSettime;
