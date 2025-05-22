#include "i2s_mcal.h"

#include <string.h>

#include "array.h"
#include "code_generator.h"
#include "compiler_const.h"
#include "data_utils.h"
#include "float_utils.h"
#include "log.h"
#include "num_to_str.h"
#include "std_includes.h"
#include "time_mcal.h"
#ifdef HAS_I2S_VOLUME
#include "i2s_volume.h"
#endif

#ifdef HAS_DDS
#include "dds_drv.h"
#endif

#ifdef HAS_COMPLEX_DIAG
#include "complex_diag.h"
#endif

#ifdef HAS_DFT
#include "dft.h"
#endif

COMPONENT_GET_NODE(I2s, i2s)
COMPONENT_GET_CONFIG(I2s, i2s)

static const I2sBusDirInfo_t I2sBusDirInfo[] = {
    {
        .bus_role = IF_BUS_ROLE_SLAVE,
        .direction = CONNECT_DIR_TRANSMITTER,
        .i2s_bus_dir = I2S_DIR_BUS_MODE_SLAVE_TX,
        .num = 1,
        .valid = true,
    },
    {
        .bus_role = IF_BUS_ROLE_SLAVE,
        .direction = CONNECT_DIR_RECEIVER,
        .i2s_bus_dir = I2S_DIR_BUS_MODE_SLAVE_RX,
        .num = 2,
        .valid = true,
    },
    {
        .bus_role = IF_BUS_ROLE_MASTER,
        .direction = CONNECT_DIR_TRANSMITTER,
        .i2s_bus_dir = I2S_DIR_BUS_MODE_MASTER_TX,
        .num = 3,
        .valid = true,
    },
    {
        .bus_role = IF_BUS_ROLE_MASTER,
        .direction = CONNECT_DIR_RECEIVER,
        .i2s_bus_dir = I2S_DIR_BUS_MODE_MASTER_RX,
        .num = 4,
        .valid = true,
    },
};

static const I2sBusDirInfo_t* I2sRoleAndDirToInfo(const IfBusRole_t bus_role, const ConnectivitDir_t direction) {
    I2sBusDirInfo_t* BusInfo = NULL;
    uint32_t cnt = ARRAY_SIZE(I2sBusDirInfo);
    uint32_t i = 0;
    for(i = 0; i < cnt; i++) {
        if(I2sBusDirInfo[i].valid) {
            if(direction == I2sBusDirInfo[i].direction) {
                if(bus_role == I2sBusDirInfo[i].bus_role) {
                    BusInfo = &I2sBusDirInfo[i];
                }
            }
        }
    }
    return BusInfo;
}

const I2sBusDirInfo_t* I2S_DirRoleToInfo(const I2sDirRole_t i2s_bus_dir) {
    I2sBusDirInfo_t* BusInfo = NULL;
    uint32_t cnt = ARRAY_SIZE(I2sBusDirInfo);
    uint32_t i = 0;
    for(i = 0; i < cnt; i++) {
        if(I2sBusDirInfo[i].valid) {
            if(i2s_bus_dir == I2sBusDirInfo[i].i2s_bus_dir) {
                BusInfo = &I2sBusDirInfo[i];
            }
        }
    }
    return BusInfo;
}

I2sDirRole_t i2s_role_and_dir_to_dir_bus(const IfBusRole_t bus_role, const ConnectivitDir_t direction) {
    I2sDirRole_t bus_dir_role = I2S_DIR_BUS_MODE_UNDEF;
    const I2sBusDirInfo_t* BusInfo = I2sRoleAndDirToInfo(bus_role, direction);
    if(BusInfo) {
        bus_dir_role = BusInfo->i2s_bus_dir;
    }

    return bus_dir_role;
}

#ifdef HAS_I2S
bool i2s_common_init(uint8_t num) {
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        memset(Node, 0x00, sizeof(I2sHandle_t));
        Node->rx_rate.min = 0xFFFFFFFF;
    }
    return true;
}
#endif /*HAS_I2S*/

