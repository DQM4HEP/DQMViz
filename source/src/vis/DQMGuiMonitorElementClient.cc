  /// \file DQMGuiMonitorElementClient.cc
/*
 *
 * DQMGuiMonitorElementClient.cc source template automatically generated by a class generator
 * Creation date : mer. oct. 14 2015
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


#include "dqm4hep/vis/DQMGuiMonitorElementClient.h"

namespace dqm4hep
{

DQMGuiMonitorElementClient::DQMGuiMonitorElementClient(const std::string &collectorName) :
		m_pMonitorElementClient(NULL)
{
	m_pMonitorElementClient = new DQMMonitorElementClient();
	m_pMonitorElementClient->setHandler(this);
	m_pMonitorElementClient->setCollectorName(collectorName);
}

//-------------------------------------------------------------------------------------------------

DQMGuiMonitorElementClient::~DQMGuiMonitorElementClient()
{
	delete m_pMonitorElementClient;
}

//-------------------------------------------------------------------------------------------------

DQMMonitorElementClient *DQMGuiMonitorElementClient::getMonitorElementClient() const
{
	return m_pMonitorElementClient;
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMGuiMonitorElementClient::receiveCollectorInfo(DQMMonitorElementClient */*pClient*/, const DQMCollectorInfo &collectorInfo)
{
	emit collectorInfoReceived(collectorInfo);
	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMGuiMonitorElementClient::receiveMonitorElementNameList(DQMMonitorElementClient */*pClient*/, const DQMMonitorElementInfoList &infoList)
{
	emit monitorElementListNameReceived(infoList);
	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMGuiMonitorElementClient::receiveMonitorElementPublication(DQMMonitorElementClient */*pClient*/, const DQMMonitorElementPublication &publication)
{
	emit monitorElementPublicationReceived(publication);
	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMGuiMonitorElementClient::handleClientConnection(DQMMonitorElementClient */*pClient*/)
{
	emit clientConnected();
	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMGuiMonitorElementClient::handleClientDisconnection(DQMMonitorElementClient */*pClient*/)
{
	emit clientDisconnected();
	return STATUS_CODE_SUCCESS;
}

} 
