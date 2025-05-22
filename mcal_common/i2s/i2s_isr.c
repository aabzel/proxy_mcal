#include "i2s_isr.h"

#include <string.h>

#include "i2s_mcal.h"
/*tx*/
bool I2sTxHalfCallback(I2sHandle_t* const Node) {
    bool res = false;
    if(Node) {
        Node->tx_half = true;
        Node->tx_half_cnt++;
        res = true;
    }
    return res;
}

bool I2sTxDoneCallback(I2sHandle_t* const Node) {
    bool res = false;
    if(Node) {
        Node->tx_done = true;
        Node->tx_done_cnt++;
        res = true;
    }
    return res;
}

bool I2sDmaCallBackTxHalf(uint8_t num) {
    bool res = false;
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        res = I2sTxHalfCallback(Node);
    }
    return res;
}

bool I2sDmaCallBackTxDone(uint8_t num) {
    bool res = false;
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        res = I2sTxDoneCallback(Node);
    }
    return res;
}

/*Rx*/

bool I2sRxHalfCallback(I2sHandle_t* const Node) {
    bool res = false;
    if(Node) {
        Node->rx_half = true;
        Node->rx_half_cnt++;
        res = true;
    }
    return res;
}

bool I2sRxDoneCallback(I2sHandle_t* const Node) {
    bool res = false;
    if(Node) {
        Node->rx_done = true;
        Node->rx_done_cnt++;
#if 0
        memcpy((void*)&Node->rx_sample_fixed.u8[0], (void*)&Node->rx_sample.u8[0], 8);
        if(Node->echo) {
            memcpy((void*)&Node->tx_sample.u8[0], (void*)&Node->rx_sample.u8[0], sizeof(Node->tx_sample));
        }
#endif
        res = true;
    }
    return res;
}

bool I2sDmaCallBackRxHalf(uint8_t num) {
    bool res = false;
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        res = I2sRxHalfCallback(Node);
    }
    return res;
}

bool I2sDmaCallBackRxDone(uint8_t num) {
    bool res = false;
    I2sHandle_t* Node = I2sGetNode(num);
    if(Node) {
        res = I2sRxDoneCallback(Node);
    }
    return res;
}

/*Err*/

bool I2sErrorCallback(I2sHandle_t* const Node) {
    bool res = false;
    if(Node) {
        Node->error_cnt++;
        res = true;
    }
    return res;
}

/*RxTx*/
bool I2sTxRxHalfCallback(I2sHandle_t* const Node) {
    bool res = false;
    if(Node) {
        I2sTxHalfCallback(Node);
        I2sRxHalfCallback(Node);
        res = true;
    }
    return res;
}

bool I2sTxRxDoneCallback(I2sHandle_t* const Node) {
    bool res = false;
    if(Node) {
        I2sTxDoneCallback(Node);
        I2sRxDoneCallback(Node);
        res = true;
    }
    return res;
}

#ifdef HAS_I2S2
bool I2s2DmaCallBackTxHalf(void) {
    bool res = I2sDmaCallBackTxHalf(2);
    return res;
}

bool I2s2DmaCallBackTxDone(void) {
    bool res = I2sDmaCallBackTxDone(2);
    return res;
}

bool I2s2DmaCallBackRxDone(void) {
    bool res = I2sDmaCallBackRxDone(2);
    return res;
}

bool I2s2DmaCallBackRxHalf(void) {
    bool res = I2sDmaCallBackRxHalf(2);
    return res;
}
#endif

#ifdef HAS_I2S5
bool I2s5DmaCallBackTxHalf(void) {
    bool res = I2sDmaCallBackTxHalf(5);
    return res;
}

bool I2s5DmaCallBackTxDone(void) {
    bool res = I2sDmaCallBackTxDone(5);
    return res;
}

bool I2s5DmaCallBackRxDone(void) {
    bool res = I2sDmaCallBackRxDone(5);
    return res;
}

bool I2s5DmaCallBackRxHalf(void) {
    bool res = I2sDmaCallBackRxHalf(5);
    return res;
}
#endif
