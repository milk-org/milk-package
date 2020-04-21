# Example, run from _build directory :
#
#
# 
# cmake ..
# cmake .. -DCMAKE_INSTALL_PREFIX=../_install
#
#
#
# USING GPU libraries
# ===============================
# cmake .. -DUSE_MAGMA=ON
#
#
# BUILD TYPE
# ===============================
# Debug build type turns off optimization
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake -DCMAKE_BUILD_TYPE=Debug .. -DUSE_MAGMA=ON
#
# Release build type turns on optimization flags
# cmake -DCMAKE_BUILD_TYPE=Release ..
#
#
#
# COMPILING
# ================================
# make clean
# make
# sudo make install
#
#
#

cmake_minimum_required (VERSION 3.5)

project (milk C)

# Version number
set ( VERSION_MAJOR 1 )
set ( VERSION_MINOR 01 )
set ( VERSION_PATCH 00 )
set ( PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
set ( PROJECT_DESCRIPTION "Modular Image processing tooLKit" )
set ( VERSION_OPTION "-alpha" )

# turn VERBOSE ON
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_INSTALL_PREFIX "/usr/local/milk-${VERSION_MAJOR}.${VERSION_MINOR}")


# Configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/milk_config.h.in"
  "${PROJECT_SOURCE_DIR}/src/milk_config.h"
  )






#
# main libraries
# linked to executable by default
#

list(APPEND libsrcdir src/CommandLineInterface)
list(APPEND libname CLIcore)

list(APPEND libsrcdir src/ImageStreamIO)
list(APPEND libname ImageStreamIO)


# =======================================
# MODULES INCLUDED
# =======================================
# modules are added to two lists:
# libsrcdir : source code directory
# libname   : shared object name

# CORE MODULES
list(APPEND modulelist COREMOD_arith COREMOD_iofits COREMOD_memory COREMOD_tools)

#
# Additional modules
#
list(APPEND modulelist image_basic)
list(APPEND modulelist info)
list(APPEND modulelist ZernikePolyn)
list(APPEND modulelist image_filter)
list(APPEND modulelist image_gen)
list(APPEND modulelist statistic)
list(APPEND modulelist image_format)
list(APPEND modulelist img_reduce)
list(APPEND modulelist psf)
list(APPEND modulelist fft)
list(APPEND modulelist cudacomp)
list(APPEND modulelist kdtree)
list(APPEND modulelist linARfilterPred)
list(APPEND modulelist linopt_imtools)




foreach(mname IN LISTS modulelist)
string(REPLACE "_" "" mname1 "${mname}")
list(APPEND libsrcdir src/${mname})
list(APPEND libname milk${mname1})
message("======= adding module  ${mname}")
endforeach()




# =======================================
# BUILD TYPE
# =======================================

# Set Release build flags
set(CMAKE_C_FLAGS_DEBUG     "-O0 -g -Wall -Wextra")

# Set Release build flags
set(CMAKE_C_FLAGS_RELEASE     "-Ofast -DNDEBUG")


# Set a default build type if none was specified
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()

message("CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")



# =======================================
# COMPILE OPTIONS
# =======================================

add_compile_options(-std=gnu11)
add_compile_options(-march=native)

# OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
message("Found OpenMP")
add_compile_options(-fopenmp)
endif()

# link-time optimizer
add_compile_options(-flto)
add_compile_options(-fwhole-program)
# NOTE: -fstack-usage is incompatible with link-time optimizer
#add_compile_options(-fstack-usage)


# A common flag is -pipe. This flag has no effect on the generated code, 
# but it makes the compilation process faster. It tells the compiler 
# to use pipes instead of temporary files during the different stages 
# of compilation, which uses more memory. On systems with low memory, 
# GCC might get killed. In those cases do not use this flag. 
add_compile_options(-pipe)

message("COMPILE_OPTIONS:  ${COMPILE_OPTIONS}")



# =======================================
# GPU ACCELERATION
# =======================================

# options
option(USE_CUDA  "Use CUDA        library" OFF)
option(USE_MAGMA "Use MAGMA       library" OFF)


# MAGMA (optional)
if(USE_MAGMA)
set(USE_CUDA ON)
endif(USE_MAGMA)


if(USE_CUDA)
find_package( CUDA REQUIRED )
endif(USE_CUDA)

if(USE_MAGMA)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MAGMA REQUIRED magma)
message(STATUS ${MAGMA_LIBRARY_DIRS})
link_directories( ${MAGMA_LIBRARY_DIRS} )
endif(USE_MAGMA)




# =======================================
# DEPENDENCIES
# =======================================

# ncurses
SET(CURSES_USE_NCURSES TRUE)

find_package( Threads )



