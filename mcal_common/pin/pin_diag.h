#ifndef PIN_GENERAL_DIAG_H
#define PIN_GENERAL_DIAG_H

#include <stdbool.h>
#include <stdint.h>

#include "pin_types.h"

#ifdef HAS_PIN_CUSTOM
#include "pin_custom_diag.h"
#endif

#ifndef HAS_LOG
#error "+HAS_LOG"
#endif

#ifndef HAS_PIN
#error "+HAS_PIN"
#endif

#ifndef HAS_DIAG
#error "+HAS_DIAG"
#endif

#ifndef HAS_PIN_DIAG
#error "+HAS_PIN_DIAG"
#endif

const char* PinConfToStr(const PinConfig_t* Config);

const char* PinAltFun2Str(uint8_t code);
const char* PinAlterFun2Str(uint8_t code);
#ifdef HAS_EXT_INT
const char* PinEdgeToStr(PinIntEdge_t code);
#endif
const char* PinDir2Str(PinDir_t code);
const char* PinFunToStr(PinPinFunction_t function);
const char* PinLevel2Str(PinLogicLevel_t code);
const char* PinMode2Str(PinApiMode_t code);
const char* PinOutType2Str(uint8_t code);
const char* PinPad2Str(uint8_t pad);
const char* PinPadToStr(Pad_t pad);
const char* PinPad2WireName(uint8_t pad);
const char* PinPull2Str(PinPullMode_t code);
const char* PinSpeed2Str(uint8_t code);
const char* PinType2Str(uint8_t code);

bool pin_diag(char* key_word1, char* key_word2);
bool PinConfigDiag(const PinConfig_t* Config);

#endif /* PIN_GENERAL_DIAG_H  */
