cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(test_project)

pico_sdk_init()

add_executable(dropping_monsters
        dropping_monsters.c pico_ntsc_grph.c mini_gamepad.c
        )

# Pull in our (to be renamed) simple get you started dependencies
target_link_libraries(dropping_monsters pico_stdlib hardware_adc hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(dropping_monsters)

pico_enable_stdio_usb(dropping_monsters 0)

pico_enable_stdio_uart(dropping_monsters 0)
