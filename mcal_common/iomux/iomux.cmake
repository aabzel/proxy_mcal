#add_library(iomux)
message(STATUS "IOMUX_GENERAL_MK_INC=${IOMUX_GENERAL_MK_INC}")
if( NOT (IOMUX_GENERAL_MK_INC STREQUAL Y))
    set(IOMUX_GENERAL_MK_INC Y)
    message(STATUS "+ IOMUX General")

    set(IOMUX_GENERAL_DIR ${MCAL_COMMON_DIR}/iomux)
    message(STATUS "IOMUX_GENERAL_DIR=${IOMUX_GENERAL_DIR}")

    include_directories(${IOMUX_GENERAL_DIR})
    target_include_directories(app PUBLIC ${IOMUX_GENERAL_DIR})
    target_compile_definitions(app PUBLIC HAS_IOMUX)

    add_compile_definitions(HAS_IOMUX)
    #add_compile_options(-DHAS_IOMUX)
    target_sources(app PRIVATE ${IOMUX_GENERAL_DIR}/iomux_general_drv.c)

    if(DIAG STREQUAL Y)
        if(IOMUX_DIAG STREQUAL Y)
            message(STATUS "+ IOMUX Diag")
            target_compile_definitions(app PUBLIC HAS_IOMUX_DIAG)
            target_sources(app PRIVATE ${IOMUX_GENERAL_DIR}/iomux_diag.c)
        endif()
    endif()

    if(CLI STREQUAL Y)
        if(IOMUX_COMMANDS STREQUAL Y)
            message(STATUS "+ IOMUX Commands")
            target_compile_definitions(app PUBLIC HAS_IOMUX_COMMANDS)
            target_sources(app PRIVATE ${IOMUX_GENERAL_DIR}/iomux_commands.c)
        endif()
    endif()

    #target_include_directories(iomux PUBLIC ${IOMUX_GENERAL_DIR})
endif()
