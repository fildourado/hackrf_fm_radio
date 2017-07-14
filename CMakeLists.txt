cmake_minimum_required (VERSION 3.5)

project (hackrf_fm_radio)


MESSAGE( STATUS "${PROJECT_BINARY_DIR}" )


find_library(OPENAL_LIB openal)
if(NOT OPENAL_LIB)
  message(FATAL_ERROR "OpenAL library not found")
endif()

find_library(AUDIO_LIB audio)
if(NOT AUDIO_LIB)
  message(FATAL_ERROR "audiolib library not found")
endif()

find_library(FFTW_LIB fftw3)
if(NOT FFTW_LIB)
  message(FATAL_ERROR "FFTW library not found")
endif()

#include_directories ("/usr/include/AL/")

add_executable (openAL_example  ${PROJECT_BINARY_DIR}/../src/openAL_example.cpp )
TARGET_LINK_LIBRARIES( openAL_example openal)
TARGET_LINK_LIBRARIES( openAL_example audio)

#target_link_libraries (Tutorial  ${EXTRA_LIBS})

# link openAL example to openAL library
#/usr/include/AL



# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
#include_directories("${PROJECT_BINARY_DIR}")
 



#add_executable( Tutorial tutorial.cxx )
