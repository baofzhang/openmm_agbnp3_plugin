# - Find FFTW
# Find the native FFTW includes and library
#
#  AGBNP3_INCLUDES        - where to find agbnp3.h
#  AGBNP3_LIBRARY         - the main AGBNP3 library.
#  AGBNP3_FOUND           - True if AGBNP3 found.

if (AGBNP3_INCLUDES)
  # Already in cache, be silent
  set (AGBNP3_FIND_QUIETLY TRUE)
endif (AGBNP3_INCLUDES)

find_path (AGBNP3_INCLUDES agbnp3.h HINTS ${AGBNP3_DIR})

find_library (AGBNP3_LIBRARY NAMES agbnp3 HINTS ${AGBNP3_DIR} )
find_library (AGBNP3_NBLIST_LIBRARY NAMES nblist HINTS ${AGBNP3_DIR} )



# handle the QUIETLY and REQUIRED arguments and set AGBNP3_FOUND to TRUE if
# all listed variables are TRUE
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (AGBNP3 DEFAULT_MSG AGBNP3_LIBRARY AGBNP3_NBLIST_LIBRARY AGBNP3_INCLUDES)

mark_as_advanced (AGBNP3_LIBRARY  AGBNP3_NBLIST_LIBRARY  AGBNP3_INCLUDES)
