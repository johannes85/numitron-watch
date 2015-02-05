/*
 *   ________               __     __      __         __         .__
 *  /  _____/  ____   ____ |  | __/  \    /  \_____ _/  |_  ____ |  |__
 * /   \  ____/ __ \_/ __ \|  |/ /\   \/\/   /\__  \\   __\/ ___\|  |  \
 * \    \_\  \  ___/\  ___/|    <  \        /  / __ \|  | \  \___|   Y  \
 *  \______  /\___  >\___  >__|_ \  \__/\  /  (____  /__|  \___  >___|  /
 *         \/     \/     \/     \/       \/        \/          \/     \/
 *    Numitron Geekwatch
 *    TOTP Screen (otp lib)
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

#include "otpSigner.h"
#include "sha1.h"

#define DEBUG 1

uint32_t OTPSignerClass::sign(
	uint8_t *challenge,
	uint8_t challengeLength,
	uint8_t *key,
	uint8_t keyLength
) {
  Sha1.initHmac(key, keyLength);
  Sha1.write(challenge, challengeLength);
  hmacResult = Sha1.resultHmac();
  
  uint8_t offset1 = hmacResult[19] & 0xF;
  hotpResult =
    (uint32_t)(hmacResult[offset1]  & 0x7F) << 24 |
    (uint32_t)hmacResult[offset1+1] << 16 |
    (uint32_t)hmacResult[offset1+2] <<  8 |
    hmacResult[offset1+3] ;
  hotpResult %= 1000000;
  
  return(hotpResult);
}

OTPSignerClass OTPSigner;
