cmake_minimum_required(VERSION 2.8)
project(SoundProcessor)

set (CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} pybind11/include)

file(COPY interp_test.py  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY rrc_filter.py  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY freq_interp.py  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


if(NOT PYTHON_EXECUTABLE)
   find_program(PYTHON_EXECUTABLE python)
endif(NOT PYTHON_EXECUTABLE)

set(MATHSOURCES gm/CubicInterpolator.cc 
                gm/QPSKGen.cc
                gm/RRCFilter.cc
                gm/FreqInterpolator.cc
                gm/capture/AlsaCapture.cc
                gm/capture/AlsaInterface.cc
                gm/process/FilterN.cc
                gm/process/LogProcessor.cc
                gm/Thread.cc
                gm/shared.c)

add_subdirectory(pybind11)
include_directories(${PYTHON_INCLUDE_DIRS})

add_library(mathlib ${MATHSOURCES})
set_property(TARGET mathlib PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(mathlib fftw3f asound)

add_executable(alsa_capture gm/AlsaRun.cc)
target_link_libraries(alsa_capture mathlib rt pthread m)

add_executable(log_process gm/LogRun.cc)
target_link_libraries(log_process mathlib rt pthread m)

pybind11_add_module(dsp_example gm/PythonModel.cc)
target_link_libraries(dsp_example PUBLIC mathlib)

