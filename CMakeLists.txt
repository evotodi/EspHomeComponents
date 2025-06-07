cmake_minimum_required(VERSION 3.31)
include(FetchContent)
project(EHComponents)

set(ESPHOME_DIR "C:/Users/Justin/Documents/Projects/esphome")
set(ESPHOME_LIBS "${CMAKE_SOURCE_DIR}/.esphome/build/propanemonitor/.piolibdeps/propanemonitor")
set(ARDUINO_LIBS "C:/Users/Justin/.platformio/packages/framework-arduinoespressif32/cores/esp32")

include_directories("${ESPHOME_DIR}")
include_directories("${ARDUINO_LIBS}")

# Include dirs to includes
#include_directories("${ESPHOME_LIBS}/OneWire")

add_executable(EHComponents
        components/batt18650stats/Batt18650StatsComponent.cpp
        components/batt18650stats/Batt18650StatsComponent.h
)