_WEAK_FUN_ bool i2s_sample_freq_get(uint8_t num, uint32_t* const audio_freq_hz) {
    bool res = false;
    I2sConfig_t* Config = I2sGetConfig(num);
    if(Config) {
        *audio_freq_hz = Config->audio_freq_hz;
        res = true;
    }
    return res;
}

_WEAK_FUN_ bool i2s_set_join_write(uint8_t num, uint32_t* array1, uint32_t* array2) { return false; }

_WEAK_FUN_ bool i2s_data_format_get(uint8_t num, I2sDataFormat_t* data_format) { return false; }

_WEAK_FUN_ bool i2s_dir_bus_role_set(uint8_t num, I2sDirRole_t bus_role) { return false; }
_WEAK_FUN_ bool i2s_dir_bus_role_get(uint8_t num, I2sDirRole_t* const bus_role) { return false; }

_WEAK_FUN_ bool i2s_config_tx(uint8_t num, I2sDataFormat_t word_size, uint8_t channels, AudioFreq_t audio_freq_hz) {
    return false;
}

_WEAK_FUN_ uint32_t i2s_get_sample_rate(uint8_t num) { return false; }
_WEAK_FUN_ bool i2s_dma_stop(uint8_t num) { return false; }
_WEAK_FUN_ bool i2s_api_write(uint8_t num, SampleType_t* const array, size_t array_len) { return false; }
_WEAK_FUN_ bool i2s_api_read(uint8_t num, SampleType_t* array, size_t array_len) { return false; }

bool i2s_play_tx(uint8_t num, uint8_t dac_num, bool status) {
    bool res = false;
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
#ifdef HAS_DDS
        Node->dac_num = 0;
        DdsHandle_t* DdsNode = DdsGetNode(dac_num);
        if(DdsNode) {
            LOG_DEBUG(I2S, "SpotDac %u", dac_num);
            Node->dac_num = dac_num;
            if(DdsNode->tx_sample_array && (0 < DdsNode->sample_cnt)) {
                LOG_DEBUG(I2S, "SpotRecord %u", dac_num);
                LOG_INFO(I2S, "Play %u samples", DdsNode->sample_cnt);

                Node->loopback = false;
                Node->play = status;
                if(Node->play) {
                    res = i2s_api_write(num, (SampleType_t*)DdsNode->tx_sample_array, DdsNode->sample_cnt);
                    LOG_INFO(I2S, "StartPlay");
                    if(res) {
                        LOG_INFO(I2S, "DmaTxOk %u Samples", DdsNode->sample_cnt);
                        res = true;
                    } else {
                        Node->error_cnt++;
                        LOG_ERROR(I2S, "DmaTxErr");
                    }
                } else {
                    res = true;
                }
            } else {
                LOG_ERROR(DDS, "TrackErr");
            }
        } else {
            LOG_ERROR(DDS, "HandleErr");
        }
#else
        LOG_ERROR(I2S, "Add DDS component");
#endif /*HAS_DDS*/
    } else {
        LOG_ERROR(I2S, "HandleErr");
    }
    return res;
}

