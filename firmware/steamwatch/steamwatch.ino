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

#include <Wire.h>
#include <avr/sleep.h>
#include "settings.h"
#include "rtc.h"
#include "display.h"
#include "screen.h"
#include "screenTime.h"
#include "screenSettime.h"


Screen *currentScreen;
uint8_t wakeUpBy;
uint8_t timeSet;

void setup()
{
	// Switching off ADC to save power
	ADCSRA &= ~(1 << ADEN);

	// Set pin direction
	pinMode(BTN_LEFT, INPUT);
	pinMode(BTN_RIGHT, INPUT);
	
	// Enable pullup resistors
	digitalWrite(BTN_LEFT, HIGH);
	digitalWrite(BTN_RIGHT, HIGH);
	
	#ifdef DEBUG
		Serial.begin(9600);
	#endif
	
	Rtc.init();
	Display.init();
	
	wakeUpBy = 0;
	timeSet = 1;
	
	uint8_t status = Rtc.getStatus();
	if ((status & 1 << RTC_STATUSBIT_PON) != 0) {
		#ifdef DEBUG
			Serial.println("RTC: Power on happened");
		#endif
		timeSet = 0;
		Time time;
		time.hours = 0;
		time.minutes = 0;
		time.seconds = 0;
		time.day = 1;
		time.month = 1;
		time.year = 14;
		Rtc.set(time);
		Rtc.setStatus(status & ~(1 << RTC_STATUSBIT_PON));
	}
}

void loop()
{
	Display.show(255, 255, 0, 0);
	sleep();
	
	delay(40);
	uint8_t selectedScreen = 0;
	if (wakeUpBy == BTN_RIGHT) {
		selectedScreen = SCREEN_DATE;
	} else {
		selectedScreen = SCREEN_TIME;
	}
	do {
		if (digitalRead(BTN_LEFT) == 0 && digitalRead(BTN_RIGHT) == 0) {
			selectedScreen = SCREEN_SETTIME;
		}
	} while (digitalRead(BTN_LEFT) == 0 || digitalRead(BTN_RIGHT) == 0);
	// Time isn't set (power on), display setup screen
	if (timeSet == 0) {
		selectedScreen = SCREEN_SETTIME;
	}

	#ifdef DEBUG
		Serial.print("Wake up by: ");
		Serial.println(wakeUpBy);
		Serial.print("Switch to screen: ");
		Serial.println(selectedScreen);
	#endif

	if (selectedScreen != 0) {
		switch (selectedScreen) {
			case SCREEN_DATE:
				ScreenTime.showDate = 1;
				ScreenTime.showTime = 0;
				currentScreen = &ScreenTime;
				break;
			case SCREEN_TIME:
				ScreenTime.showDate = 0;
				ScreenTime.showTime = 1;
				currentScreen = &ScreenTime;
				break;
			case SCREEN_SETTIME:
				currentScreen = &ScreenSettime;
				break;
		}
		
		currentScreen->init();
		do {
			currentScreen->loop();
		} while(currentScreen->screenFinished == 0);
		
		if (selectedScreen == SCREEN_SETTIME) {
			if (ScreenSettime.timeSet) {
				timeSet = 1;
			}
		}
	}
}

void sleep() {
	delay(500);
	
	Display.setEnabled(0);
	
	// Switching off ADC
	// Commented out because this project doesn't need the ADC and it's already disabled
	//ADCSRA &= ~(1 << ADEN);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	attachInterrupt(0, wakeUpIsr0, LOW);
	attachInterrupt(1, wakeUpIsr1, LOW);
	sleep_mode();
	// -> WAKE UP POINT
	sleep_disable();
	
	// Switching on ADC
	// Commented out because this project doesn't need the ADC and it's already disabled
	//ADCSRA |= 1 << ADEN;
	
	detachInterrupt(0);
	detachInterrupt(1);
	
	Display.setEnabled(1);
}

void wakeUpIsr0() {
	wakeUpBy = BTN_LEFT;
}

void wakeUpIsr1() {
	wakeUpBy = BTN_RIGHT;
}
