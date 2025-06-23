cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

option(ENABLE_USB_STDIO "Enable USB debugging" ON)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(TestPIO)

pico_sdk_init()

add_executable(${CMAKE_PROJECT_NAME}
    main.cpp
    led.pio
)

pico_generate_pio_header(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/led.pio)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    NDEBUG
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    hardware_pio
    pico_stdlib
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Werror -Wall)
if (ENABLE_USB_STDIO)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE ENABLE_USB_STDIO)
    # enable usb output, disable uart output
    pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
    pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)
endif()


# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})
