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

#include "rtc.h"
#include <Wire.h>

uint32_t Time::getUnixTimeLocal()
{
	uint32_t ret;
	uint8_t i;
	
	uint8_t yearDiff = 2000 + year - 1970;
	
	// Last years
	ret = yearDiff * (SECS_PER_DAY * 365);
	for (i = 0; i < yearDiff; i++) {
		if (LEAP_YEAR(i)) {
			ret += SECS_PER_DAY;
		}
	}
	
	// This year
	// Last months
	for (i = 1; i < month; i++) {
		if (i == 2 && LEAP_YEAR(yearDiff)) {
			ret += SECS_PER_DAY * 29;
		} else {
			ret += SECS_PER_DAY * monthDays[i - 1];
		}
	}
	// This month
	ret += 
		((day - 1) * SECS_PER_DAY) +
		(hours * SECS_PER_HOUR) +
		(minutes * SECS_PER_MIN) +
		seconds;
	
	return(ret);
}

uint32_t Time::getUnixTimeUTC() {
	return (getUnixTimeLocal() - (utcOffset * SECS_PER_HOUR));
}

void RtcClass::init()
{
	init(0);
}

void RtcClass::init(int8_t utcOffset)
{
	this->utcOffset = utcOffset;
	Wire.begin();
}

void RtcClass::now(Time &time)
{
    Wire.beginTransmission(RTC_ADDR_WRITE >> 1);
    Wire.write(RTC_ADDR_S);
    Wire.endTransmission();
    Wire.requestFrom(RTC_ADDR_READ >> 1, 7);
    
    time.seconds = decodeBcd(Wire.read());
    time.minutes = decodeBcd(Wire.read());
    time.hours = decodeBcd(Wire.read());
    time.day = decodeBcd(Wire.read());
    time.weekday = decodeBcd(Wire.read());
    time.month = decodeBcd(Wire.read());
    time.year = decodeBcd(Wire.read());
	
	time.utcOffset = utcOffset;
 }
 
void RtcClass::set(Time &time)
{
	Wire.beginTransmission(RTC_ADDR_WRITE >> 1);
	Wire.write(RTC_ADDR_S);
	Wire.write(encodeBcd(time.seconds));
	Wire.write(encodeBcd(time.minutes));
	Wire.write(encodeBcd(time.hours));
	Wire.write(encodeBcd(time.day));
	Wire.endTransmission();
  
	Wire.beginTransmission(RTC_ADDR_WRITE >> 1);
	Wire.write(RTC_ADDR_MONTHS);
	Wire.write(encodeBcd(time.month));
	Wire.write(encodeBcd(time.year));
	Wire.endTransmission();
	
	utcOffset = time.utcOffset;
}

uint8_t RtcClass::getStatus()
{
	Wire.beginTransmission(RTC_ADDR_WRITE >> 1);
	Wire.write(RTC_ADDR_STATUS);
	Wire.endTransmission();
	Wire.requestFrom(RTC_ADDR_READ >> 1, 7);
	return(Wire.read());
}

void RtcClass::setStatus(uint8_t status)
{
	Wire.beginTransmission(RTC_ADDR_WRITE >> 1);
	Wire.write(RTC_ADDR_STATUS);
	Wire.write(status);
	Wire.endTransmission();
}

uint8_t RtcClass::decodeBcd(uint8_t bcd)
{
	return((((bcd >> 4) & 0x0F) * 10) + (bcd & 0x0F));
}

uint8_t RtcClass::encodeBcd(uint8_t dec)
{
	return((dec/10 * 16) + (dec % 10));
}

RtcClass Rtc;
