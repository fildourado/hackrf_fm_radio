cmake_minimum_required (VERSION 3.5)

project (hackrf_fm_radio)


MESSAGE( STATUS "${PROJECT_BINARY_DIR}" )

set (CMAKE_CXX_STANDARD 11)

find_library(OPENAL_LIB openal)
if(NOT OPENAL_LIB)
  message(FATAL_ERROR "OpenAL library not found")
endif()

find_library(HACKRF_LIB hackrf)
if(NOT HACKRF_LIB)
  message(FATAL_ERROR "HackRF library not found")
endif()


#find_library(AUDIO_LIB audio)
#if(NOT AUDIO_LIB)
#  message(FATAL_ERROR "audiolib library not found")
#endif()

#find_library(FFTW_LIB fftw3)
#if(NOT FFTW_LIB)
#  message(FATAL_ERROR "FFTW library not found")
#endif()


add_executable (openAL_example  ${PROJECT_BINARY_DIR}/../src/openAL_example.cpp )
TARGET_LINK_LIBRARIES( openAL_example openal)


include_directories ("/home/fdourado/repos/hackrf/host/libhackrf/src/")

add_executable (rxsamps_example  ${PROJECT_BINARY_DIR}/../src/rx_samps.cpp )
TARGET_LINK_LIBRARIES( rxsamps_example hackrf)
