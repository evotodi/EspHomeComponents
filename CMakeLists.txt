cmake_minimum_required(VERSION 3.31)
include(FetchContent)
project(EHComponents)

set(ESPHOME_DIR "C:/Users/Justin/Documents/Projects/esphome")
set(ESPHOME_LIBS "${CMAKE_SOURCE_DIR}/.esphome/build/propanemonitor/.piolibdeps/propanemonitor")
set(ARDUINO_LIBS "C:/Users/Justin/.platformio/packages/framework-arduinoespressif32/cores/esp32")

include_directories("${ESPHOME_DIR}")
include_directories("${ARDUINO_LIBS}")

# Get OneWire
FetchContent_Declare(OneWire
        GIT_REPOSITORY https://github.com/PaulStoffregen/OneWire.git
        GIT_TAG        v2.3.8
        GIT_SHALLOW true
)
FetchContent_MakeAvailable(OneWire)
include_directories("${onewire_SOURCE_DIR}")

# Get DallasTemperature
FetchContent_Declare(DallasTemperature
        GIT_REPOSITORY https://github.com/milesburton/Arduino-Temperature-Control-Library
        GIT_TAG        v4.0.3
        GIT_SHALLOW true
        SOURCE_SUBDIR     pathThatDoesNotExist
)
FetchContent_MakeAvailable(DallasTemperature)
include_directories("${dallastemperature_SOURCE_DIR}")


add_executable(EHComponents
        components/batt18650stats/Batt18650StatsComponent.cpp
        components/batt18650stats/Batt18650StatsComponent.h
        components/ds18powered/Ds18PoweredComponent.cpp
        components/ds18powered/Ds18PoweredComponent.h
)