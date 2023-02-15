cmake_minimum_required(VERSION 3.23)
project(ArduinoMega)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ArduinoMega
        ArduinoMega.ino
        Controller.cpp
        Controller.h
        imu.cpp
        imu.h
        Parameter.h
        PID.h)
