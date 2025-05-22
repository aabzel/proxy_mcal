#ifndef CAN_MCAL_DRV_H
#define CAN_MCAL_DRV_H

#include "can_types.h"
#include "can_config.h"
#ifdef HAS_CAN_DIAG
#include "can_diag.h"
#endif
#include "can_custom_drv.h"

#ifdef HAS_CAN_FD
#include "can_fd_mcal.h"
#endif

CanHandle_t* CanGetNode(uint8_t num);
const CanConfig_t* CanGetConfig(uint8_t num);
const CanInfo_t* CanGetInfo(uint8_t num);

bool can_is_message_valid(CanMessage_t* const Message);

/*API*/
bool can_mcal_init(void);
bool can_init_one(uint8_t num);
bool can_init_custom(void);
bool CanIsValidConfig(const CanConfig_t* const Config);
bool can_init_common_one(const CanConfig_t* const Config, CanHandle_t* const Node) ;

bool can_health_monitor_proc(void);
bool can_health_monitor_proc_one(uint8_t num);


uint8_t can_fd_dlc_to_size(const uint8_t dlc);

bool can_proc(void);
bool can_proc_one(uint8_t num);

bool can_heartbeat_proc(void);
bool can_heartbeat_proc_one(uint8_t num) ;


bool can_tec_get( uint8_t num, uint32_t* const tec);
bool can_rec_get( uint8_t num, uint32_t* const rec);

bool can_is_init(uint8_t num) ;
bool can_is_allowed(uint8_t num) ;
bool can_api_transmit_buff(uint8_t num, uint16_t id, const uint8_t* const data, uint8_t size);
bool can_api_transmit_data(uint8_t num, const CanMessage_t* const Message);
bool can_check(void);
bool can_api_baudrate_set(uint8_t num, uint32_t baudrate);
bool can_api_baudrate_get(uint8_t num, uint32_t * const bit_rate);
bool can_wait_tx_done_ll(CanHandle_t* const Node) ;
bool can_mesg_buff_rx(uint8_t can_num, uint8_t mb_idx,  uint32_t can_id) ;

#endif /* CAN_MCAL_DRV_H  */
