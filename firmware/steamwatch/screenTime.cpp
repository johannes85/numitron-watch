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

#include "screenTime.h"
#include "display.h"

void ScreenTimeClass::init()
{
	Screen::init();
	
	timeout.timeoutValue = 450;
		
	Rtc.now(currentTime);	
	
	currentStep = 0;
	timeout.reset();
}

void ScreenTimeClass::loop()
{
	if (timeout.isTimedOut() || currentStep == 0) {
		if (currentStep == 0) {
			if (showDate == 1) {
				currentStep = 1;
			} else if (showTime == 1) {
				currentStep = 7;
			} else {
				
			}
		} else {
			currentStep++;
			if (currentStep == 6 && showTime == 0) {
				currentStep = 12;
			}
		}
		switch (currentStep) {
			case 1:
				Display.show(currentTime.day / 10, currentTime.day % 10, 0, 0);
				timeout.timeoutValue = 450;
				break;
			case 2:
				Display.show(255, 255, 0, 0);
				timeout.timeoutValue = 150;
				break;
			case 3:
				Display.show(currentTime.month / 10, currentTime.month % 10, 0, 0);
				timeout.timeoutValue = 450;
				break;
			case 4:
				Display.show(255, 255, 0, 0);
				timeout.timeoutValue = 150;
				break;
			case 5:
				Display.show(currentTime.year / 10, currentTime.year % 10, 0, 0);
				timeout.timeoutValue = 600;
				break;
			case 6:
				Display.show(255, 255, 0, 0);
				timeout.timeoutValue = 1000;
				break;
			case 7:
				Display.show(currentTime.hours / 10, currentTime.hours % 10, 0, 0);
				timeout.timeoutValue = 450;
				break;
			case 8:
				Display.show(255, 255, 0, 0);
				timeout.timeoutValue = 150;
				break;
			case 9:
				Display.show(currentTime.minutes / 10, currentTime.minutes % 10, 0, 0);
				timeout.timeoutValue = 450;
				break;
			case 10:
				Display.show(255, 255, 0, 0);
				timeout.timeoutValue = 150;
				break;
			case 11:
				Display.show(currentTime.seconds / 10, currentTime.seconds % 10, 0, 0);
				timeout.timeoutValue = 600;
				break;
		}
		timeout.reset();
		
		if (currentStep > 11) {
			screenFinished = 1;
		}
	}
}

ScreenTimeClass ScreenTime;

