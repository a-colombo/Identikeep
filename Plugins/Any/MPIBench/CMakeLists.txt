# define minimum cmake version
cmake_minimum_required(VERSION 3.0.2)

SETCPP11()

include_directories("../../../include")

set(SOURCES plugin.cpp MPIBench.cpp)

if (NO_PLUGINS)
    add_library(MPIBench STATIC ${SOURCES})
else()
    add_library(MPIBench MODULE ${SOURCES})
endif()

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
if(MPI_COMPILE_FLAGS)
  set_target_properties(MPIBench PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(MPIBench PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()




