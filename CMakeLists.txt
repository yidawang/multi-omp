cmake_minimum_required (VERSION 2.6)
project (Multi-OMP)

if ($ENV{VERBOSE})
  set (CMAKE_VERBOSE_MAKEFILE ON)
endif ($ENV{VERBOSE})

# add the binary tree to the search path for include files
include_directories("${PROJECT_SOURCE_DIR}/src")

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#  "${PROJECT_SOURCE_DIR}/PhiSVMConfig.h.in"
#  "${PROJECT_BINARY_DIR}/PhiSVMConfig.h"
#  )

# should we use MKL? otherwise we use OpenBLAS
#option (USE_MKL "Use Intel MKL" ON)

# set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -fopenmp -std=c++14 -fPIC")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lglog")

if ($ENV{USE_MKL}) # assuming icc
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_MKL -ipo")# -no-vec")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mkl=parallel")
  if ($ENV{KNL})
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__MIC__ -xMIC-AVX512")
      set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lmemkind")
  else ($ENV{KNL})
      #set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xCOMMON-AVX512")
  endif ($ENV{KNL})
else ($ENV{USE_MKL})
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lopenblas -lomp")
endif ($ENV{USE_MKL})

if ($ENV{APPLE}) # in mac
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DAPPLE")
endif ($ENV{APPLE})

if ($ENV{REPORT})
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -qopt-report")
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -qopt-report")
endif ($ENV{REPORT})

set(CMAKE_C_COMPILER ${CC})
set(CMAKE_CXX_COMPILER ${CXX})

# get all *.cpp files recursively
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# add the executable
add_executable (multi-omp ${SRC_FILES})
#target_link_libraries (FCMA  ${EXTRA_LIBS})