bool i2s_play_static_tx(uint8_t num, uint8_t dac_num, bool status) {
    bool res = false;
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        I2sDirRole_t bus_role = I2S_DIR_BUS_MODE_UNDEF;
        res = i2s_dir_bus_role_get(num, &bus_role);
        if(I2S_DIR_BUS_MODE_MASTER_TX != bus_role) {
            res = i2s_dir_bus_role_set(num, I2S_DIR_BUS_MODE_MASTER_TX);
        }
#ifdef HAS_DDS
        Node->dac_num = 0;
        DdsHandle_t* DdsNode = DdsGetNode(dac_num);
        if(DdsNode) {
            LOG_DEBUG(I2S, "SpotDac:%u,Play:48 Sam", dac_num);
            Node->dac_num = dac_num;
            Node->loopback = false;
            Node->play = status;
            if(Node->play) {
                res = i2s_api_write(num, (SampleType_t*)DdsNode->tx_sample_array, 48 * 2);
                LOG_INFO(I2S, "StartPlay1KHz");
                if(res) {
                    LOG_INFO(I2S, "DmaTxOk,%u Sam", DdsNode->sample_cnt);
                    res = true;
                } else {
                    LOG_ERROR(I2S, "DmaTxErr,%s");
                    Node->error_cnt++;
                }
            } else {
                res = true;
            }
        } else {
            LOG_ERROR(DDS, "HandleErr");
        }
#else
        LOG_ERROR(I2S, "AddDdsComponent");
#endif /*HAS_DDS*/
    } else {
        LOG_ERROR(I2S, "HandleErr");
    }
    return res;
}

_WEAK_FUN_ uint8_t i2s_sample_size_get(uint8_t num) { return false; }
_WEAK_FUN_ bool i2s_init_custom(void) { return false; }
_WEAK_FUN_ bool i2s_init_one(uint8_t num) { return false; }
_WEAK_FUN_ bool i2s_read_write(uint8_t num, uint32_t tx_sample) { return false; }
_WEAK_FUN_ bool i2s_loopback_shared_memory(uint8_t num, size_t words_num) { return false; }
_WEAK_FUN_ bool i2s_read_sample(uint8_t num, size_t size) { return false; }
_WEAK_FUN_ bool i2s_send(uint8_t num, bool status) { return false; }
_WEAK_FUN_ bool i2s_loopback(uint8_t num, size_t words_num) { return false; }

bool i2s_init_common(const I2sConfig_t* const Config, I2sHandle_t* const Node) {
    bool res = false;
    if(Config) {
        if(Node) {
#ifdef HAS_I2S_DMA
            Node->Dma = Config->Dma;
#endif

            Node->mode = Config->mode;
            Node->debug_led1_num = Config->debug_led1_num;
            Node->debug_led2_num = Config->debug_led2_num;
            Node->PadDebug2 = Config->PadDebug2;
            Node->PadDebug1 = Config->PadDebug1;
            Node->mode = Config->mode;
            Node->sample_mode = Config->sample_mode;
            Node->irq_priority = Config->irq_priority;
            Node->samples_cnt = Config->samples_cnt;
            Node->bus_role = Config->bus_role;
            Node->data_format = Config->data_format;
            Node->audio_freq_hz = Config->audio_freq_hz;
#ifdef HAS_I2S_DMA
            const I2sInfo_t* Info = I2sGetInfo(Config->num);
            if(Info) {
                Node->CallBackTxHalf = Info->CallBackTxHalf;
                Node->CallBackTxDone = Info->CallBackTxDone;
                Node->CallBackRxHalf = Info->CallBackRxHalf;
                Node->CallBackRxDone = Info->CallBackRxDone;
            }
#endif
            Node->valid = true;
            memset(Node->RxBuffer, 0, sizeof(Node->RxBuffer));
            memset(Node->TxBuffer, 0, sizeof(Node->TxBuffer));
            memset(&Node->Tx, 0, sizeof(Node->Tx));
            memset(&Node->Rx, 0, sizeof(Node->Rx));
            Node->rec = false;
            Node->state = I2S_STATE_IDLE;
            Node->play = false;
            Node->proc_on = true;
#ifdef HAS_DFT
            Node->dft_mem = Config->dft_mem;
#endif
            res = true;
        }
    }
    return res;
}

