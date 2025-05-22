#ifndef CAN_DIAG_H
#define CAN_DIAG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "can_types.h"
#ifdef HAS_CAN_CUSTOM_DIAG
#include "can_custom_diag.h"
#endif

const char* CanModeToStr( CanMode_t mode);
const char* CanConfigTo2Str(const CanConfig_t* const Config);
const char* CanIdTypeToStr(CanIdentifier_t id_type);
const char* CanFrameTypeToStr(CanTxFrame_t frame_type);
const char* CanMessageToStr(const CanMessage_t* const Message);
bool CanDiagNode(const CanHandle_t* const Node);
bool can_fd_diag(void);
bool can_diag(void);

#ifdef __cplusplus
}
#endif

#endif /* CAN_DIAG_H */
