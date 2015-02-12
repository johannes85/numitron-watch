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

#include "settings.h"
#include "EEPROM.h"

int8_t EpromSettingsClass::readTimezoneOffset()
{
	#if DEBUG
	Serial.println(F("EpromSettings::readTimezoneOffset: Reading from EEPROM"));
	#endif
	int8_t offset = EEPROM.read(SETTINGS_ADDR_OFFSET);
	#if DEBUG
	Serial.print(F("EpromSettings::readTimezoneOffset: Result: "));
	Serial.println(offset);
	#endif
	if (offset < -15 || offset > 15) {
		#if DEBUG
		Serial.println(F("EpromSettings::writeTimezoneOffset: Out of bounds, setting to default value: 0"));
		#endif
		offset = 0;
	}
	return offset;	
}

void EpromSettingsClass::writeTimezoneOffset(int8_t offset)
{
	#if DEBUG
	Serial.println(F("EpromSettings::writeTimezoneOffset: Writing to EEPROM"));
	Serial.print(F("EpromSettings::writeTimezoneOffset: Value: "));
	Serial.println(offset);
	#endif
	if (offset < -15 || offset > 15) {
		#if DEBUG
		Serial.println(F("EpromSettings::writeTimezoneOffset: Out of bounds, setting to default value: 0"));
		#endif
		offset = 0;
	}
	EEPROM.write(SETTINGS_ADDR_OFFSET, offset);
}

EpromSettingsClass EpromSettings;