#ifdef HAS_MCU
bool i2s_calc_byte_rate(void) {
    bool res = false;
    uint8_t num = 0;
    for(num = 0; num < i2s_get_cnt(); num++) {
        I2sHandle_t* Node = I2sGetNode(num);
        if(Node) {
            Node->rx_rate.cur = Node->cnt.byte_rx - Node->cnt_prev.byte_rx;
            Node->rx_rate.min = min32u(Node->rx_rate.min, Node->rx_rate.cur);
            Node->rx_rate.max = max32u(Node->rx_rate.max, Node->rx_rate.cur);
            Node->cnt_prev.byte_rx = Node->cnt.byte_rx;

            Node->tx_rate.cur = Node->cnt.byte_tx - Node->cnt_prev.byte_tx;
            Node->tx_rate.min = min32u(Node->tx_rate.min, Node->tx_rate.cur);
            Node->tx_rate.max = max32u(Node->tx_rate.max, Node->tx_rate.cur);
            Node->cnt_prev.byte_tx = Node->cnt.byte_tx;
        }
        res = true;
    }
    return res;
}
#endif

bool i2s_is_init(uint8_t num) {
    bool res = false;
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        res = Node->init_done;
    }
    return res;
}

bool i2s_is_allowed(uint8_t num) {
    bool res = false;
    const I2sConfig_t* Config = I2sGetConfig(num);
    if(Config) {
        I2sHandle_t* Node = I2sGetNode(num);
        if(Node) {
            res = true;
        }
    }

    return res;
}

uint32_t I2sByteToSample(uint32_t byte) {
    uint32_t sample = 0;
    sample = byte / sizeof(SampleType_t);
    return sample;
}

uint32_t i2s_err_total(I2sErr_t* Err) {
    uint32_t cnt = 0;
    if(Err) {
        cnt = Err->next + Err->read + Err->start + Err->write;
    }
    return cnt;
}

#ifdef HAS_DFT
_WEAK_FUN_ bool i2s_calc_dft(uint8_t num) {
    bool res = false;
    log_level_t ll = log_level_get(DFT);
    LOG_INFO(DFT, "I2S%u CalcDft", num);
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        res = i2s_sample_freq_get(num, &Node->audio_freq_hz);
        if(res) {
            double sample_period_s = 1.0 / ((double)Node->audio_freq_hz);
            uint32_t sample_cnt = I2S_BUFFER_SIZE / 2;
            LOG_INFO(DFT, "SampleFreq:%u Hz,SamplePeriod:%f us,N:%u Sam", Node->audio_freq_hz,
                     SEC_2_USEC(sample_period_s), sample_cnt);
            array_i16_decimation_2(Node->RxBuffer, I2S_BUFFER_SIZE);
            res = dft_calc((SampleType_t*)Node->RxBuffer, sample_cnt, Node->dft_mem, sample_period_s);
            if(res) {
                LOG_INFO(DFT, "CalcOk");
            } else {
                LOG_ERROR(DFT, "CalcErr");
            }

            uint32_t g = ft_find_freq(Node->dft_mem, sample_cnt, sample_period_s, &Node->maxFreq);
            (void)g;

            if(LOG_LEVEL_DEBUG == ll) {
#ifdef HAS_COMPLEX_DIAG
                complex_array_print(Node->dft_mem, sample_cnt, sample_period_s, sample_cnt);
#endif /*HAS_COMPLEX_DIAG*/
            }
            // complex_array_print_csv(Node->dft_mem, Node->samples_cnt, sample_period_s, Node->samples_cnt);
#ifdef HAS_COMPLEX_DIAG
            LOG_INFO(I2S, "MaxGarm:%s", ComplexToStr(Node->dft_mem[g]));
#endif /*HAS_COMPLEX_DIAG*/
        }
    }
    return res;
}
#endif /*HAS_DFT*/

