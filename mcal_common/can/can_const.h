#ifndef CAN_GEN_CONST_H
#define CAN_GEN_CONST_H

#include "can_dep.h"

#include "time_mcal.h"

#define CAN_PERIOD_US 100
#define CAN_HEARTBEAT_PERIOD_US SEC_2_USEC(50)
#define CAN_FD_HEARTBEAT_PERIOD_US SEC_2_USEC(60)
#define CAN_HEALTH_MONITOR_PERIOD_US SEC_2_USEC(4)

#define CAN_TX_DONE_TIMEOUT_MS 5000

typedef enum {
    CAN_CFG_MODE_COMMUNICATE = 1,
    CAN_CFG_MODE_LOOPBACK = 2,
    CAN_CFG_MODE_LISTENONLY = 3,
    CAN_CFG_MODE_LISTENONLY_LOOPBACK = 4,
    CAN_CFG_MODE_FREEZE = 5,

    CAN_CFG_MODE_UNDEF = 0
} CanMode_t;


typedef enum {
    CAN_FD_DLC_UNDEF = 0
}CanFdDlc_t;


typedef enum {
    CAN_FRAME_ID_STANDARD = 1, /* standard Id */
    CAN_FRAME_ID_EXTENDED = 2,  /* extended Id */

    CAN_FRAME_ID_UNDEF = 0
}CanIdentifier_t  ;

typedef enum{
    CAN_FD_BIT_RATE_SWITCH_ON = 1,
    CAN_FD_BIT_RATE_SWITCH_OFF = 2,
    CAN_FD_BIT_RATE_SWITCH_UNDEF = 0,
}CanFdBitRateSwitch_t;

typedef enum {
    CAN_TX_FRAME_DATA = 1, /*!< data frame */
    CAN_TX_FRAME_REMOTE = 2,  /*!< remote frame */

    CAN_TX_FRAME_UNDEF = 0
}CanTxFrame_t  ;

#endif /* CAN_GEN_CONST_H */
