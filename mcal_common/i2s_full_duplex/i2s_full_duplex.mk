$(info I2S_FULL_DUPLEX_GENERAL_MK_INC=  $(I2S_FULL_DUPLEX_GENERAL_MK_INC) )
ifneq ($(I2S_FULL_DUPLEX_GENERAL_MK_INC),Y)
    I2S_FULL_DUPLEX_GENERAL_MK_INC=Y

    I2S_FULL_DUPLEX_MCAL_DIR = $(MCAL_COMMON_DIR)/i2s_full_duplex
    #@echo $(error I2S_FULL_DUPLEX_MCAL_DIR=$(I2S_FULL_DUPLEX_MCAL_DIR))
    OPT += -DHAS_I2S_FULL_DUPLEX
    OPT += -DHAS_I2S_FULL_DUPLEX_PROC

    ifeq ($(I2S_FULL_DUPLEX_DEBUG),Y)
        OPT += -DHAS_I2S_FULL_DUPLEX_DEBUG
    endif

    INCDIR += -I$(I2S_FULL_DUPLEX_MCAL_DIR)

    I2S_FULL_DUPLEX_SELF_SYNC=Y
    ifeq ($(I2S_FULL_DUPLEX_SELF_SYNC),Y)
        OPT += -DHAS_I2S_FULL_DUPLEX_SELF_SYNC
    endif
    
    SOURCES_C += $(I2S_FULL_DUPLEX_MCAL_DIR)/i2s_full_duplex.c
    SOURCES_C += $(I2S_FULL_DUPLEX_MCAL_DIR)/i2s_full_duplex_isr.c

    ifeq ($(DIAG),Y)
        ifeq ($(I2S_FULL_DUPLEX_DIAG),Y)
            OPT += -DHAS_I2S_FULL_DUPLEX_DIAG
            SOURCES_C += $(I2S_FULL_DUPLEX_MCAL_DIR)/i2s_full_duplex_diag.c
        endif
    endif

    ifeq ($(CLI),Y)
        ifeq ($(I2S_FULL_DUPLEX_COMMANDS),Y)
            OPT += -DHAS_I2S_FULL_DUPLEX_COMMANDS
            SOURCES_C += $(I2S_FULL_DUPLEX_MCAL_DIR)/i2s_full_duplex_commands.c
        endif
    endif
endif