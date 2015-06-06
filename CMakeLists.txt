#---------------------------------------------------
# OpenMM Example Plugin
#----------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# We need to know where OpenMM is installed so we can access the headers and libraries.
SET(OPENMM_DIR "/usr/local/openmm" CACHE PATH "Where OpenMM is installed")
INCLUDE_DIRECTORIES("${OPENMM_DIR}/include")
LINK_DIRECTORIES("${OPENMM_DIR}/lib" "${OPENMM_DIR}/lib/plugins")

# set flags for linking on mac
IF(APPLE)
    SET (CMAKE_INSTALL_NAME_DIR "@rpath")
    SET(EXTRA_COMPILE_FLAGS "-msse2 -stdlib=libc++")
ENDIF(APPLE)

# Select where to install
IF(${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT})
    IF(WIN32)
        SET(CMAKE_INSTALL_PREFIX "$ENV{ProgramFiles}/OpenMM" CACHE PATH "Where to install the plugin" FORCE)
    ELSE(WIN32)
        SET(CMAKE_INSTALL_PREFIX "/usr/local/openmm" CACHE PATH "Where to install the plugin" FORCE)
    ENDIF(WIN32)
ENDIF(${CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT})

#
#build/install AGBNP3 library
#
SET(AGBNP3LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/AGBNP3")
SET(AGBNP3LIB_NAME agbnp3)
SET(AGBNP3LIB_INCLUDE_DIR ${AGBNP3LIB_DIR})
ADD_LIBRARY(${AGBNP3LIB_NAME} SHARED "${AGBNP3LIB_DIR}/agbnp3.c" "${AGBNP3LIB_DIR}/agbnp3_utils.c" "${AGBNP3LIB_DIR}/agbnp3_cpu_simd_kernels.c" "${AGBNP3LIB_DIR}/sse_mathfun.h" "${AGBNP3LIB_DIR}/agbnp3.h" "${AGBNP3LIB_DIR}/agbnp3_private.h")
SET_TARGET_PROPERTIES(${AGBNP3LIB_NAME} PROPERTIES COMPILE_FLAGS "-O3 -DUSE_SSE")
INSTALL_TARGETS(/lib RUNTIME_DIRECTORY /lib ${AGBNP3LIB_NAME})

#
#build/install nblist library
#
SET(NBLISTLIB_NAME nblist)
ADD_LIBRARY(${NBLISTLIB_NAME} SHARED "${AGBNP3LIB_DIR}/nblist.c" "${AGBNP3LIB_DIR}/nblist.h")
INSTALL_TARGETS(/lib RUNTIME_DIRECTORY /lib ${NBLISTLIB_NAME})


#
#build/Install the main API library
#
SET(AGBNP3_PLUGIN_SOURCE_SUBDIRS openmmapi)
SET(AGBNP3_LIBRARY_NAME AGBNP3Plugin)
SET(SHARED_AGBNP3_TARGET ${AGBNP3_LIBRARY_NAME})
#These are all the places to search for header files which are to be part of the API.
SET(API_INCLUDE_DIRS "openmmapi/include" "openmmapi/include/internal")
# Locate header files.
SET(API_INCLUDE_FILES)
FOREACH(dir ${API_INCLUDE_DIRS})
    FILE(GLOB fullpaths ${dir}/*.h)
    SET(API_INCLUDE_FILES ${API_INCLUDE_FILES} ${fullpaths})
ENDFOREACH(dir)
# Collect up source files
SET(SOURCE_FILES) # empty
SET(SOURCE_INCLUDE_FILES)
FOREACH(subdir ${AGBNP3_PLUGIN_SOURCE_SUBDIRS})
    FILE(GLOB src_files  ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/src/*.cpp)
    FILE(GLOB incl_files ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/src/*.h)
    SET(SOURCE_FILES         ${SOURCE_FILES}         ${src_files})   #append
    SET(SOURCE_INCLUDE_FILES ${SOURCE_INCLUDE_FILES} ${incl_files})
    ## Make sure we find these locally before looking in OpenMM/include if
    ## OpenMM was previously installed there.
    INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/include)
ENDFOREACH(subdir)
# Create the library.
ADD_LIBRARY(${SHARED_AGBNP3_TARGET} SHARED ${SOURCE_FILES} ${SOURCE_INCLUDE_FILES} ${API_INCLUDE_FILES})
SET_TARGET_PROPERTIES(${SHARED_AGBNP3_TARGET}
    PROPERTIES COMPILE_FLAGS "-DAGBNP3_BUILDING_SHARED_LIBRARY ${EXTRA_COMPILE_FLAGS}"
    LINK_FLAGS "${EXTRA_COMPILE_FLAGS}")
TARGET_LINK_LIBRARIES(${SHARED_AGBNP3_TARGET} OpenMM)
INSTALL_TARGETS(/lib RUNTIME_DIRECTORY /lib ${SHARED_AGBNP3_TARGET})
# install headers
FILE(GLOB API_ONLY_INCLUDE_FILES "openmmapi/include/*.h")
INSTALL (FILES ${API_ONLY_INCLUDE_FILES} DESTINATION include)
FILE(GLOB API_ONLY_INCLUDE_FILES_INTERNAL "openmmapi/include/internal/*.h")
INSTALL (FILES ${API_ONLY_INCLUDE_FILES_INTERNAL} DESTINATION include/internal)

# Enable testing

#ENABLE_TESTING()
#ADD_SUBDIRECTORY(serialization/tests)

#
# Build/Install for the Reference platform
#
ADD_SUBDIRECTORY(platforms/reference)

# Build the Python API
FIND_PROGRAM(PYTHON_EXECUTABLE python)
FIND_PROGRAM(SWIG_EXECUTABLE swig)
IF(PYTHON_EXECUTABLE AND SWIG_EXECUTABLE)
    SET(AGBNP3_BUILD_PYTHON_WRAPPERS ON CACHE BOOL "Build wrappers for Python")
ELSE(PYTHON_EXECUTABLE AND SWIG_EXECUTABLE)
    SET(AGBNP3_BUILD_PYTHON_WRAPPERS OFF CACHE BOOL "Build wrappers for Python")
ENDIF(PYTHON_EXECUTABLE AND SWIG_EXECUTABLE)
IF(AGBNP3_BUILD_PYTHON_WRAPPERS)
    ADD_SUBDIRECTORY(python)
ENDIF(AGBNP3_BUILD_PYTHON_WRAPPERS)

