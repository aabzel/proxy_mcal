#ifndef I2S_GENERAL_DRIVER_H
#define I2S_GENERAL_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "std_includes.h"
#include "i2s_config.h"
#include "i2s_types.h"
#include "clock_const.h"
#include "i2s_isr.h"
#include "i2s_custom_drv.h"
#ifdef HAS_I2S_DIAG
#include "i2s_diag.h"
#endif

#ifdef HAS_I2S_DMA
#include "i2s_dma_mcal.h"
#endif

#define I2S_BYTES_TO_FRAMES(BYTE_SIZE, FRAME_SIZE) ((BYTE_SIZE)/(FRAME_SIZE))

/* API */
const I2sConfig_t* I2sGetConfig(uint8_t num);
const I2sInfo_t* I2sGetInfo(uint8_t num);
I2sHandle_t* I2sGetNode(uint8_t num);

bool i2s_proc_ctrl(uint8_t num, bool on_off);
bool i2s_init_common(const I2sConfig_t* const  Config, I2sHandle_t* const Node);
bool i2s_wait_idle(uint8_t num);
bool i2s_mcal_init(void);
bool i2s_init_role_one_ll(const I2sConfig_t* Config, I2sHandle_t* Node, I2sDirRole_t bus_role);
bool i2s_init_one(uint8_t num);
bool i2s_proc_one(uint8_t num);
bool i2s_proc(void);
bool i2s_init_custom(void) ;
uint32_t I2sByteToSample(uint32_t byte);

/*setters*/
bool i2s_api_start(uint8_t num, size_t array_len) ;
bool i2s_api_write(uint8_t num, SampleType_t* const array, size_t array_len);
bool i2s_dir_bus_role_set(uint8_t num, I2sDirRole_t bus_role) ;
bool i2s_clock_ctrl(ClockBus_t clock_bus, uint32_t clock_type, bool onOff) ;
bool i2s_ctrl(uint8_t num, bool on);
bool i2s_config_tx(uint8_t num, I2sDataFormat_t word_size, uint8_t channels, AudioFreq_t audio_freq);
bool i2s_clock_polarity_set(uint8_t num, Cpol_t clock_polarity) ;
bool i2s_data_format_set(uint8_t num, I2sDataFormat_t data_format);
bool i2s_master_clock_ctrl(uint8_t num, MclkOut_t mclk_out) ;
bool i2s_pin_mux(uint8_t num, I2sDirRole_t bus_role);
//bool i2s_interrupt_ctrl(uint8_t num, AT32_I2S_INTERRUPT i2s_interrupt, bool onOff) ;
bool i2s_dma_ctrl(uint8_t num, bool on_off);
bool i2s_dma_pause(uint8_t num);
bool i2s_dma_stop(uint8_t num);
bool i2s_loopback(uint8_t num, size_t words_num);
bool i2s_loopback_shared_memory(uint8_t num, size_t words_num);
bool i2s_play_tone(uint8_t num,uint8_t dac_num, double freq, SampleType_t amplitude, uint32_t duration_ms);
bool i2s_play_tx(uint8_t num, uint8_t dac_num, bool status);
bool i2s_play_static_tx(uint8_t num, uint8_t dac_num, bool status);
bool i2s_play_1khz(uint8_t num, uint8_t dac_num, SampleType_t amplitude, uint32_t phase_ms);
bool i2s_sample_freq_set(uint8_t num, AudioFreq_t audio_freq);
bool i2s_stop(uint8_t num);
bool i2s_standard_set(uint8_t num, Standard_t standard) ;
bool i2s_send(uint8_t num, bool status);
bool i2s_set_join_write(uint8_t num, uint32_t* array1, uint32_t* array2);
bool i2s_test(uint8_t num);

/*getters*/
bool i2s_api_read(uint8_t num, SampleType_t* array, size_t array_len);
bool i2s_data_format_get(uint8_t num, I2sDataFormat_t* data_format) ;
bool i2s_dir_bus_role_get(uint8_t num, I2sDirRole_t* const bus_dir_role);
bool i2s_calc_byte_rate(void);
bool i2s_calc_dft(uint8_t num);
bool i2s_is_init(uint8_t num);
bool i2s_is_allowed(uint8_t num);
bool i2s_listen(uint8_t num, uint32_t des_duration_ms) ;
bool i2s_read_sample(uint8_t num, size_t size);
bool i2s_read_write(uint8_t num, uint32_t tx_sample);
bool i2s_sample_freq_get(uint8_t num, uint32_t* const audio_freq);
uint32_t i2s_err_total(I2sErr_t* Err) ;
uint32_t i2s_get_sample_rate(uint8_t num);
uint32_t I2sAudioFreq2Hz(AudioFreq_t audio_freq);
//uint32_t i2s_err_total(I2sErr_t* Err);
uint8_t i2s_sample_size_get(uint8_t num);
const I2sBusDirInfo_t* I2S_DirRoleToInfo(const I2sDirRole_t i2s_bus_dir);
I2sDirRole_t i2s_role_and_dir_to_dir_bus(const IfBusRole_t bus_role,
                                    const ConnectivitDir_t dirrection);
#ifdef HAS_I2S_VOLUME
SampleType_t i2s_calc_pcm_max_ll( I2sHandle_t* const Node );
bool i2s_increase_pcms_ll(I2sHandle_t* const Node);
bool i2s_decrease_pcms_ll( I2sHandle_t* const Node);
#endif

#ifdef HAS_I2S
bool i2s_common_init(uint8_t num);
#endif /*HAS_I2S*/

#ifdef __cplusplus
}
#endif

#endif /* I2S_GENERAL_DRIVER_H */