_WEAK_FUN_ bool i2s_play_1khz(uint8_t num, uint8_t dac_num, SampleType_t amplitude, uint32_t phase_ms) {
    bool res = false;
    LOG_INFO(I2S, "I2S:%u,DAC:%u,Amp:%d,Phase:%u Ms", num, dac_num, amplitude, phase_ms);
#ifdef HAS_DDS
    res = dds_set_sin(dac_num, 1000.0, amplitude, (double)phase_ms, 0);
    if(res) {
        res = dds_set_static_period_48k_1000hz_word(dac_num);
        DdsHandle_t* DdsNode = DdsGetNode(dac_num);
        if(DdsNode) {
            res = false;
            res = i2s_api_write(num, DdsNode->tx_sample_array, DdsNode->sample_cnt);
            if(res) {
                LOG_DEBUG(I2S, "I2S%u,PlayStart...", num); // hang on
            }
        } else {
            LOG_ERROR(I2S, "DAC%u Err", dac_num);
        }
    } else {
        LOG_ERROR(I2S, "SetSin DAC%u,Amp:%d,Phase:%u Err", dac_num, amplitude, phase_ms);
    }
#endif /*HAS_DDS*/
    return res;
}

bool i2s_wait_idle(uint8_t num) {
    bool res = true;
    uint32_t up_time_ms = 0;
    uint32_t diff_ms = 0;
    uint32_t start_uptime_ms = time_get_ms32();
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        bool loop = true;
        while(loop) {
            res = wait_in_loop_ms(100);
            if(I2S_STATE_IDLE == Node->state) {
                loop = false;
                res = true;
            }

            up_time_ms = time_get_ms32();
            diff_ms = up_time_ms - start_uptime_ms;
            if(20000 < diff_ms) {
                loop = false;
                res = false;
            }
        }
    }
    return res;
}

bool i2s_proc_ctrl(uint8_t num, bool on_off) {
    bool res = false;
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        Node->proc_on = on_off;
        res = true;
    }
    return res;
}

_WEAK_FUN_
bool i2s_api_start(uint8_t num, size_t array_len) {
    bool res = false;
    return res;
}

_WEAK_FUN_ bool i2s_proc_one(uint8_t num) {
    bool res = true;
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        if(Node->proc_on) {

#ifdef HAS_I2S_VOLUME
            res = i2s_volume_proc_ll(Node);
#endif

            uint32_t up_time = time_get_ms32();
            switch((uint8_t)Node->state) {
            case I2S_STATE_RUN: {
                if(Node->play_off_time_stamp_ms < up_time) {
                    res = i2s_stop(num);
                    LOG_DEBUG(I2S, "PlayStop");
                }

            } break;
            case I2S_STATE_REC: {
                if(Node->rec_off_time_stamp_ms < up_time) {
                    res = i2s_stop(num);
                    log_level_t ll = log_level_get(I2S);
                    LOG_DEBUG(I2S, "RecStop");

#ifdef HAS_ARRAY_DIAG
                    if(LOG_LEVEL_DEBUG == ll) {
                        i2s_print_rx_ll(Node);
                    }
                    // i2s_stream_diag(num);
#endif
                }

            } break;
            } // switch
        }
    }

#if 0
        if(Node->tx_int) {
            uint32_t i = 0;
            Node->tx_int = false;
            if(Node->switch_needed) {
                switch(Node->cur_tx_track) {
                case 0:{
                    for(i=0; i<Node->frames; i++){
                        Node->TxBuffer[1][i] = Node->RxBuffer[1][i];
                    }
                    //memcpy(&(Node->TxBuffer[0][0]),&(Node->RxBuffer[0][0]),Node->frames*4);
                    res = true;
                } break;
                case 1:{
                    for(i=0; i<Node->frames; i++){
                        Node->TxBuffer[0][i] = Node->RxBuffer[0][i];
                    }
                    //memcpy(&(Node->TxBuffer[1][0]),&(Node->RxBuffer[1][0]),Node->frames*4);
                    res = true;
                } break;
                }
            }
        }
#endif

    return res;
}

