#ifndef CAN_COMMANDS_H
#define CAN_COMMANDS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "std_includes.h"
#include "debug_info.h"
#include "ostream.h"

#ifndef HAS_CAN
#error "+HAS_CAN"
#endif

#ifndef HAS_MICROCONTROLLER
#error "+HAS_MICROCONTROLLER"
#endif

#ifdef HAS_CAN_CUSTOM
#include "can_custom_commands.h"
#else
#define CAN_CUSTOM_COMMANDS
#endif


#ifdef HAS_CAN_FD
bool can_fd_send_command(int32_t argc, char* argv[]);
bool can_fd_diag_command(int32_t argc, char* argv[]);

#define CAN_FD_COMMANDS                                                                   \
        SHELL_CMD("can_fd_send", "cfs", can_fd_send_command, "CanFdSendHexStr"),          \
        SHELL_CMD("can_fd_diag", "cafd", can_fd_diag_command, "CanFdDiag"),

#else
#define CAN_FD_COMMANDS
#endif

bool can_bit_rate_command(int32_t argc, char* argv[]);
bool can_diag_command(int32_t argc, char* argv[]);
bool can_init_command(int32_t argc, char* argv[]);
bool can_mesg_buff_command(int32_t argc, char* argv[]);
bool can_send_command(int32_t argc, char* argv[]);
bool can_loopback_command(int32_t argc, char* argv[]);

#define CAN_COMMANDS                                                             \
        CAN_FD_COMMANDS                                                          \
        CAN_CUSTOM_COMMANDS                                                      \
        SHELL_CMD("can_loopback", "clb", can_loopback_command, "CanLoopBack"),   \
        SHELL_CMD("can_mesg_buff", "cmb", can_mesg_buff_command, "CanMesgBuf"),  \
        SHELL_CMD("can_baud", "cb", can_bit_rate_command, "CanBaud"),            \
        SHELL_CMD("can_diag", "cad", can_diag_command, "CanDiag"),               \
        SHELL_CMD("can_init", "ci", can_init_command, "CanInit"),                \
        SHELL_CMD("can_send", "cs", can_send_command, "CanSendHexStr"),

#ifdef __cplusplus
}
#endif

#endif /* CAN_COMMANDS_H */
