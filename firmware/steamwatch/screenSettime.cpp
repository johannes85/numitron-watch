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

#include "screenSettime.h"
#include "display.h"

void ScreenSettimeClass::init()
{
	Screen::init();
	
	timeoutBlink.timeoutValue = 200;
	timeoutClose.timeoutValue = 5000;
	
	timeSet = 0;
	displayVisible = 1;
	currentOption = 0;
	oldButtonValueLeft = 1;
	oldButtonValueRight = 1;
	Rtc.now(currentTime);
	currentValue = currentTime.day;
	
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
		
		int maxValue = 0;
		int initValue = 0;
		switch (currentOption) {
			// Day
			case 0:
				maxValue = 31;
				initValue = 1;
				break;
			// Month
			case 1:
				maxValue = 12;
				initValue = 1;
				break;
			// Year
			case 2:
				maxValue = 99;
				break;
			// Hour
			case 3:
				maxValue = 23;
				break;
			// Minutes
			case 4:
			// Seconds
			case 5:
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
					currentTime.day = currentValue;
					currentValue = currentTime.month;
					break;
				case 2:
					currentTime.month = currentValue;
					currentValue = currentTime.year;
					break;
				case 3:
					currentTime.year = currentValue;
					currentValue = currentTime.hours;
					break;
				case 4:
					currentTime.hours = currentValue;
					currentValue = currentTime.minutes;
					break;
				case 5:
					currentTime.minutes = currentValue;
					currentValue = currentTime.seconds;
					break;
				case 6:
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
	
	if (currentOption < 6) {
		Display.show(
			currentValue / 10,
			currentValue % 10,
			displayVisible == 1 ? 1 : 0,
			displayVisible == 1 ? 1 : 0
		);
		if (timeoutBlink.isTimedOut()) {
			displayVisible = displayVisible == 1 ? 0 : 1;
			timeoutBlink.reset();
		}
		
		if (timeoutClose.isTimedOut()) {
			screenFinished = 1;
		}
	} else {
		Rtc.set(currentTime);
		screenFinished = 1;
		timeSet = 1;
	}
}

ScreenSettimeClass ScreenSettime;

