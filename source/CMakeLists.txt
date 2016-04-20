#######################################################
# cmake file for building DQMViz package
# @author Eté Rémi , IPNL
# Copyright (c) CNRS / IPNL
#######################################################

# include directories
INCLUDE_DIRECTORIES( BEFORE include )

# require proper c++
ADD_DEFINITIONS( "-pedantic -Wunused-value -O2" )
ADD_DEFINITIONS("-Wno-long-long -Wreturn-type -Werror")


# ----- Build the global library -----
AUX_SOURCE_DIRECTORY( src/vis SRC_FILES )
    
QT4_WRAP_CPP (
    VIS_MOC_SRC_FILES
    include/dqm4hep/vis/DQMMonitoringView.h
    include/dqm4hep/vis/DQMMonitoringModel.h
    include/dqm4hep/vis/DQMMonitoringController.h
    include/dqm4hep/vis/DQMGuiMonitorElement.h
    include/dqm4hep/vis/DQMMonitorElementView.h
    include/dqm4hep/vis/DQMGuiMonitorElementClient.h
    include/dqm4hep/vis/DQMJobInterface.h
	include/dqm4hep/vis/DQMLoggerWidget.h
	include/dqm4hep/vis/DQMJobInterfaceWidget.h
    include/dqm4hep/vis/DQMRunControlWidget.h
    include/dqm4hep/vis/DQMCanvas.h
    include/dqm4hep/vis/DQMCanvasView.h
    include/dqm4hep/vis/DQMRootWidget.h
    include/dqm4hep/vis/DQMBrowserWidget.h
    include/dqm4hep/vis/DQMMonitorElementInfoWidget.h
)

ADD_SHARED_LIBRARY( ${PROJECT_NAME} ${SRC_FILES} ${VIS_MOC_SRC_FILES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${QT_LIBRARIES} ${ROOT_QTROOT_LIBRARY} ${ROOT_GQT_LIBRARY} ${ROOT_GUI_LIBRARY} )

INSTALL(
        TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
)

# ----- Build job interface binary -----
ADD_EXECUTABLE( dqm4hep_start_job_interface main/dqm4hep_start_job_interface.cc )
TARGET_LINK_LIBRARIES( dqm4hep_start_job_interface ${PROJECT_NAME} ${PROJECT_NAME} )

INSTALL(
    TARGETS dqm4hep_start_job_interface
    RUNTIME DESTINATION bin
)

# ----- Build run control binary -----
ADD_EXECUTABLE( dqm4hep_start_run_control main/dqm4hep_start_run_control.cc )
TARGET_LINK_LIBRARIES( dqm4hep_start_run_control ${PROJECT_NAME} ${PROJECT_NAME} )

INSTALL(
    TARGETS dqm4hep_start_run_control
    RUNTIME DESTINATION bin
)

# ----- Build dqm monitoring gui binary -----
ADD_EXECUTABLE( dqm4hep_start_dqm_monitor_gui main/dqm4hep_start_dqm_monitor_gui.cc )
TARGET_LINK_LIBRARIES( dqm4hep_start_dqm_monitor_gui ${PROJECT_NAME} ${PROJECT_NAME} )

INSTALL(
    TARGETS dqm4hep_start_dqm_monitor_gui
    RUNTIME DESTINATION bin
)

# ----- Install headers -----
INSTALL_DIRECTORY( include DESTINATION . FILES_MATCHING PATTERN "*.h" )

