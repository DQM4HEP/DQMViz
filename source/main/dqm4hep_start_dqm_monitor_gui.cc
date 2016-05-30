  /// \file dqm4hep_start_dqm_monitor_gui.cc
/*
 *
 * dqm4hep_start_dqm_monitor_gui.cc main source file template automatically generated
 * Creation date : mer. nov. 5 2014
 *
 * This file is part of DQM4HEP libraries.
 *
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */

#include <QApplication>
#include <QMetaType>

#include "DQMVizConfig.h"       // for soft version
#include "dqm4hep/DQM4HEP.h"
#include "dqm4hep/DQMNetworkTool.h"
#include "dqm4hep/DQMLogging.h"

#include "dqm4hep/vis/DQMMonitoring.h"
#include "dqm4hep/vis/DQMMonitoringModel.h"
#include "dqm4hep/vis/DQMMonitoringView.h"
#include "dqm4hep/vis/DQMMonitoringController.h"
#include "dqm4hep/vis/DQMGuiMonitorElement.h"
#include "dqm4hep/vis/DQMGuiTools.h"

// -- tclap headers
#include "tclap/CmdLine.h"
#include "tclap/Arg.h"

using namespace dqm4hep;

int main(int argc, char* argv[])
{
	DQM4HEP::screenSplash();

	std::string cmdLineFooter = "Please report bug to <rete@ipnl.in2p3.fr>";
	TCLAP::CmdLine *pCommandLine = new TCLAP::CmdLine(cmdLineFooter, ' ', DQMViz_VERSION_STR);
	std::string log4cxx_file = std::string(DQMCore_DIR) + "/conf/defaultLoggerConfig.xml";

	TCLAP::ValueArg<std::string> settingsFileArg(
				  "f"
				 , "import-file"
				 , "The xml file containing a pre-selection of monitor elements and the settings of the canvas area"
				 , false
				 , ""
				 , "string");
	pCommandLine->add(settingsFileArg);

	TCLAP::ValueArg<std::string> loggerConfigArg(
				  "l"
				 , "logger-config"
				 , "The xml logger file to configure log4cxx"
				 , false
				 , log4cxx_file
				 , "string");
	pCommandLine->add(loggerConfigArg);

	std::vector<std::string> allowedLevels;
	allowedLevels.push_back("INFO");
	allowedLevels.push_back("WARN");
	allowedLevels.push_back("DEBUG");
	allowedLevels.push_back("TRACE");
	allowedLevels.push_back("ERROR");
	allowedLevels.push_back("FATAL");
	allowedLevels.push_back("OFF");
	allowedLevels.push_back("ALL");
	TCLAP::ValuesConstraint<std::string> allowedLevelsContraint( allowedLevels );

	TCLAP::ValueArg<std::string> verbosityArg(
				  "v"
				 , "verbosity"
				 , "The verbosity level used for this application"
				 , false
				 , "INFO"
				 , &allowedLevelsContraint);
	pCommandLine->add(verbosityArg);

	pCommandLine->parse(argc, argv);

	log4cxx_file = loggerConfigArg.getValue();
	log4cxx::xml::DOMConfigurator::configure(log4cxx_file);

	dqmMainLogger->setLevel( log4cxx::Level::toLevel( verbosityArg.getValue() ) );

	QApplication app(argc, argv);

	qRegisterMetaType<DQMHostInfo>("DQMHostInfo");
	qRegisterMetaType<DQMMonitorElementInfoList>("DQMMonitorElementInfoList");
	qRegisterMetaType<DQMPublication>("DQMPublication");
	qRegisterMetaType<DQMGuiMonitorElementList>("DQMMonitorElementList");
	qRegisterMetaType<DQMMonitorElementRequest>("DQMMonitorElementRequest");
	qRegisterMetaType<DQMGuiMonitorElement *>("DQMGuiMonitorElement *");


	DQMMonitoring *pMonitoring = new DQMMonitoring();

	DQMMonitoringModel *pModel = new DQMMonitoringModel(pMonitoring);
	DQMMonitoringView *pView = new DQMMonitoringView(pMonitoring);
	DQMMonitoringController *pController = new DQMMonitoringController(pMonitoring);

	pView->buildView();
	pView->showView();

	DQMGuiTools::checkDimDnsNode(pView->getMainWindow());

	if( settingsFileArg.isSet() )
		pController->openFile( settingsFileArg.getValue() );

	return app.exec();
}
