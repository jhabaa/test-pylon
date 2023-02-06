cmake_minimum_required(VERSION 3.16)
project(daemon)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)



set(PYLON_DIR /opt/pylon)
set(PYLON_LIB_DIR ${PYLON_DIR}/lib)
set(PYLON_INCLUDE_DIRS ${PYLON_DIR}/include)

find_library(PYLON_BASE NAMES pylonbase
                        PATHS ${PYLON_LIB_DIR})
find_library(PYLON_GCBASE NAMES GCBase_gcc_v3_1_Basler_pylon
                          PATHS ${PYLON_LIB_DIR})
find_library(PYLON_GENAPI GenApi_gcc_v3_1_Basler_pylon
                          PATHS ${PYLON_LIB_DIR})
find_library(PYLON_UTILITY pylonutility
                           PATHS ${PYLON_LIB_DIR})

set(PYLON_LIBS
	${PYLON_BASE} ${PYLON_GCBASE} ${PYLON_GENAPI} ${PYLON_UTILITY}
)




add_executable(test
main.cpp
)

target_include_directories(test PUBLIC
	${PYLON_INCLUDE_DIRS}
)
target_link_libraries(test
	${PYLON_LIBS}
)


