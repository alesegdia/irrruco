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

find_path(Aruco_INCLUDE_DIR Box2D/Box2D.h
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

find_library(Aruco_LIBRARY_DEBUG
  Box2D-d
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

find_library(Aruco_LIBRARY_RELEASE
  Box2D
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


# Handle the QUIETLY and REQUIRED arguments and set SNDFILE_FOUND to TRUE if
# all listed variables are TRUE.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Aruco DEFAULT_MSG Aruco_LIBRARY Aruco_INCLUDE_DIR)

if(Aruco_FOUND)
  set(Aruco_LIBRARIES ${Aruco_LIBRARY})
else(Aruco_FOUND)
  set(Aruco_LIBRARIES)
endif(Aruco_FOUND)

