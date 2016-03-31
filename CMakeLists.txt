#################################################
# cmake file for building DQMViz package
# @author Eté Rémi
# Copyright (c) CNRS / IPNL
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
#################################################

# ----- project name -----
PROJECT( DQMViz )

SET( ${PROJECT_NAME}_VERSION_MAJOR 2 )
SET( ${PROJECT_NAME}_VERSION_MINOR 0 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

###############################
# Require C++11
INCLUDE( CheckCXXCompilerFlag )

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

IF( COMPILER_SUPPORTS_CXX11 )
  SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSE()
	MESSAGE( SEND_ERROR "DQMCore requires C++11 support. Please upgrade your compiler !" )
ENDIF()

# ----- dependencies -----
LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
MESSAGE( STATUS "CMAKE_MODULE_PATH : ${CMAKE_MODULE_PATH}" )

# load default settings
FIND_PACKAGE( ILCUTIL COMPONENTS ILCSOFT_CMAKE_MODULES QUIET )

IF( ILCUTIL_FOUND )
    INCLUDE( ilcsoft_default_settings )
ELSE()
    INCLUDE( default_settings )
ENDIF()


# ----- Generate DOXYGEN documentation (optionnal) -----
OPTION( INSTALL_DOC             "Set to OFF to skip build/install Documentation" OFF )


# ----- Find ROOT package -----
LIST( APPEND ROOT_COMPONENT_LIST Core Hist Rint HistPainter Graf Graf3d MathCore Net RIO Tree QtRoot GQt Gui )
FIND_PACKAGE( ROOT COMPONENTS ${ROOT_COMPONENT_LIST} REQUIRED )

# ----- Find Threads or multithreading.-----
FIND_PACKAGE( Threads REQUIRED )

# ----- Find LOG4CXX package -----
FIND_PACKAGE( LOG4CXX REQUIRED )

# ----- Find json cpp package -----
FIND_PACKAGE( JSONCPP REQUIRED )

# ----- Find DIM package -----
FIND_PACKAGE( DIM REQUIRED SHARED )

# ----- Find DIMJC package -----
FIND_PACKAGE( DIMJC REQUIRED )

# ----- Find DQMCore package -----
FIND_PACKAGE( DQMCore REQUIRED )

# ----- Find xdrstream package -----
FIND_PACKAGE( xdrstream REQUIRED )

# Include package properties
FOREACH( pkg ROOT LOG4CXX JSONCPP DIM DIMJC DQMCore xdrstream )

  INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
  LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
  ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )

ENDFOREACH()


# ----- Find Qt4 package -----
SET( QT_ROOT_DIR $ENV{QTDIR} )
SET( QT_QMAKE_EXECUTABLE ${QT_ROOT_DIR}/bin/qmake )

FIND_PACKAGE( Qt4 REQUIRED )

INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIRS} )
ADD_DEFINITIONS ( ${QT_DEFINITIONS} )

INCLUDE( ${QT_USE_FILE} )


# ----- Export ${PROJECT_NAME}_DEPENDS* to ${PROJECT_NAME}Config.cmake -----
SET( ${PROJECT_NAME}_DEPENDS_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}  ${DIM_INCLUDE_DIRS}   ${JSONCPP_INCLUDE_DIRS})
SET( ${PROJECT_NAME}_DEPENDS_LIBRARY_DIRS ${ROOT_LIBRARY_DIR}   ${DIM_LIBRARY_DIRS}   ${JSONCPP_LIBRARY_DIRS})
SET( ${PROJECT_NAME}_DEPENDS_LIBRARIES    ${ROOT_LIBRARIES}     ${DIM_LIBRARIES}      ${JSONCPP_LIBRARIES})

LIST( APPEND ${PROJECT_NAME}_DEPENDS_INCLUDE_DIRS ${streamlog_INCLUDE_DIRS} )
LIST( APPEND ${PROJECT_NAME}_DEPENDS_LIBRARY_DIRS ${streamlog_LIBRARY_DIRS} )
LIST( APPEND ${PROJECT_NAME}_DEPENDS_LIBRARIES    ${streamlog_LIBRARIES}    )


LIST( APPEND ${PROJECT_NAME}_DEPENDS_INCLUDE_DIRS ${QT_INCLUDE_DIRS}  ${DIMJC_INCLUDE_DIRS} )
LIST( APPEND ${PROJECT_NAME}_DEPENDS_LIBRARY_DIRS ${QT_LIBRARY_DIRS}  ${DIMJC_LIBRARY_DIRS} )
LIST( APPEND ${PROJECT_NAME}_DEPENDS_LIBRARIES    ${QT_LIBRARIES}     ${DIMJC_LIBRARIES} )


# ----- Install doc -----
IF( INSTALL_DOC )

    FIND_PACKAGE( Doxygen )

    IF( DOXYGEN_FOUND )
        ADD_SUBDIRECTORY( ./doc )
    ELSE()
        MESSAGE( SEND_ERROR "Could not find doxygen required to build documentation" )
        MESSAGE( "Please install doxygen or set INSTALL_DOC to OFF" )
        MESSAGE( "" )
    ENDIF()

ENDIF()


# ---- Generate DQMVizConfig.h
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.h.cmake.in"
    "${PROJECT_SOURCE_DIR}/source/include/${PROJECT_NAME}Config.h" @ONLY )


# ----- Add source directory -----
ADD_SUBDIRECTORY( ./source )

# ----- Install icons -----
INSTALL_DIRECTORY( icons DESTINATION . )

# ----- display some variables and write them to cache -----
DISPLAY_STD_VARIABLES()

# ----- generate and install following configuration files -----
GENERATE_PACKAGE_CONFIGURATION_FILES( ${PROJECT_NAME}Config.cmake ${PROJECT_NAME}ConfigVersion.cmake ${PROJECT_NAME}LibDeps.cmake )

