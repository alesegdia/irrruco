# - Find Aruco
# Find the native Aruco includes and libraries
#
#  Aruco_INCLUDE_DIR - where to find Aruco headers.
#  Aruco_LIBRARIES   - libraries needed for linking Aruco.
#  Aruco_FOUND       - True if libaruco was found.

if(Aruco_INCLUDE_DIR)
  # Already in cache, be silent
  set(Aruco_FIND_QUIETLY TRUE)
endif(Aruco_INCLUDE_DIR)

find_path(Aruco_INCLUDE_DIR aruco
  PATH_SUFFIXES include
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw          # Fink
  /opt/local   # DarwinPorts
  /opt/csw     # Blastwave
  /opt
  ${ArucoDIR}
  $ENV{ArucoDIR})

find_library(Aruco_LIBRARY aruco
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw          # Fink
  /opt/local   # DarwinPorts
  /opt/csw     # Blastwave
  /opt
  ${ArucoDIR}
  $ENV{ArucoDIR})


if(Aruco_LIBRARY)
  set(Aruco_FOUND TRUE)
else()
  set(Aruco_FOUND FALSE)
endif()

# Handle the QUIETLY and REQUIRED arguments and set SNDFILE_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Aruco Aruco_FOUND Aruco_LIBRARY Aruco_INCLUDE_DIR)