bool i2s_play_tone(uint8_t num, uint8_t dac_num, double freq, SampleType_t amplitude, uint32_t duration_ms) {
    bool res = false;
    if(0.0 < freq) {
        if(0 < amplitude) {
            if(duration_ms) {
                LOG_INFO(I2S, "Play,Amplitude:%d,Freq:%f Hz,Durat:%ums", amplitude, freq, duration_ms);
                I2sHandle_t* Node = I2sGetNode(num);
                if(Node) {
                    uint32_t fps = 0;
                    fps = i2s_get_sample_rate(num);
                    LOG_INFO(I2S, "SampleRate: %u Hz", fps);
                    res = dds_set_sin(dac_num, freq, amplitude, 0, (SampleType_t)0);
                    if(res) {
                        LOG_INFO(I2S, "CfgDacOk");
                    } else {
                        LOG_ERROR(I2S, "CfgDacErr");
                    }

                    res = dds_set_array(dac_num, 2, 0);
                    if(res) {
                        LOG_INFO(I2S, "CalcDataOk");
                    } else {
                        LOG_ERROR(I2S, "CalcDataErr");
                    }

#ifdef HAS_DDS_DIAG
                    log_level_t ll = log_level_get(DDS);
                    if(LOG_LEVEL_DEBUG == ll) {
                        dds_diag();
                        dds_print_track(dac_num);
                    }
#endif

                    res = i2s_play_tx(num, dac_num, true);
                    if(res) {
                        LOG_INFO(I2S, "PlayOk");
                    } else {
                        LOG_ERROR(I2S, "PlayErr");
                    }

                    uint32_t up_time = time_get_ms32();
                    Node->play_off_time_stamp_ms = up_time + duration_ms;
                    res = true;
                } else {
                    LOG_ERROR(I2S, "NodeErr");
                }
            } else {
                LOG_ERROR(I2S, "DurationErr");
            }
        } else {
            LOG_ERROR(I2S, "AmpErr");
        }
    } else {
        LOG_ERROR(I2S, "FreqErr");
    }
    return res;
}
// TODO replace i2s_listen -> i2s_rec
bool i2s_listen(uint8_t num, uint32_t des_duration_ms) {
    bool res = false;
    // log_level_t ll=log_level_get(  I2S);
    LOG_WARNING(I2S, "Listen:%u,Dur:%u ms", num, des_duration_ms);
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        res = i2s_sample_freq_get(num, &Node->audio_freq_hz);
        if(res) {
            double sample_time_s = 1.0 / ((double)Node->audio_freq_hz);
            Node->max_diration_s = sample_time_s * ((double)I2S_BUFFER_SIZE) / 2.0;
            LOG_INFO(I2S, "SampleTime:%s", DoubleToStr(sample_time_s));
            LOG_INFO(I2S, "RecDuration:%s", DoubleToStr(Node->max_diration_s));
            double parts_d = MSEC_2_SEC(des_duration_ms) / Node->max_diration_s;
            uint32_t parts_i = (uint32_t)parts_d;
            LOG_INFO(I2S, "Parts:%f=%u", parts_d, parts_i);
            uint32_t duration_ms = parts_i * SEC_2_MSEC(Node->max_diration_s);
            LOG_WARNING(I2S, "SamSize:%u byte,Listen %u ms,%u Samples", sizeof(SampleType_t), duration_ms,
                        I2S_BUFFER_SIZE);

            res = i2s_api_read(num, (SampleType_t*)Node->RxBuffer, I2S_BUFFER_SIZE);
            if(res) {
                Node->state = I2S_STATE_REC;
                uint32_t up_time = time_get_ms32();
                Node->rec_off_time_stamp_ms = up_time + duration_ms;
            } else {
                LOG_ERROR(I2S, "I2S%u,ReadErr", num);
            }
        }
    } else {
        LOG_ERROR(I2S, "I2S%u NodeErr", num);
    }

    return res;
}

COMPONENT_INIT_PATTERT_CNT(I2S, I2S, i2s, I2S_COUNT)

COMPONENT_PROC_PATTERT_CNT(I2S, I2S, i2s, I2S_COUNT)
