########################################################################################################################
# Subproject for adding IC manufacturer files (HAL's, LL's, CMSIS)
########################################################################################################################
cmake_minimum_required(VERSION 3.22)
project(IcManufacturerDrivers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

########################################################################################################################
# Hardware specific definitions
########################################################################################################################
# add_definitions()

########################################################################################################################
# Directory for subproject for test purposes
########################################################################################################################
#add_subdirectory(IcManufacturerDriversWrappers)

file(GLOB SOURCES
        "stm32f4xx_hal_driver/Src/*.c"
)

set(INCLUDES_PUBLIC
        ./
        ./stm32f4xx_hal_driver/Inc/
        ./stm32f4xx_hal_driver/Inc/Legacy/
        ./cmsis_device_f4/Include/
        ./CMSIS_5/CMSIS/Core/Include/
)

########################################################################################################################
# templates not needed - exclude them
########################################################################################################################
list(FILTER SOURCES EXCLUDE REGEX "template")
list(FILTER INCLUDES_PUBLIC EXCLUDE REGEX "template")

add_library(IcManufacturerDrivers
        ${SOURCES}
)

target_include_directories(IcManufacturerDrivers PUBLIC
        ${INCLUDES_PUBLIC}
)