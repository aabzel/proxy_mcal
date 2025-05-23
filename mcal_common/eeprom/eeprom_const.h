#ifndef EEPROM_MCAL_CONST_H
#define EEPROM_MCAL_CONST_H

#include "eeprom_dep.h"

#define EEPROM_ERASE_PATTERN 0xFF

typedef enum {
    EEPROM_OPERATION_NOPE = 1,
    EEPROM_OPERATION_PROGRAM_BYTE = 8,
    EEPROM_OPERATION_PROGRAM_WORD = 16,
    EEPROM_OPERATION_PROGRAM_QWORD = 32,
    EEPROM_OPERATION_ERRASE = 256,
    EEPROM_OPERATION_UNDEF = 0
}EepromOperation_t;

typedef enum {
    EEPROM_STATE_IDLE = 1,
    EEPROM_STATE_WAIT_END_OF_OPERATION = 2,
    EEPROM_STATE_WAIT_END_OF_WRITE = 3,
    EEPROM_STATE_WAIT_END_OF_ERASE = 4,
    EEPROM_STATE_UNDEF = 0
}EepromState_t;

typedef enum {
    EEPROM_INPUT_NONE = 10,
    EEPROM_INPUT_OPERATION_BUSY = 1,
    EEPROM_INPUT_NO_OPERATION = 6,
    EEPROM_INPUT_PROGRAMMING_ERROR = 2,
    EEPROM_INPUT_ERASE_PROGRAM_PROTECTION_ERROR = 4,
    EEPROM_INPUT_OPERATION_DONE = 5,
    EEPROM_INPUT_TIMEOUT = 3,
    EEPROM_INPUT_UNDEF = 0
}EepromInput_t;

#endif /* EEPROM_MCAL_CONST_H */
