  /// \file DQMCanvasView.h
/*
 *
 * DQMCanvasView.h header template automatically generated by a class generator
 * Creation date : lun. oct. 12 2015
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


#ifndef DQMCANVASVIEW_H
#define DQMCANVASVIEW_H

// -- dqm4hep headers
#include "dqm4hep/DQM4HEP.h"
#include "dqm4hep/DQMXmlIO.h"

// -- qt headers
#include <QWidget>
#include <QMenu>

namespace dqm4hep
{

class DQMMonitoring;
class DQMTabWidget;
class DQMCanvasArea;
class DQMCanvas;
class DQMGuiMonitorElement;

/** DQMCanvasView class
 */ 
class DQMCanvasView : public QWidget, public DQMXmlIO
{
	Q_OBJECT

public:
	/** Constructor
	 */
	DQMCanvasView(DQMMonitoring *pMonitoring, QWidget *pParent = 0);

	/** Destructor
	 */
	virtual ~DQMCanvasView();

	/** Get the monitoring instance
	 */
	DQMMonitoring *getMonitoring() const;

	/** Export settings to xml element
	 */
	TiXmlElement *toXml() const;

	/** Import settings from xml element
	 */
	void fromXml(TiXmlElement *const pXmlElement);

	/** Get the current canvas area
	 */
	virtual DQMCanvasArea *getCurrentCanvasArea() const;

	/** Get the current canvas area name
	 */
	virtual std::string getCurrentCanvasAreaName() const;

	/** Get the canvas area by index
	 */
	virtual DQMCanvasArea *getCanvasArea(int index) const;

	/** Get the canvas area index using an instance of canvas area
	 */
	virtual int getCanvasAreaIndex(DQMCanvasArea *pCanvasArea) const;

	/** Get the canvas area name by index
	 */
	virtual std::string getCanvasAreaName(int index) const;

	/** Get the number of canvas areas
	 */
	virtual int canvasAreaCount() const;

	/** Get the canvas area list
	 */
	virtual QList<DQMCanvasArea*> canvasAreaList() const;

public slots:
	/** Create a canvas area
	 */
	virtual void createCanvasArea(const std::string &areaName = "");

	/** Create a canvas area and set it as current
	 */
	virtual void createCanvasAreaAndSetCurrent(const std::string &areaName = "");

	/** Draw the monitor element in the current area.
	 *  Update it if already drawn
	 */
	virtual void drawInCurrentArea(DQMGuiMonitorElement *pMonitorElement);

	/** Draw the monitor element in area indexed by 'index'.
	 *  Update it if already drawn
	 */
	virtual void drawInArea(DQMGuiMonitorElement *pMonitorElement, int index);

	/** Draw the monitor element in a new area
	 */
	virtual void drawInNewArea(DQMGuiMonitorElement *pMonitorElement, const std::string &areaName = "");

	/** Remove the canvas area by index
	 */
	virtual void removeCanvasArea(int index);

	/** Rename the canvas area indexed by 'index' with name 'newAreaName'
	 */
	virtual void renameCanvasArea(int index, const std::string &newAreaName);

	/** Clear the area indexed by 'index'
	 */
	virtual void clearArea(int index);

	/** Clear and remove all canvas areas
	 */
	virtual void clear();

    /** Show the context menu of this widget
     */
	virtual void showContextMenu(const QPoint &point);

    /** Set the current canvas area by index
     */
	virtual void setCurrentCanvasArea(int index);

	/** Move the canvas from the current area to the area indexed by 'index'
	 */
	virtual void moveCanvas(DQMCanvas *pCanvas, int newAreaIndex);

protected:
    /** Event filter mainly used to process the context menu
     */
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);

    /** Create the context menu
     */
    virtual QMenu *createContextMenu() const;

    /** Rename the canvas area given by 'index'.
     *  Get the area name from an input text dialog
     */
    void renameCanvasAreaFromInputDialog(int index);

private slots:
	/** Handle rename area action triggered by context menu
	 */
	void handleRenameAreaActionTriggered();

	/** Handle remove area action triggered by context menu
	 */
	void handleRemoveAreaActionTriggered();

	/** Handle clear area action triggered by context menu
	 */
	void handleClearAreaActionTriggered();

	/** Handle save as action triggered by context menu
	 */
	void handleSaveAsActionTriggered();

private:

	DQMMonitoring         *m_pMonitoring;
	DQMTabWidget          *m_pTabWidget;
};

} 

#endif  //  DQMCANVASVIEW_H