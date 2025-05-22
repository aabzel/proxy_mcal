#include "can_mcal.h"

#include "array.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "code_generator.h"
#include "compiler_const.h"
#include "log.h"

#ifdef HAS_ISO_TP
#include "iso_tp.h"
#endif

COMPONENT_GET_CONFIG(Can, can)

COMPONENT_GET_NODE(Can, can)


uint8_t can_fd_dlc_to_size(const uint8_t dlc_code) {
    uint8_t payload_size = 0 ;
    if (dlc_code <= 8) {
        payload_size = dlc_code ;
    } else {
        switch(dlc_code) {
            case 9: payload_size = 12 ; break;
            case 10: payload_size = 16 ; break;
            case 11: payload_size = 20 ; break;
            case 12: payload_size = 24 ; break;
            case 13: payload_size = 32 ; break;
            case 14: payload_size = 48 ; break;
            case 15: payload_size = 64 ; break;
            default: payload_size = 64 ; break;
        }
    }
    return payload_size;

}





bool can_wait_tx_done_ll(CanHandle_t* const Node) {
    bool res = false;

    uint32_t cur_ms = 0;
    uint32_t dutation_ms = 0;
    uint32_t start_ms = time_get_ms32();
    while(1) {
        cur_ms = time_get_ms32();
        dutation_ms = cur_ms - start_ms;
        if(CAN_TX_DONE_TIMEOUT_MS < dutation_ms) {
            res = false;
            LOG_ERROR(CAN, "TxTimeOut");
            break;
        }
    }
    return res;
}

static bool can_is_std_is_valid(CanMessage_t* const Message) {
    bool res = false;
    if(Message->identifier.standard <= 0x7FF) {
        res = true;
    }
    return res;
}

static bool can_is_ext_is_valid(CanMessage_t* const Message) {
    bool res = false;
    if(Message->identifier.extended <= 0x1FFFFFFF) {
        res = true;
    }
    return res;
}

bool can_is_message_valid(CanMessage_t* const Message) {
    bool res = false;
    if(Message) {
        if(Message->size <= 64) {
            if(0 < Message->size) {
                switch((uint8_t)Message->id_type) {
                case CAN_FRAME_ID_STANDARD:
                    res = can_is_std_is_valid(Message);
                    break;
                case CAN_FRAME_ID_EXTENDED:
                    res = can_is_ext_is_valid(Message);
                    break;
                }
            }
        }
    }
    return res;
}


bool can_is_init(uint8_t num) {
    bool res = false;
    CanHandle_t* Node = CanGetNode(num);
    if(Node) {
        res = Node->init_done;
    }
    return res;
}


bool can_init_common_one(const CanConfig_t* const Config, CanHandle_t* const Node) {
   bool res = false;
   if(Config) {
       if(Node) {
           Node->bit_rate = Config->bit_rate;
           Node->bit_rate_fd = Config->bit_rate_fd;
           Node->heart_beat = Config->heart_beat;
           Node->dma_channel_rx = Config->dma_channel_rx;
           Node->dma_channel_tx = Config->dma_channel_tx;
           Node->fd_enable = Config->fd_enable;
           Node->identifier = Config->identifier;
           Node->mode = Config->mode;
           Node->move_mode = Config->move_mode;
           Node->padding = Config->padding;
           Node->payload_size = Config->payload_size;
           Node->re_tx = Config->re_tx;
           Node->rx_fifo_on_off = Config->rx_fifo_on_off;
           Node->rx_id = Config->rx_id;
           Node->tx_id = Config->tx_id;
           res = true;
       }
   }
   return res;
}

bool CanIsValidConfig(const CanConfig_t* const Config) {
    bool res = false;
    if(Config) {
        res = true;
        if(res) {
            if(0 < Config->payload_size) {
                res = true;
            } else {
                res = false;
                LOG_ERROR(CAN, "ErrPayloadSize");
            }
        }

        if(res) {
            if(0 < Config->identifier) {
                res = true;
            } else {
                res = false;
                LOG_ERROR(CAN, "ErrID");
            }
        }

        if(res) {
            if(0 < Config->move_mode) {
                res = true;
            } else {
                res = false;
                LOG_ERROR(CAN, "ErrMoveMode");
            }
        }

        if(res) {
            if(0 < Config->bit_rate) {
                res = true;
            } else {
                res = false;
                LOG_ERROR(CAN, "ErrBitRate");
            }
        }

        if(res) {
            if(0 < Config->bit_rate_fd) {
                res = true;
            } else {
                res = false;
                LOG_ERROR(CAN, "ErrBitRateFd");
            }
        }

        if(res) {
            if(Config->bit_rate <= Config->bit_rate_fd) {
                res = true;
            } else {
                res = false;
                LOG_ERROR(CAN, "ErrBitRatesNonSense");
            }
        }


        if(res) {
            if(0 < Config->mode) {
                res = true;
            } else {
                res = false;
                LOG_ERROR(CAN, "ErrMode");
            }
        }

        if(res) {
            if(0 < Config->rx_id) {
                res = true;
            } else {
                res = false;
                LOG_ERROR(CAN, "ErrRxId");
            }
        }

        if(res) {
            if(0 < Config->tx_id) {
                res = true;
            } else {
                res = false;
                LOG_ERROR(CAN, "ErrTxId");
            }
        }
    }
    return res;
}

