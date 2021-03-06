  /// \file DQMJobInterface.h
/*
 *
 * DQMJobInterface.h header template automatically generated by a class generator
 * Creation date : dim. janv. 10 2016
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


#ifndef DQMJOBINTERFACE_H
#define DQMJOBINTERFACE_H

// -- dimjc headers
#include "DimDQMJobInterface.h"

// -- qt headers
#include <QObject>

namespace dqm4hep
{

/** DQMJobInterface class
 */
class DQMJobInterface : public QObject, public DimDQMJobInterface
{
	Q_OBJECT

public:
	/** Constructor
	 */
	DQMJobInterface();

	/** Destructor
	 */
	virtual ~DQMJobInterface();

	/** Whether the automatic update mode has been started
	 */
	bool started() const;

public slots:
	/** Start update.
	 *  Alias slot function for startTimer(nSeconds)
	 */
	void startUpdate(int nSeconds);

	/** Stop update.
	 *  Alias slot function for stopTimer()
	 */
	void stopUpdate();

private:
	/** Call back method when jobs status are received
	 */
	void statusReceived(const std::string &hostName);

signals:
	/** Signal emitted when job status are received
	 */
	void statusReceived(const QString &hostName);

private:

	bool              m_started;
};

} 

#endif  //  DQMJOBINTERFACE_H
