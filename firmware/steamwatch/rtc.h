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

#ifndef _RTC_h
#define _RTC_h

#include "Arduino.h"

//Flag is set when EEPROM page is busy due to “write” or automatic EEPROM refresh in progress
#define RTC_STATUSBIT_EEbusy 7
//Flag is set at Power-On, flag must be cleared by writing “0”
#define RTC_STATUSBIT_PON 5
//Flag is set when Self-Recovery Reset or System Reset has been generated.
#define RTC_STATUSBIT_SR 4
//VLOW2 Interrupt generated when supply voltage drops below VLOW2 threshold
#define RTC_STATUSBIT_V2F 3
//VLOW1 Interrupt generated when supply voltage drops below VLOW1 threshold
#define RTC_STATUSBIT_V1F 2

#define RTC_ADDR_STATUS 0x03
#define RTC_ADDR_READ   0xAD
#define RTC_ADDR_WRITE  0xAC
#define RTC_ADDR_M      0x09
#define RTC_ADDR_H      0x0A
#define RTC_ADDR_S      0x08
#define RTC_ADDR_MONTHS  0x0D

class Time
{
	public:
		uint8_t hours;
		uint8_t minutes;
		uint8_t seconds;
		uint8_t day;
		uint8_t weekday;
		uint8_t month;
		uint8_t year;
};

class RtcClass
{
	private:
		uint8_t decodeBcd(uint8_t bcd);
		uint8_t encodeBcd(uint8_t dec);
	public:
		void init();
		void now(Time &time);
		void set(Time &time);
		uint8_t getStatus();
		void setStatus(uint8_t status);
};

extern RtcClass Rtc;

#endif
