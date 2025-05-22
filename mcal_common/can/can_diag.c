#include "can_diag.h"

#include <stdio.h>
#include <string.h>

#include "array_diag.h"
#include "can_mcal.h"
#include "common_diag.h"
#include "diag_includes.h"
#include "log.h"
#include "mcal_diag.h"
#include "table_utils.h"
#include "writer_config.h"

const char* CanModeToStr(CanMode_t mode) {
    char* name = "?";
    switch(mode) {
    case CAN_CFG_MODE_COMMUNICATE:
        name = "Communicate";
        break;
    case CAN_CFG_MODE_LOOPBACK:
        name = "LoopBack";
        break;
    case CAN_CFG_MODE_LISTENONLY:
        name = "ListenOnly";
        break;
    case CAN_CFG_MODE_LISTENONLY_LOOPBACK:
        name = "ListenOnlyLoopBack";
        break;
    case CAN_CFG_MODE_FREEZE:
        name = "Freeze";
        break;
    default:
        name = "?";
        break;
    }
    return name;
}

const char* CanIdTypeToStr(CanIdentifier_t identifier) {
    char* name = "?";
    switch(identifier) {
    case CAN_FRAME_ID_STANDARD:
        name = "Std11bit";
        break;
    case CAN_FRAME_ID_EXTENDED:
        name = "Ext29bit";
        break;
    default:
        name = "Rem";
        break;
    }
    return name;
}

const char* CanFrameTypeToStr(CanTxFrame_t frame_type) {
    char* name = "?";
    switch((uint8_t)frame_type) {
    case CAN_TX_FRAME_DATA:
        name = "Dat";
        break;
    case CAN_TX_FRAME_REMOTE:
        name = "Rem";
        break;
    }
    return name;
}

const char* CanMessageToStr(const CanMessage_t* const Message) {
    static char text[150] = "";
    if(Message) {
        strcpy(text, "");
        snprintf(text, sizeof(text), "%sID:0x%x,", text, Message->identifier.extended);
        snprintf(text, sizeof(text), "%sIDType:%s", text, CanIdTypeToStr(Message->id_type));
        snprintf(text, sizeof(text), "%sFrame:%s", text, CanFrameTypeToStr(Message->frame_type));
        snprintf(text, sizeof(text), "%sSize:%u,", text, Message->size);
        snprintf(text, sizeof(text), "%sData:%s,", text, ArrayToStr(Message->data, Message->size));
    }
    return text;
}



const char* CanConfigTo2Str(const CanConfig_t* const Config) {
    static char text[150] = "";
    if(Config) {
        strcpy(text, "");
        snprintf(text, sizeof(text), "%sCAN%u,", text, Config->num);
        snprintf(text, sizeof(text), "%s%sBit/s,", text, Uint32ValToStr(Config->bit_rate));
        snprintf(text, sizeof(text), "%sFD:%sBit/s,", text, Uint32ValToStr(Config->bit_rate_fd));
        snprintf(text, sizeof(text), "%sRxID:0x%x,", text, Config->rx_id);
        snprintf(text, sizeof(text), "%sTxID:0x%x,", text, Config->tx_id);
        snprintf(text, sizeof(text), "%sFD:%s,", text, OnOffToStr(Config->fd_enable));
        snprintf(text, sizeof(text), "%sMoveMode:%s,", text, McalMoveModeToStr(Config->move_mode));
        snprintf(text, sizeof(text), "%sIDtype:%s,", text, CanIdTypeToStr(Config->identifier));
        snprintf(text, sizeof(text), "%sMode:%s", text, CanModeToStr(Config->mode));
    }
    return text;
}

bool CanDiagNode(const CanHandle_t* const Node) {
    bool res = false;
    if(Node) {
        LOG_INFO(CAN, "IsrCnt:%u", Node->isr_cnt);
        LOG_INFO(CAN, "IsrErrCnt:%u", Node->isr_err_cnt);
        LOG_INFO(CAN, "IsrRxCcnt:%u", Node->isr_rx_cnt);
        LOG_INFO(CAN, "IsrTxCnt:%u", Node->isr_tx_cnt);
        LOG_INFO(CAN, "ErrCode:%u", Node->err_code);
        LOG_INFO(CAN, "RxID:0x%x", Node->rx_id);
        res = true;
    }
    return res;
}

bool can_diag(void) {
    bool res = false;
    uint8_t num = 0;
    static const table_col_t cols[] = {
        {6, "CAN"}, {12, "REG"}, {8, "Rate"}, {7, "TEC"}, {7, "REC"},
    };
    table_header(&(curWriterPtr->stream), cols, ARRAY_SIZE(cols));

    for(num = 0; num < CAN_COUNT; num++) {
        char text[300] = {0};
        strcpy(text, TSEP);
        uint32_t bit_rate = 0;
        uint32_t rec = 0;
        uint32_t tec = 0;
        res = can_api_baudrate_get(num, &bit_rate);
        res = can_tec_get(num, &rec);
        res = can_rec_get(num, &tec);
        CanHandle_t* Node = CanGetNode(num);
        if(Node) {
            snprintf(text, sizeof(text), "%s CAN%u " TSEP, text, num);
            snprintf(text, sizeof(text), "%s 0x%08x " TSEP, text, Node->CANx);
            snprintf(text, sizeof(text), "%s %6u " TSEP, text, bit_rate);
            snprintf(text, sizeof(text), "%s %5u " TSEP, text, tec);
            snprintf(text, sizeof(text), "%s %5u " TSEP, text, rec);
            cli_printf("%s" CRLF, text);
            res = true;
        }
    }

    table_row_bottom(&(curWriterPtr->stream), cols, ARRAY_SIZE(cols));

    return res;
}

#ifdef HAS_CAN_FD
bool can_fd_diag(void) {
    bool res = false;
    uint8_t num = 0;
    static const table_col_t cols[] = {
        {6, "CAN"}, {12, "REG"}, {9, "rate"}, {7, "TEC"}, {7, "REC"},
    };
    table_header(&(curWriterPtr->stream), cols, ARRAY_SIZE(cols));

    for(num = 0; num < CAN_COUNT; num++) {
        char text[300] = {0};
        strcpy(text, TSEP);
        uint32_t bit_rate_fd = 0;
        uint32_t rec = 0;
        uint32_t tec = 0;
        res = can_fd_tec_get(num, &rec);
        res = can_fd_rec_get(num, &tec);
        res = can_fd_baudrate_get(num, &bit_rate_fd);
        CanHandle_t* Node = CanGetNode(num);
        if(Node) {
            snprintf(text, sizeof(text), "%s CAN%u " TSEP, text, num);
            snprintf(text, sizeof(text), "%s 0x%08x " TSEP, text, Node->CANx);
            snprintf(text, sizeof(text), "%s %5u " TSEP, text, bit_rate_fd);
            snprintf(text, sizeof(text), "%s %5u " TSEP, text, tec);
            snprintf(text, sizeof(text), "%s %5u " TSEP, text, rec);
            // res = CanDiagNode(Node);
            cli_printf("%s" CRLF, text);
            res = true;
        }
    }

    table_row_bottom(&(curWriterPtr->stream), cols, ARRAY_SIZE(cols));

    return res;
}
#endif
