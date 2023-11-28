########################################################################################################################
# <Setup build environment>
########################################################################################################################
set(PROJECT_NAME "ChassisController")

cmake_minimum_required(VERSION 3.26)
########################################################################################################################
# </Setup build environment>
########################################################################################################################

########################################################################################################################
# For running configuration in test mode (automated software testing) condition below should be met:
# ```if ("${CMAKE_BUILD_TYPE}" STREQUAL "TestsBuild")```
# so build type should be passed accordingly
########################################################################################################################

########################################################################################################################
# <Build types>
# Expand/modify as necessary (may be project specific)
########################################################################################################################
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
    add_definitions(-DRELEASE)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
    add_definitions(-DDEBUG -DDEBUG_LOG_RTT)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
    add_definitions(-DRELEASE)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
    add_definitions(-DDEBUG -DDEBUG_LOG_RTT)
endif ()
########################################################################################################################
# </Build types>
########################################################################################################################

########################################################################################################################
# <Common configuration>
########################################################################################################################
project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

########################################################################################################################
# </Common configuration>
########################################################################################################################

########################################################################################################################
# <Test configuration>
# Configuration for running tests (unit and other) on PC
########################################################################################################################
if ("${CMAKE_BUILD_TYPE}" STREQUAL "TestsBuild")
    set(CMAKE_C_COMPILER gcc)
    set(CMAKE_CXX_COMPILER g++)
########################################################################################################################
# </Test configuration>
# Configuration for running tests (unit and other) on PC
########################################################################################################################

########################################################################################################################
# <Cross compilation configuration>
# Platform specific configuration
########################################################################################################################
else()
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
    set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
    set(CMAKE_AR arm-none-eabi-ar)
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CMAKE_OBJDUMP arm-none-eabi-objdump)
    set(SIZE arm-none-eabi-size)
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

########################################################################################################################
    #Uncomment for hardware floating point
    add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
    add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
    add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

    #Uncomment for software floating point
    #add_compile_options(-mfloat-abi=soft)
########################################################################################################################

    add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
    add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
    # Enable assembler files preprocessing
    add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

    #Definitions passed to program during compilation time
    add_definitions(
            -DDEBUG
            -DUSE_HAL_DRIVER
            -DSTM32F401xC
            -DTX_INCLUDE_USER_DEFINE_FILE
    )

    #Modify as needed
    include_directories(
            Core/Inc

            Drivers/STM32F4xx_HAL_Driver/Inc
            Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
            Drivers/CMSIS/Device/ST/STM32F4xx/Include
            Drivers/CMSIS/Include

            AZURE_RTOS/App

            Middlewares/ST/threadx/common/inc/
            Middlewares/ST/threadx/ports/cortex_m4/gnu/inc/
            Middlewares/ST/threadx/utility/low_power/
    )

    #passing source files
    file(GLOB_RECURSE SOURCES
            "AZURE_RTOS/*.*"
            "ThreadX/*.*"
            "Core/*.*"
            "Middlewares/*.*"
            "Drivers/*.*"
    )

    set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F401CCUX_FLASH.ld)

    add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
    add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
    add_link_options(-T ${LINKER_SCRIPT})

    add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

    set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
    set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

    add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
            COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
            COMMENT "Building ${HEX_FILE}
    Building ${BIN_FILE}")
endif()