message("====================================================")
message("VERSION  ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message("====================================================")
message("CMAKE_THREAD_LIBS_INIT  :  ${CMAKE_THREAD_LIBS_INIT}")
message("PROJECT_SOURCE_DIR       = ${PROJECT_SOURCE_DIR}")
message("PROJECT_BINARY_DIR       = ${PROJECT_BINARY_DIR}")


message("CMAKE_INSTALL_PREFIX     = ${CMAKE_INSTALL_PREFIX}")

message("USE_CUDA                 = ${USE_CUDA}")

message("USE_MAGMA                = ${USE_MAGMA}")
if(USE_MAGMA)
message("   MAGMA_LIBRARIES       = ${MAGMA_LIBRARIES}")
endif(USE_MAGMA)


message("EXTRAMODULES             = ${EXTRAMODULES}")

message("====================================================")





# ADD SOURCE DIRECTORIES
# recursively add libraries to be compiled

foreach(lsrcdir IN LISTS libsrcdir)
add_subdirectory ( "${lsrcdir}" )
endforeach()


#
# Add extra optional modules (list provided by user)
# separator character ";"
#

FOREACH(extramodule ${EXTRAMODULES})
add_subdirectory ( "src/${extramodule}" )
ENDFOREACH()


# main
add_executable(milk src/CLImain.c)

target_include_directories(milk PUBLIC
${PROJECT_SOURCE_DIR}/src
${GSL_INCLUDE_DIRS}
${FFTW_INCLUDE_DIRS}
${FFTWF_INCLUDE_DIRS}
${CURSES_INCLUDE_DIR}
)

# adds the options to all targets within the directory and its sub-directories
target_compile_options(milk PUBLIC 
${FFTW_CFLAGS_OTHER}
${FFTWF_CFLAGS_OTHER}
-fopenmp
)

set(BUILD_FLAGS "-DPACKAGE_NAME=\\\"milk\\\" -DCONFIGDIR=\\\"${PROJECT_SOURCE_DIR}/config\\\" -DSOURCEDIR=\\\"${PROJECT_SOURCE_DIR}\\\" -DABSSRCTOPDIR=\\\"${PROJECT_SOURCE_DIR}\\\" -DPACKAGE_BUGREPORT=\\\"https://github.com/milk-org/milk/issues\\\"" )


#
# link order must satisfy dependancies
# 

# ADD SOURCE DIRECTORIES
# recursively add libraries to be compiled

foreach(lname IN LISTS libname)
target_link_libraries (milk PUBLIC "${lname}" )
endforeach()




target_link_libraries (milk PUBLIC
${EXTRAMODULES}
m
readline
ncurses
cfitsio
dl
rt
${GSL_LIBRARIES} 
${FFTW_LIBRARIES}
${FFTWF_LIBRARIES}
${CMAKE_THREAD_LIBS_INIT}
-fopenmp
)


if(USE_CUDA)
target_link_libraries (milk PUBLIC ${CUDA_LIBRARIES} ${CUDA_CUBLAS_LIBRARIES})
set(BUILD_FLAGS "${BUILD_FLAGS} -DHAVE_CUDA" )
endif(USE_CUDA)


if(USE_MAGMA)
target_link_libraries (milk PUBLIC ${MAGMA_LIBRARIES})
set(BUILD_FLAGS "${BUILD_FLAGS} -DHAVE_CUDA -DHAVE_MAGMA" )
endif(USE_MAGMA)



set_target_properties(milk PROPERTIES COMPILE_FLAGS "${BUILD_FLAGS}" )



message("====================================================")
get_target_property(milkcompopts milk COMPILE_OPTIONS)

message("BUILD_FLAGS             :  ${BUILD_FLAGS}")
message("COMPILE_FLAGS           :  ${COMPILE_FLAGS}")
message("COMPILE_OPTIONS         :  ${milkcompopts}")
message("CMAKE_EXE_LINKER_FLAGS  :  ${CMAKE_EXE_LINKER_FLAGS}")

message("CMAKE_C_FLAGS           :  ${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS         :  ${CMAKE_CXX_FLAGS}")

message("CMAKE_C_FLAGS_DEBUG           : ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE         : ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_C_FLAGS_RELWITHDEBINFO  : ${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message("CMAKE_C_FLAGS_MINSIZEREL      : ${CMAKE_C_FLAGS_MINSIZEREL}")

message("CMAKE_CURRENT_SOURCE_DIR      : ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_CURRENT_BINARY_DIR      : ${CMAKE_CURRENT_BINARY_DIR}")
message("INSTALL_PKGCONFIG_DIR         : ${INSTALL_PKGCONFIG_DIR}")
message("====================================================")


set(PROGRAM_PERMISSIONS_DEFAULT
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

install(TARGETS milk DESTINATION bin PERMISSIONS ${PROGRAM_PERMISSIONS_DEFAULT} SETUID)

# Note: may need to write "/usr/local/magma/lib" to:
# /etc/ld.so.conf.d/magma.conf



# =======================================
# PKG-CONFIG
# =======================================

foreach(lname IN LISTS libname)
string(APPEND LINKSTRING "-l${lname} ")
endforeach()


if(USE_CUDA)
string(APPEND DFLAGS "-DMILK_USE_CUDA=ON ")
endif(USE_CUDA)

if(USE_MAGMA)
string(APPEND DFLAGS "-DMILK_USE_MAGMA=ON ")
endif(USE_MAGMA)


message("LINKSTRING: ${LINKSTRING}")

set(INSTALL_PKGCONFIG_DIR "lib/pkgconfig"
    CACHE PATH "Installation directory for pkgconfig (.pc) files")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/milk.pc.in
               ${CMAKE_CURRENT_BINARY_DIR}/milk.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/milk.pc
        DESTINATION "${INSTALL_PKGCONFIG_DIR}")




include(CTest)


add_test(milkCLIlaunch milk-exec "m?")
#set(CTEST_CUSTOM_MAXIMUM_PASSED_TEST_OUTPUT_SIZE 0)
#set(CTEST_CUSTOM_MAXIMUM_FAILED_TEST_OUTPUT_SIZE 0)
set_tests_properties(milkCLIlaunch
		PROPERTIES
		TIMEOUT 120
		CTEST_CUSTOM_MAXIMUM_FAILED_TEST_OUTPUT_SIZE 0)
add_test(milksemspeedtest milk-semtestspeed 100000 1)


