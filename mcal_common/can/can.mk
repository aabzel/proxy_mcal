$(info CAN_COMMON_MCAL_MK_INC=$(CAN_COMMON_MCAL_MK_INC) )

ifneq ($(CAN_COMMON_MCAL_MK_INC),Y)
    CAN_COMMON_MCAL_MK_INC=Y

    CAN_MCAL_DIR = $(MCAL_COMMON_DIR)/can
    #@echo $(error CAN_MCAL_DIR=$(CAN_MCAL_DIR))

    INCDIR += -I$(CAN_MCAL_DIR)
    OPT += -DHAS_CAN
    OPT += -DHAS_CAN_PROC

    SOURCES_C += $(CAN_MCAL_DIR)/can_general.c
    
    ifeq ($(CAN_HEARTBEAT),Y)
        OPT += -DHAS_CAN_HEARTBEAT_PROC
    endif

    ifeq ($(CAN_FD),Y)
        OPT += -DHAS_CAN_FD
        SOURCES_C += $(CAN_MCAL_DIR)/can_fd_general.c

        ifeq ($(CAN_FD_HEARTBEAT),Y)
            OPT += -DHAS_CAN_FD_HEARTBEAT_PROC
        endif

    endif

    ifeq ($(CAN0),Y)
        OPT += -DHAS_CAN0
    endif

    ifeq ($(CAN1),Y)
        OPT += -DHAS_CAN1
    endif

    ifeq ($(CAN2),Y)
        OPT += -DHAS_CAN2
    endif

    ifeq ($(CAN3),Y)
        OPT += -DHAS_CAN3
    endif
 
    ifeq ($(CAN4),Y)
        OPT += -DHAS_CAN4
    endif

    ifeq ($(CAN5),Y)
        OPT += -DHAS_CAN5
    endif

    ifeq ($(CAN_DIAG),Y)
        OPT += -DHAS_CAN_DIAG
        SOURCES_C += $(CAN_MCAL_DIR)/can_diag.c
    endif

    ifeq ($(CAN_COMMANDS),Y)
        OPT += -DHAS_CAN_COMMANDS
        SOURCES_C += $(CAN_MCAL_DIR)/can_commands.c
    endif
endif



