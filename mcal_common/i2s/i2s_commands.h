#ifndef I2S_GENERAL_COMMANDS_H
#define I2S_GENERAL_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "cli_drv.h"

#ifdef HAS_I2S_CUSTOM_COMMANDS
#include "i2s_custom_commands.h"
#else
#define I2S_CUSTOM_COMMANDS
#endif

#ifndef HAS_I2S
#error "+ HAS_I2S"
#endif

#ifndef HAS_LOG
#error "+ HAS_LOG"
#endif

#ifndef HAS_CLI
#error "+ HAS_CLI"
#endif

bool i2s_set_dac_command(int32_t argc, char* argv[]);
bool i2s_bus_role_command(int32_t argc, char* argv[]);
bool i2s_test_command(int32_t argc, char* argv[]);
bool i2s_config_command(int32_t argc, char* argv[]);
bool i2s_dma_pause_command(int32_t argc, char* argv[]);
bool i2s_dma_stop_command(int32_t argc, char* argv[]);
bool i2s_ctrl_command(int32_t argc, char* argv[]);
bool i2s_diag_command(int32_t argc, char* argv[]);
bool i2s_diag_configs_command(int32_t argc, char* argv[]);
bool i2s_stream_diag_command(int32_t argc, char* argv[]);
bool i2s_diag_errors_command(int32_t argc, char* argv[]);
bool i2s_diag_sample_command(int32_t argc, char* argv[]);
bool i2s_set_iir_command(int32_t argc, char* argv[]);
//bool i2s_set_play_command(int32_t argc, char* argv[]);
bool i2s_set_loopback_command(int32_t argc, char* argv[]);
bool i2s_set_echo_command(int32_t argc, char* argv[]);
bool i2s_init_command(int32_t argc, char* argv[]);
bool i2s_diag_rx_command(int32_t argc, char* argv[]);

#ifdef HAS_DDS
bool i2s_set_dac_num_command(int32_t argc, char* argv[]);

#define I2S_DAC_COMMANDS                                                                           \
        SHELL_CMD("i2s_dac_num", "i2sdn", i2s_set_dac_num_command, "I2sSetDacNum"),
#else
#define I2S_DAC_COMMANDS
#endif

#define I2S_DIAG_COMMANDS                                                                                              \
    SHELL_CMD("i2s_diag_cfg", "i2sdc", i2s_diag_configs_command, "I2sDiagConfig"),                                     \
    SHELL_CMD("i2s_diag", "i2sd", i2s_diag_command, "I2sDiag"),                                                        \
    SHELL_CMD("i2s_rx_diag", "i2sdx", i2s_diag_rx_command, "I2sDiagRx"),                                                        \
    SHELL_CMD("i2s_diag_err", "i2ser", i2s_diag_errors_command, "I2sDiagErrors"),                                      \
    SHELL_CMD("i2s_stream_diag", "i2ssd", i2s_stream_diag_command, "I2sStreamDiag"),                                   \
    SHELL_CMD("i2s_diag_sample", "i2sa", i2s_diag_sample_command, "I2sDiagSample"),


bool i2s_read_sample_command(int32_t argc, char* argv[]);
bool i2s_read_command(int32_t argc, char* argv[]);
bool i2s_read_write_command(int32_t argc, char* argv[]);
bool i2s_rec_reverse_command(int32_t argc, char* argv[]);
bool i2s_rec_show_command(int32_t argc, char* argv[]);
bool i2s_calc_dft_command(int32_t argc, char* argv[]) ;
bool i2s_listen_command(int32_t argc, char* argv[]);

#define I2S_RECORD_COMMANDS                                                                              \
    SHELL_CMD("i2s_calc_dft", "i2sdf", i2s_calc_dft_command, "I2sCalcDft"),                              \
    SHELL_CMD("i2s_listen", "i2sli", i2s_listen_command, "I2sListen"),                                   \
    SHELL_CMD("i2s_rec_show", "i2sre", i2s_rec_show_command, "I2sRecShow"),                              \
    SHELL_CMD("i2s_rec_reverce", "i2srr", i2s_rec_reverse_command, "I2sRecRev"),                              \
    SHELL_CMD("i2s_read_sample", "i2srs", i2s_read_sample_command, "I2sReadSample "),                    \
    SHELL_CMD("i2s_read", "i2srd", i2s_read_command, "I2sRead "),                                        \
    SHELL_CMD("i2s_txrx", "i2srw", i2s_read_write_command, "I2sReadWrite"),                              \


bool i2s_dma_resume_command(int32_t argc, char* argv[]);
bool i2s_set_tone_command(int32_t argc, char* argv[]);
bool i2s_write_command(int32_t argc, char* argv[]);
bool i2s_play_tone_command(int32_t argc, char* argv[]);
bool i2s_play_command(int32_t argc, char* argv[]);

#define I2S_PLAY_COMMANDS                                                                                          \
    SHELL_CMD("i2s_tone", "i2sn", i2s_set_tone_command, "I2sTone"),                                                \
    SHELL_CMD("i2s_play", "i2spl", i2s_play_command, "I2sSetPlay"),                                                \
    SHELL_CMD("i2s_test", "i2st", i2s_test_command, "I2sTest"),                                                    \
    SHELL_CMD("i2s_play_tone", "i2spt", i2s_play_tone_command, "I2sPlayTone1kHz"),                                 \
    SHELL_CMD("i2s_write", "i2sw", i2s_write_command, "I2sWriteHexStr"),                                           \
    SHELL_CMD("i2s_echo", "i2se", i2s_set_echo_command, "I2sEcho"),


#define I2S_COMMANDS                                                                                                   \
    I2S_CUSTOM_COMMANDS                                                                                                \
    I2S_DIAG_COMMANDS                                                                                                  \
    I2S_RECORD_COMMANDS                                                                                                \
    I2S_PLAY_COMMANDS                                                                                                  \
    I2S_DAC_COMMANDS                                                                                                   \
    SHELL_CMD("i2s_loop", "i2sl", i2s_set_loopback_command, "I2sLoopBack"),                                            \
    SHELL_CMD("i2s_iir", "i2sii", i2s_set_iir_command, "I2sIrr"),                                                      \
    SHELL_CMD("i2s_bus_role", "i2sbr", i2s_bus_role_command, "I2sBusRole"),                                            \
    SHELL_CMD("i2s_dma_pause", "i2sdp", i2s_dma_pause_command, "I2sDmaPause"),                                         \
    SHELL_CMD("i2s_dma_stop", "i2sds", i2s_dma_stop_command, "I2sDmaStop"),                                            \
    SHELL_CMD("i2s_ctrl", "i2sct", i2s_ctrl_command, "I2sCtrl"),                                                       \
    SHELL_CMD("i2s_dma_resume", "i2sdr", i2s_dma_resume_command, "I2sDmaResume"),                                      \
    SHELL_CMD("i2s_config", "i2sc", i2s_config_command, "I2sConfig"),                                                  \
    SHELL_CMD("i2s_init", "i2si", i2s_init_command, "I2sInit"),

#ifdef __cplusplus
}
#endif

//        SHELL_CMD("i2s_set_play", "i2sp", i2s_set_play_command, "I2sPlay"),
#endif /* I2S_GENERAL_COMMANDS_H */
