  /// \file DQMRootWidget.cc
/*
 *
 * DQMRootWidget.cc source template automatically generated by a class generator
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


#include "dqm4hep/vis/DQMRootWidget.h"
#include "dqm4hep/vis/DQMCanvas.h"
#include "dqm4hep/vis/DQMCanvasArea.h"
#include "dqm4hep/vis/DQMCanvasView.h"
#include "dqm4hep/vis/DQMMonitoring.h"
#include "dqm4hep/vis/DQMMonitoringController.h"
#include "dqm4hep/vis/DQMMonitoringModel.h"
#include "dqm4hep/vis/DQMMonitoringView.h"
#include "dqm4hep/DQMMonitorElement.h"
#include "DQMCoreConfig.h"
#include "DQMVizConfig.h"

#include <TImage.h>
#include <TH1.h>
#include <TPaveStats.h>
#include <TStyle.h>

#include <QAction>
#include <QApplication>
#include <QInputDialog>

namespace dqm4hep
{

DQMRootWidget::DQMRootWidget(DQMMonitoring *pMonitoring, QWidget *pParent) :
	TQtWidget(pParent, 0, true),
	m_pMonitoring(pMonitoring),
	m_pCanvas(NULL),
	m_pCurrentMonitorElement(NULL),
	m_pNoVisImage(NULL)
{
	this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
    		this, SLOT(showContextMenu(const QPoint &)));
}

//-------------------------------------------------------------------------------------------------

DQMRootWidget::~DQMRootWidget() 
{
	if(m_pNoVisImage)
		delete m_pNoVisImage;
}

//-------------------------------------------------------------------------------------------------

DQMMonitoring *DQMRootWidget::getMonitoring() const
{
	return m_pMonitoring;
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::setCanvas(DQMCanvas *pCanvas)
{
	m_pCanvas = pCanvas;
}

//-------------------------------------------------------------------------------------------------

DQMCanvas *DQMRootWidget::getCanvas() const
{
	return m_pCanvas;
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
	TQtWidget::mouseDoubleClickEvent(e);
	this->postDraw();
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::mouseReleaseEvent(QMouseEvent *e)
{
	TQtWidget::mouseReleaseEvent(e);
	this->postDraw();
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::postDraw()
{
	// fake update triggered by object resize
	QSize currentSize = size();

	currentSize.setHeight(currentSize.height()+1);
	currentSize.setWidth(currentSize.width()+1);
	resize(currentSize);

	currentSize.setHeight(currentSize.height()-1);
	currentSize.setWidth(currentSize.width()-1);
	resize(currentSize);

	GetCanvas()->Modified();
	GetCanvas()->Resize();
	GetCanvas()->Update();
	Refresh();
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::drawNoVis()
{
	TImage *pNoVisImage = this->getNoVisImage();

	this->GetCanvas()->cd();
	this->GetCanvas()->Clear();
	pNoVisImage->Draw();
	this->GetCanvas()->Update();
	this->postDraw();
	m_pCurrentMonitorElement = NULL;
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::draw(DQMGuiMonitorElement *pMonitorElement)
{
	bool drawNoVis = (pMonitorElement == NULL ||
			pMonitorElement->getMonitorElement() == NULL ||
			pMonitorElement->getMonitorElement()->getObject() == NULL);

	if(drawNoVis)
	{
		this->drawNoVis();
		this->updateMonitorElement(pMonitorElement);
		return;
	}

	TObject *pObject = pMonitorElement->getMonitorElement()->getObject();
	const std::string &drawOption(pMonitorElement->getMonitorElement()->getDrawOption());

	gStyle->SetStatColor(kGray);

	this->GetCanvas()->cd();
	this->GetCanvas()->Clear();
	pObject->Draw(drawOption.c_str());
	this->GetCanvas()->Update();
	this->postDraw();

	this->updateMonitorElement(pMonitorElement);
}

//-------------------------------------------------------------------------------------------------

DQMGuiMonitorElement *DQMRootWidget::getCurrentMonitorElement() const
{
	return m_pCurrentMonitorElement;
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::showContextMenu(const QPoint &point)
{
    QMenu *pContextMenu = this->createContextMenu();
    pContextMenu->exec(this->mapToGlobal(point));
    pContextMenu->deleteLater();
}

//-------------------------------------------------------------------------------------------------

TImage *DQMRootWidget::getNoVisImage()
{
	if(NULL == m_pNoVisImage)
	{
		std::string noVisFileName = std::string(DQMViz_DIR) + "/icons/NO_VIS.xpm";
		m_pNoVisImage = TImage::Open(noVisFileName.c_str());
		m_pNoVisImage->SetEditable(kFALSE);
		m_pNoVisImage->SetConstRatio(0);
	}

	return m_pNoVisImage;
}

//-------------------------------------------------------------------------------------------------

QMenu *DQMRootWidget::createContextMenu() const
{
	QMenu *pContextMenu = new QMenu();

	QMenu *pMoveToCanvasAreaMenu = pContextMenu->addMenu("Move to canvas area");

	DQMCanvasView *pCanvasView = this->getMonitoring()->getView()->getCanvasView();
	DQMCanvasArea *pCurrentCanvasArea = pCanvasView->getCurrentCanvasArea();
	QList<DQMCanvasArea*> canvasAreaList = pCanvasView->canvasAreaList();

	if(canvasAreaList.count() == 1)
		pMoveToCanvasAreaMenu->setEnabled(false);
	else
	{
		for(int i=0 ; i<canvasAreaList.count() ; i++)
		{
			DQMCanvasArea *pCanvasArea = canvasAreaList.at(i);

			if(pCanvasArea == pCurrentCanvasArea)
				continue;

			QAction *pAction = pMoveToCanvasAreaMenu->addAction(pCanvasView->getCanvasAreaName(i).c_str(), this, SLOT(moveCanvas()));
			pAction->setData(i);
		}
	}

	pContextMenu->addAction("Update", this, SLOT(queryUpdate()));
	pContextMenu->addAction("Remove", this, SLOT(removeCanvas()));

	pContextMenu->addSeparator();

	QMenu *pSetDrawOptionMenu = pContextMenu->addMenu("Set draw option");

	if(!this->getCurrentMonitorElement())
	{
		pSetDrawOptionMenu->setEnabled(false);
	}
	else
	{
		pSetDrawOptionMenu->addAction(this->getCurrentMonitorElement()->getMonitorElement()->getDrawOption().c_str(), this, SLOT(setDrawOption()));
		pSetDrawOptionMenu->addSeparator();
		this->addDrawOptions(pSetDrawOptionMenu);
	}

    pContextMenu->addAction("UnZoom", this, SLOT(unzoom()));
    pContextMenu->addAction("Save as", this, SLOT(saveAs()));
    pContextMenu->addAction("ROOT Panel", this, SLOT(openROOTPanel()));
    pContextMenu->addSeparator();
    pContextMenu->addAction("Show info", this, SLOT(showMonitorElementInfo()));
    pContextMenu->addAction("QTest results", this, SLOT(showQTestResults()));

	return pContextMenu;
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::addDrawOptions(QMenu *pMenu) const
{
	pMenu->addAction("", this, SLOT(setDrawOption()));
	pMenu->addAction("E", this, SLOT(setDrawOption()));
	pMenu->addAction("LEGO", this, SLOT(setDrawOption()));
	pMenu->addSeparator();
	pMenu->addAction("COL", this, SLOT(setDrawOption()));
	pMenu->addAction("COLZ", this, SLOT(setDrawOption()));
	pMenu->addAction("SURF", this, SLOT(setDrawOption()));
	pMenu->addAction("SURF1", this, SLOT(setDrawOption()));
	pMenu->addSeparator();
    QMenu *pMoreDrawOptionMenu = pMenu->addMenu("More draw options");

    pMoreDrawOptionMenu->addAction("FUNC", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("LEGO1", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("LEGO2", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("E0", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("E1", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("E2", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("E3", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("E4", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("E5", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("E6", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addSeparator();
    pMoreDrawOptionMenu->addAction("ARR", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("BOX", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("SURF2", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("SURF3", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("SURF4", this, SLOT(setDrawOption()));
    pMoreDrawOptionMenu->addAction("SURF5", this, SLOT(setDrawOption()));
    pMenu->addSeparator();
    pMenu->addAction("Custom draw option", this, SLOT(setDrawOptionFromDialog()));
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::moveCanvas()
{
	QAction *pAction = (QAction*) sender();

	if(!pAction || !this->getCanvas())
		return;

	int index = pAction->data().toInt();
	DQMCanvasView *pCanvasView = this->getMonitoring()->getView()->getCanvasView();
	pCanvasView->moveCanvas(this->getCanvas(), index);
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::queryUpdate()
{
	if(!this->getCurrentMonitorElement())
		return;

	this->getMonitoring()->getController()->queryUpdate(this->getCurrentMonitorElement());
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::removeCanvas()
{
	if(!this->getCanvas())
		return;

	this->getMonitoring()->getView()->getCanvasView()->getCurrentCanvasArea()->removeCanvas(this->getCanvas(), true);
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::setDrawOption()
{
	QAction *pDrawOptionAction = qobject_cast<QAction*>(sender());

	if(!pDrawOptionAction || !this->getCurrentMonitorElement())
		return;

	this->getCurrentMonitorElement()->setDrawOption(pDrawOptionAction->text().toStdString());
	this->draw(this->getCurrentMonitorElement());
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::setDrawOptionFromDialog()
{
	if(!this->getCurrentMonitorElement())
		return;

    bool ok = false;
    QString oldDrawOption = this->getCurrentMonitorElement()->getMonitorElement()->getDrawOption().c_str();

    QString newDrawOption = QInputDialog::getText(this, "Set draw option",
                                               "", QLineEdit::Normal,
                                               oldDrawOption, &ok);

    if(ok)
    {
    	this->getCurrentMonitorElement()->setDrawOption(newDrawOption.toStdString());
    	this->draw(this->getCurrentMonitorElement());
    }
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::unzoom()
{
	if(!this->getCurrentMonitorElement())
		return;

	TH1 *pHistogram = this->getCurrentMonitorElement()->getMonitorElement()->get<TH1>();

	if(pHistogram)
	{
		this->GetCanvas()->cd();
		pHistogram->GetXaxis()->UnZoom();
		pHistogram->GetYaxis()->UnZoom();
		this->GetCanvas()->Modified();
		this->GetCanvas()->Update();
		this->postDraw();
	}
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::saveAs()
{
	this->getMonitoring()->getController()->saveAs(this->getCanvas());
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::openROOTPanel()
{
	if(!this->getCurrentMonitorElement())
		return;

	this->getMonitoring()->getController()->openInROOTWindow(this->getCurrentMonitorElement());
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::showMonitorElementInfo()
{
	if(!this->getCurrentMonitorElement())
		return;

	this->getMonitoring()->getController()->openMonitorElementInfo(this->getCurrentMonitorElement());
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::showQTestResults()
{
	if(!this->getCurrentMonitorElement())
		return;

	this->getMonitoring()->getController()->openQualityTestResults(this->getCurrentMonitorElement());
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::redraw()
{
	if(!this->getCurrentMonitorElement())
		return;

	this->draw(this->getCurrentMonitorElement());
}

//-------------------------------------------------------------------------------------------------

void DQMRootWidget::updateMonitorElement(DQMGuiMonitorElement *pNewGuiMonitorElement)
{
	DQMGuiMonitorElement *pCurrentGuiMonitorElement = this->getCurrentMonitorElement();

	if(NULL != pCurrentGuiMonitorElement && pCurrentGuiMonitorElement == pNewGuiMonitorElement)
	{
		QObject::disconnect(pCurrentGuiMonitorElement, SIGNAL(destroyed()), this, SLOT(removeCanvas()));
		QObject::disconnect(pCurrentGuiMonitorElement, SIGNAL(updated()), this, SLOT(redraw()));
	}

	if(pNewGuiMonitorElement)
	{
		QObject::connect(pNewGuiMonitorElement, SIGNAL(destroyed()), this, SLOT(removeCanvas()));
		QObject::connect(pNewGuiMonitorElement, SIGNAL(updated()), this, SLOT(redraw()));

		DQMCanvas *pCanvas = this->getCanvas();

		pCanvas->setWindowTitle(pNewGuiMonitorElement->getMonitorElement()->getTitle().c_str());
		pCanvas->setWindowIcon(pCanvas->getMonitoring()->getController()->getIcon(pNewGuiMonitorElement->getMonitorElement()->getQuality()));
	}

	m_pCurrentMonitorElement = pNewGuiMonitorElement;
}

} 

