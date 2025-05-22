#ifndef CAN_MCAL_TYPES_H
#define CAN_MCAL_TYPES_H

#include "std_includes.h"
#include "can_const.h"
#include "can_custom_types.h"
#include "mcal_const.h"
#include "microcontroller_const.h"

typedef union{
    uint32_t standard;   /* specifies the 11 bits standard identifier. value between 0 to 0x7FF. */
    uint32_t extended;                  /* specifies the 29 bits extended identifier. value between 0 to 0x1FFFFFFF. */
}CanId_t;

typedef struct{
    CanId_t identifier;
    CanIdentifier_t id_type;           /* specifies identifier type for the transmit message.*/
    CanTxFrame_t frame_type;           /* specifies frame type for the transmit message.*/
    uint8_t size;                      /* Data Length Code. specifies frame data length that will be transmitted.
                                              this parameter can be a value between 0 to 8 */
    uint8_t data[64];                  /* contains the transmit data. it ranges from 0 to 0xFF. */
#ifdef HAS_CAN_FD
    CanFdBitRateSwitch_t bit_rate_switch;
#endif
    uint8_t mesg_buff_idx;
} CanMessage_t;



#define CAN_FD_MCAL_COMMON_VARIABLES                                          \
    bool fd_enable;    /* Enable/Disable the Flexible Data Rate feature. */   \
    uint32_t bit_rate_fd;   /*for CAD FD data phase*/

#define CAN_MCAL_COMMON_VARIABLES                                             \
    CAN_FD_MCAL_COMMON_VARIABLES                                              \
    CanIdentifier_t identifier;                                               \
    MoveMode_t move_mode;                                                     \
    bool valid;                                                               \
    bool rx_fifo_on_off;                                                      \
    bool heart_beat;                                                          \
    uint8_t num;                                                              \
    uint8_t padding;                                                          \
    uint8_t dma_channel_rx;                                                   \
    uint8_t dma_channel_tx;                                                   \
    uint8_t payload_size;                                                     \
    uint32_t bit_rate;                                                        \
    uint32_t rx_id;                                                           \
    uint32_t tx_id;                                                           \
    CanMode_t mode;                                                           \
    bool re_tx;

typedef struct {
    CAN_MCAL_COMMON_VARIABLES
    char *name;
}CanConfig_t;

typedef struct {
    uint8_t num;
    uint8_t can_num;
    uint8_t mb_index;
    bool valid;
    char *name;
}CanMessageBufferConfig_t;

typedef struct {
    uint8_t num;
    uint8_t can_num;
    uint8_t mb_index;
    bool valid;
    char *name;
    uint8_t state;
} CanMessageBufferHandle_t;

typedef struct {
    CAN_MCAL_COMMON_VARIABLES
    CAN_CUSTOM_VARIABLES
    volatile uint32_t rx_bytes;  // Total rx byte
    volatile uint32_t tx_bytes;  // Total tx byte
    volatile bool unproc_rx_message;
    volatile bool tx_done;
    volatile bool rx_done;
    volatile uint32_t isr_cnt;
    volatile uint32_t err_code;
    volatile uint32_t heart_beat_sn; // serial number of sent heart_beat message
    volatile uint32_t isr_rx_cnt;
    volatile uint32_t isr_tx_cnt;
    volatile uint32_t isr_err_cnt;
    volatile CanMessage_t RxMessage;
    bool init_done;
} CanHandle_t;

#endif /* CAN_MCAL_TYPES_H */
