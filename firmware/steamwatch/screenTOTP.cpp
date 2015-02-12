/*
 *   ________               __     __      __         __         .__
 *  /  _____/  ____   ____ |  | __/  \    /  \_____ _/  |_  ____ |  |__
 * /   \  ____/ __ \_/ __ \|  |/ /\   \/\/   /\__  \\   __\/ ___\|  |  \
 * \    \_\  \  ___/\  ___/|    <  \        /  / __ \|  | \  \___|   Y  \
 *  \______  /\___  >\___  >__|_ \  \__/\  /  (____  /__|  \___  >___|  /
 *         \/     \/     \/     \/       \/        \/          \/     \/
 *    Numitron Geekwatch
 *    TOTP Screen
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

#include "screenTOTP.h"
#include "display.h"
#include "otpSigner.h"
#include "otpSettings.h"

// Rename otpSettings.example.h to otpSettings.h and edit it to set your OTP key
uint8_t otpKey[TOTP_SECRET_LENGTH] = TOTP_SECRET;

void ScreenTOTPClass::init()
{
	Screen::init();
	
	Rtc.now(currentTime);
	uint32_t time = currentTime.getUnixTimeUTC();
	uint32_t decade = time / 30;
	uint32_t tm = decade;
	uint8_t challenge[8];
	for (int i = 7; i >= 0; i--) {
		challenge[i] = tm;
		tm >>= 8;
	}
	uint32_t response = OTPSigner.sign(challenge, 8, otpKey, 20);
	uint8_t i = 0;
	do {
		responseDigits[5 - i] = response % 10;
		response /= 10;
		i++;
	} while (response > 0);

	currentStep = 0;
	timeout.timeoutValue = 450;
	timeout.reset();
}

void ScreenTOTPClass::loop()
{
	if (timeout.isTimedOut() || currentStep == 0) {
		currentStep++;
		if (currentStep % 2 == 0) {
			Display.show(255, 255, 0, 0);
			timeout.timeoutValue = 150;
		} else {
			Display.show(responseDigits[currentStep - 1], responseDigits[currentStep], 0, 0);
			timeout.timeoutValue = 450;
		}
		timeout.reset();
		if (currentStep > 5) {
			screenFinished = 1;
		}
	}
}


ScreenTOTPClass ScreenTOTP;