bool can_is_allowed(uint8_t num) {
    bool res = false;
    CanHandle_t* Node = CanGetNode(num);
    if(Node) {
        const CanConfig_t* Config = CanGetConfig(num);
        if(Config) {
            res = true;
        }
    }
    return res;
}

_WEAK_FUN_
bool can_mesg_buff_rx(uint8_t can_num, uint8_t mb_idx,  uint32_t can_id) {
    bool res = false;
    LOG_ERROR(CAN, "Indef");
    return res;
}

_WEAK_FUN_ bool can_init_custom(void) {
    bool res = true;
    LOG_INFO(CAN, "InitCustom");
    return res;
}

_WEAK_FUN_ bool can_heartbeat_proc_one(uint8_t num) {
    bool res = false;
    CanHandle_t* Node = CanGetNode(num);
    if(Node) {
        LOG_PARN(CAN, "CAN%u,HeartBeat,Proc", num);
        CanMessage_t Message = {0};
        Message.id_type = CAN_FRAME_ID_STANDARD;
        Message.frame_type = CAN_TX_FRAME_DATA;
        Message.identifier.standard = 0x7FF;
        Message.size = 8;
        array_incr(Message.data, 8, 0);
        snprintf((char *) Message.data, 8, "CAN%u,%u", num, Node->heart_beat_sn);
        res = can_api_transmit_data(num, &Message);
        if(res) {
            LOG_PARN(CAN, "CAN%u,TxOk", num);
            Node->heart_beat_sn++;
        } else {
            LOG_DEBUG(CAN, "CAN%u,TxErr", num);
        }
    }
    return res;
}




_WEAK_FUN_
bool can_tec_get(uint8_t num, uint32_t* const tec) {
    bool res = false;
    return res;
}

_WEAK_FUN_
bool can_rec_get(uint8_t num, uint32_t* const rec) {
    bool res = false;
    return res;
}



_WEAK_FUN_ bool can_proc_one(uint8_t num) {
    bool res = false;
    LOG_PARN(CAN, "CAN%u Proc", num);
    CanHandle_t* Node = CanGetNode(num);
    if(Node) {
        if(Node->unproc_rx_message) {
            LOG_NOTICE(CAN, "CAN%u,Rx:%s", num, CanMessageToStr(&Node->RxMessage));
            Node->unproc_rx_message = false;
            if(Node->rx_id == Node->RxMessage.identifier.standard) {
                /*Is allowed RX*/
#ifdef HAS_ISO_TP
                static IsoTpFrame_t RxFrame = {0};
                memcpy(RxFrame.data, Node->RxMessage.data, 8);
                res = iso_tp_proc_rx(num, Node->RxMessage.identifier.standard, &RxFrame);
#endif
            } else {
                LOG_DEBUG(CAN, "AlienCanID 0x%x,MyCanID:0x%x", Node->RxMessage.identifier.standard, Node->rx_id);
            }
        }
    }
    return res;
}

bool can_api_transmit_buff(uint8_t num, uint16_t id, const uint8_t* const data, uint8_t size) {
    bool res = false;
    if(data) {
        if(size) {
            CanMessage_t TxMessage = {0};
            TxMessage.identifier.standard = id;
            TxMessage.id_type = CAN_FRAME_ID_STANDARD;
            TxMessage.frame_type = CAN_TX_FRAME_DATA;
            TxMessage.size = size;
            memcpy(TxMessage.data, data, size);
            res = can_api_transmit_data(num, &TxMessage);
            // log_res_num(CAN, res, num, "Tx");
            if(res) {
                LOG_DEBUG(CAN, "CAN%u TxOk", num);
            } else {
                LOG_DEBUG(CAN, "CAN%u TxErr", num);
            }
        }
    }
    return res;
}

#if 0
bool can_send_api(uint8_t num, uint8_t* array, uint16_t size) {
    bool res = false;
    return res;
}
#endif

bool can_heartbeat_proc(void) {
    bool res = true;
    uint32_t ok = 0;
    LOG_PARN(CAN, "ProcAll:%u", CAN_COUNT);
    uint32_t num = 0;
    for(num = 0; num <= CAN_COUNT; num++) {
        res = can_heartbeat_proc_one(num);
        if(res) {
            ok++;
        }
    }
    if(ok) {
        res = true;
        LOG_PARN(CAN, "Proc %u CAN", ok);
    } else {
        res = false;
    }
    return res;
}

bool can_health_monitor_proc(void) {
    bool res = true;
    uint32_t ok = 0;
    LOG_PARN(CAN, "ProcAll:%u", CAN_COUNT);
    uint32_t num = 0;
    for(num = 0; num <= CAN_COUNT; num++) {
        res = can_health_monitor_proc_one(num);
        if(res) {
            ok++;
        }
    }
    if(ok) {
        res = true;
        LOG_PARN(CAN, "ProcOk,%u,CANs", ok);
    } else {
        res = false;
    }
    return res;
}


COMPONENT_PROC_PATTERT_CNT(CAN, CAN, can, CAN_COUNT)

COMPONENT_INIT_PATTERT_CNT(CAN, CAN, can, CAN_COUNT)
