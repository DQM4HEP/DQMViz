  /// \file DQMRunControlInterfaceWidget.cc
/*
 *
 * DQMRunControlInterfaceWidget.cc source template automatically generated by a class generator
 * Creation date : sam. avr. 30 2016
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

// -- dqm4hep headers
#include "dqm4hep/vis/DQMRunControlInterfaceWidget.h"
#include "dqm4hep/vis/DQMGuiTools.h"
#include "dqm4hep/DQMRun.h"
#include "dqm4hep/DQMCoreTool.h"
#include "dqm4hep/DQMXmlHelper.h"
#include "dqm4hep/tinyxml.h"

// -- qt headers
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QInputDialog>
#include <QMenu>

// -- std headers
#include <fstream>

namespace dqm4hep
{

DQMRunControlInterfaceWidget::DQMRunControlInterfaceWidget() :
		QMainWindow()
{
	DQMGuiTools::checkDimDnsNode( this );

	QString iconDir = QString(DQM4HEP_DIR) + "/icons";
	this->setWindowIcon(QIcon(iconDir + "/RUN_CTRL.png"));

	QWidget *pCentralWidget = new QWidget();
	pCentralWidget->setLayout(new QVBoxLayout());
	this->setCentralWidget(pCentralWidget);

    QLayout *pRunControlLayout = pCentralWidget->layout();

    // Run action group box
    m_pRunActionGroupBox = new QGroupBox("Run info", this);
    QFormLayout *pRunActionLayout = new QFormLayout();
    pRunActionLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    m_pRunActionGroupBox->setLayout(pRunActionLayout);
    pRunControlLayout->addWidget(m_pRunActionGroupBox);

    m_pRunNumberSpinBox = new QSpinBox();
    m_pRunNumberSpinBox->setMinimum(1);
    m_pRunNumberSpinBox->setMaximum(std::numeric_limits<int>::max());
    m_pRunNumberSpinBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_pRunNumberSpinBox->setAlignment(Qt::AlignRight);
    pRunActionLayout->addRow("Run number : ", m_pRunNumberSpinBox);

    m_pRunDescriptionTextEdit = new QTextEdit();
    pRunActionLayout->addRow("Run description : ", m_pRunDescriptionTextEdit);

    m_pRunParametersTableWidget = new QTableWidget(0,2);
    m_pRunParametersTableWidget->setSelectionMode(QTableWidget::ExtendedSelection);

    m_pRunParametersTableWidget->setHorizontalHeaderLabels(QStringList() << "Parameter" << "Value");
    QHeaderView* pHeaderView = m_pRunParametersTableWidget->horizontalHeader();
    pHeaderView->setResizeMode(QHeaderView::Stretch);
    pRunActionLayout->addRow("Run Parameters : ", m_pRunParametersTableWidget);

    QWidget *pParameterButtonWidget = new QWidget();
    QHBoxLayout *pParameterButtonLayout = new QHBoxLayout();
    pParameterButtonWidget->setLayout(pParameterButtonLayout);

    pParameterButtonLayout->addSpacerItem(new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    m_pAddParameterButton = new QPushButton("Add Parameter");
    pParameterButtonLayout->addWidget(m_pAddParameterButton);

    m_pDeleteParameterButton = new QPushButton("Delete Parameter");
    pParameterButtonLayout->addWidget(m_pDeleteParameterButton);
    pRunActionLayout->addWidget(pParameterButtonWidget);

    m_pDetectorNameEdit = new QLineEdit();
    pRunActionLayout->addRow("Detector name : ", m_pDetectorNameEdit);

    QWidget *pRunActionButtonWidget = new QWidget();
    QHBoxLayout *pRunActionButtonLayout = new QHBoxLayout();
    pRunActionButtonWidget->setLayout(pRunActionButtonLayout);

    QFont buttonFont;
    buttonFont.setWeight(QFont::Bold);
    buttonFont.setPointSize(15);

    m_pStartOfRunButton = new QPushButton("Start run");
    m_pStartOfRunButton->setMinimumHeight(70);
    m_pStartOfRunButton->setFont(buttonFont);
    pRunActionButtonLayout->addWidget(m_pStartOfRunButton);

    m_pEndOfRunButton = new QPushButton("End run");
    pRunActionButtonLayout->addWidget(m_pEndOfRunButton);
    m_pEndOfRunButton->setMinimumHeight(70);
    m_pEndOfRunButton->setFont(buttonFont);
    m_pEndOfRunButton->setEnabled(false);

    pRunControlLayout->addWidget(pRunActionButtonWidget);

	// run status group box
	QGroupBox *pRunStatusGroupBox = new QGroupBox("Run status");
	QGridLayout *pRunStatusLayout = new QGridLayout();
	pRunStatusGroupBox->setLayout(pRunStatusLayout);
	pRunControlLayout->addWidget(pRunStatusGroupBox);

	pRunStatusLayout->addWidget(new QLabel("Run number"), 0, 0, Qt::AlignCenter);
	pRunStatusLayout->addWidget(new QLabel("Run state"), 0, 1, Qt::AlignCenter);
	pRunStatusLayout->addWidget(new QLabel("Start time"), 0, 2, Qt::AlignCenter);
	pRunStatusLayout->addWidget(new QLabel("Elapsed time"), 0, 3, Qt::AlignCenter);

	m_pRunNumberLabel = new QLabel(QString("<b>None</b>"));
	pRunStatusLayout->addWidget(m_pRunNumberLabel, 1, 0, Qt::AlignCenter);
	m_pRunStateLabel = new QLabel(stateToString(STOPPED_STATE).c_str());
	pRunStatusLayout->addWidget(m_pRunStateLabel, 1, 1, Qt::AlignCenter);
	m_pRunStartTimeLabel = new QLabel("None");
	pRunStatusLayout->addWidget(m_pRunStartTimeLabel, 1, 2, Qt::AlignCenter);
	m_pRunElapsedTimeLabel = new QLabel("None");
	pRunStatusLayout->addWidget(m_pRunElapsedTimeLabel, 1, 3, Qt::AlignCenter);

	m_pLogger = new DQMLoggerWidget();
	pCentralWidget->layout()->addWidget(m_pLogger);

	// menus
	QMenu *pFileMenu = this->menuBar()->addMenu("&File");
	pFileMenu->addAction("&Import", this, SLOT(importFile()));
	pFileMenu->addAction("&Export", this, SLOT(exportFile()));

	// signal/slot connections
    connect(m_pDeleteParameterButton, SIGNAL(clicked()), this, SLOT(deleteRow()));
    connect(m_pAddParameterButton, SIGNAL(clicked()), this, SLOT(addNewRow()));
	connect(m_pStartOfRunButton, SIGNAL(clicked()), this, SLOT(sendStartOfRun()));
	connect(m_pEndOfRunButton, SIGNAL(clicked()), this, SLOT(sendEndCurrentRun()));

	connect(this, SIGNAL(onStartOfRun()), this, SLOT(fillCurrentRunInfo()));
	connect(this, SIGNAL(onStartOfRun()), this, SLOT(tidyButtonState()));
	connect(this, SIGNAL(onStartOfRun()), &m_elapsedTimeTimer, SLOT(start()));

	connect(this, SIGNAL(onEndOfRun()), this, SLOT(fillCurrentRunInfo()));
	connect(this, SIGNAL(onEndOfRun()), this, SLOT(tidyButtonState()));
	connect(this, SIGNAL(onEndOfRun()), &m_elapsedTimeTimer, SLOT(stop()));

	connect( &m_elapsedTimeTimer, SIGNAL(timeout()), this, SLOT(updateElapsedTime()));

	m_elapsedTimeTimer.setInterval(1000);
	m_elapsedTimeTimer.setSingleShot(false);

    m_pRunControlClient = new DQMDimRunControlClient();
    m_pRunControlClient->addListener( this );
}

DQMRunControlInterfaceWidget::~DQMRunControlInterfaceWidget()
{
	if( m_pRunControlClient->isConnectedToService() )
		m_pRunControlClient->disconnectFromService();

	delete m_pRunControlClient;
}

void DQMRunControlInterfaceWidget::setRunControlName(const std::string &name)
{
	if( m_pRunControlClient->getRunControlName() == name )
		return;

	if( m_pRunControlClient->isConnectedToService() )
		m_pRunControlClient->disconnectFromService();

	m_pRunControlClient->setRunControlName( name );
	m_pRunControlClient->connectToService();

	this->setWindowTitle();
}


std::string DQMRunControlInterfaceWidget::getRunControlName() const
{
	return m_pRunControlClient->getRunControlName();
}

std::string DQMRunControlInterfaceWidget::getDetectorName() const
{
	return m_pRunControlClient->isRunning() ? m_pRunControlClient->getCurrentRun()->getDetectorName() : "";
}

std::string DQMRunControlInterfaceWidget::getDescription() const
{
	return m_pRunControlClient->isRunning() ? m_pRunControlClient->getCurrentRun()->getDescription() : "";
}

std::string DQMRunControlInterfaceWidget::getStartTime() const
{
	if( m_pRunControlClient->isRunning() )
	{
		DQMRun *pRun = m_pRunControlClient->getCurrentRun();

		std::string startTime;
		DQMCoreTool::timeToHMS( std::chrono::system_clock::to_time_t(pRun->getStartTime()), startTime );

		return startTime;
	}
	else
		return "None";
}

std::string DQMRunControlInterfaceWidget::getElapsedTime() const
{
	if( m_pRunControlClient->isRunning() )
	{
		DQMRun *pRun = m_pRunControlClient->getCurrentRun();

		std::string elapsedTime;
        tzset();
        time_t startTime = std::chrono::system_clock::to_time_t(pRun->getStartTime());
        if (daylight) // convert to UTC
            startTime -= timezone;

		DQMCoreTool::timeToHMS( time(0) - startTime, elapsedTime );
		return elapsedTime;
	}
	else
		return "None";
}

std::string DQMRunControlInterfaceWidget::getStatus() const
{
	return stateToString( m_pRunControlClient->getRunState() );
}

DQMParameters DQMRunControlInterfaceWidget::getRunParameters() const
{
	DQMParameters parameters;

	if( ! m_pRunControlClient->isRunning() )
		return parameters;

	StringVector keys( m_pRunControlClient->getCurrentRun()->getParameterKeys() );

	for(auto iter = keys.begin(), endIter = keys.end() ; endIter != iter ; ++iter)
	{
		std::string parameter;
		m_pRunControlClient->getCurrentRun()->getParameter( *iter , parameter );
		parameters[ *iter ] = parameter;
	}

	return parameters;
}

int DQMRunControlInterfaceWidget::getRunNumber() const
{
	return m_pRunControlClient->isRunning() ? m_pRunControlClient->getCurrentRunNumber() : 0;
}

void DQMRunControlInterfaceWidget::onStartOfRun(DQMRun *const /*pRun*/)
{
	emit onStartOfRun();
}

void DQMRunControlInterfaceWidget::onEndOfRun(const DQMRun *const /*pRun*/)
{
	emit onEndOfRun();
}

void DQMRunControlInterfaceWidget::importFile(const std::string &fileName)
{
    TiXmlDocument xmlDocument(fileName);

    if (!xmlDocument.LoadFile())
    {
        QMessageBox::warning(this, "Warning", "Invalid xml file");
        return;
    }

    const TiXmlHandle xmlDocumentHandle(&xmlDocument);
    const TiXmlHandle xmlHandle = TiXmlHandle(xmlDocumentHandle.FirstChildElement().Element());

    TiXmlElement *const pRunInfoElement = xmlHandle.FirstChildElement("runinfo").Element();

    if(!pRunInfoElement)
    {
        QMessageBox::warning(this, "Warning", "Invalid xml file : Couldn't find <runinfo> element !");
        return;
    }

    TiXmlHandle runInfoHandle(pRunInfoElement);

    int runNumber = 0;
    std::string detectorName;
    std::string description;

    m_pRunParametersTableWidget->clearContents();

    while (m_pRunParametersTableWidget->rowCount() > 0)
        m_pRunParametersTableWidget->removeRow(0);

    try
    {
        TiXmlElement *const pRunNumberElement = runInfoHandle.FirstChildElement("runnumber").Element();
        TiXmlElement *const pDetectorElement = runInfoHandle.FirstChildElement("detector").Element();
        TiXmlElement *const pDescriptionElement = runInfoHandle.FirstChildElement("description").Element();
        TiXmlElement *const pParametersElement = runInfoHandle.FirstChildElement("parameters").Element();

        if(pRunNumberElement)
        {
        	THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pRunNumberElement, "value", runNumber));
        }

        if(pDetectorElement)
        {
        	THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pDetectorElement, "name", detectorName));
        }

        if(pDescriptionElement)
        {
        	THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pDescriptionElement, "value", description));
        }

        if(pParametersElement)
        {
        	TiXmlHandle parametersHandle(pParametersElement);

            for (TiXmlElement *pXmlElement = parametersHandle.FirstChild("parameter").Element(); NULL != pXmlElement;
                pXmlElement = pXmlElement->NextSiblingElement("parameter"))
            {
            	std::string name;
            	std::string value;

            	THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pXmlElement, "name", name));
            	THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pXmlElement, "value", value));

                int row = m_pRunParametersTableWidget->rowCount();

                m_pRunParametersTableWidget->insertRow(row);
                m_pRunParametersTableWidget->setItem(row, 0, new QTableWidgetItem(name.c_str()));
                m_pRunParametersTableWidget->setItem(row, 1, new QTableWidgetItem(value.c_str()));
            }
        }
    }
	catch(const StatusCodeException &exception)
	{
        QMessageBox::warning(this, "Warning", ("Invalid xml file : Couldn't parse it : " + exception.toString()).c_str());
        return;
	}

    m_pRunNumberSpinBox->setValue(runNumber);
    m_pRunDescriptionTextEdit->clear();
    m_pRunDescriptionTextEdit->setText(description.c_str());
    m_pDetectorNameEdit->setText(detectorName.c_str());
}

void DQMRunControlInterfaceWidget::exportFile(const std::string &fileName)
{
	TiXmlDocument document;

	TiXmlDeclaration *pDeclaration = new TiXmlDeclaration( "1.0", "", "" );
	document.LinkEndChild(pDeclaration);

	TiXmlElement *pRootElement = new TiXmlElement("dqm4hep");
	document.LinkEndChild(pRootElement);

	TiXmlElement *pRunInfoElement = new TiXmlElement("runinfo");
	pRootElement->LinkEndChild(pRunInfoElement);

	TiXmlElement *pRunNumberElement = new TiXmlElement("runnumber");
	pRunNumberElement->SetAttribute("value", static_cast<int>(m_pRunNumberSpinBox->value()));
	pRunInfoElement->LinkEndChild(pRunNumberElement);

	TiXmlElement *pDetectorElement = new TiXmlElement("detector");
	pDetectorElement->SetAttribute("name", m_pDetectorNameEdit->text().toStdString());
	pRunInfoElement->LinkEndChild(pDetectorElement);

	TiXmlElement *pDescriptionElement = new TiXmlElement("description");
	pDescriptionElement->SetAttribute("value", m_pRunDescriptionTextEdit->toPlainText().toStdString());
	pRunInfoElement->LinkEndChild(pDescriptionElement);


    if(m_pRunParametersTableWidget->rowCount() > 0)
    {
    	TiXmlElement *pParametersElement = new TiXmlElement("parameters");
    	pRunInfoElement->LinkEndChild(pParametersElement);

        for(unsigned int i=0 ; i<m_pRunParametersTableWidget->rowCount() ; i++)
        {
            QTableWidgetItem *pKeyItem = m_pRunParametersTableWidget->item(i, 0);
            QTableWidgetItem *pValueItem = m_pRunParametersTableWidget->item(i, 1);

            if(!pKeyItem || !pValueItem)
                continue;

        	TiXmlElement *pParameterElement = new TiXmlElement("parameter");
        	pParameterElement->SetAttribute("name", pKeyItem->text().toStdString());
        	pParameterElement->SetAttribute("value", pValueItem->text().toStdString());
        	pParametersElement->LinkEndChild(pParameterElement);
        }
    }

	document.SaveFile(fileName);
}

void DQMRunControlInterfaceWidget::importFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import", "", "Xml (*.xml)");
    this->importFile(fileName.toStdString());
}

void DQMRunControlInterfaceWidget::exportFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export", "", "XML (*.xml)");
    this->exportFile(fileName.toStdString());
}

void DQMRunControlInterfaceWidget::fillCurrentRunInfo()
{
    m_pRunNumberSpinBox->setValue( this->getRunNumber() );
    m_pRunDescriptionTextEdit->clear();
    m_pRunDescriptionTextEdit->setText( this->getDescription().c_str() );
    m_pDetectorNameEdit->setText( this->getDetectorName().c_str() );

    m_pRunParametersTableWidget->clearContents();

    while (m_pRunParametersTableWidget->rowCount() > 0)
        m_pRunParametersTableWidget->removeRow(0);

    DQMParameters runParameters( this->getRunParameters() );

    for (auto iter = runParameters.begin(), endIter = runParameters.end() ; endIter != iter ; ++iter)
    {
        int row = m_pRunParametersTableWidget->rowCount();

        m_pRunParametersTableWidget->insertRow(row);
        m_pRunParametersTableWidget->setItem(row, 0, new QTableWidgetItem(iter->first.c_str()));
        m_pRunParametersTableWidget->setItem(row, 1, new QTableWidgetItem(iter->second.c_str()));
    }

    QString stateColor = m_pRunControlClient->isRunning() ? "green" : "red";
    QString state = QString("<b style=\"color:%1;\">%2</b>").arg(stateColor).arg(this->getStatus().c_str());
    m_pRunNumberLabel->setText(QString("<b>%1</b>").arg(this->getRunNumber()));
    m_pRunStateLabel->setText( state );
    m_pRunStartTimeLabel->setText( this->getStartTime().c_str() );
    m_pRunElapsedTimeLabel->setText( this->getElapsedTime().c_str() );

    this->setWindowTitle();
}

void DQMRunControlInterfaceWidget::sendStartOfRun()
{
	int runNumber = m_pRunNumberSpinBox->value();

    std::string description( m_pRunDescriptionTextEdit->toPlainText().toStdString() );
    std::string detectorName( m_pDetectorNameEdit->text().toStdString() );

    DQMRun *pRun = new DQMRun(runNumber, description, detectorName);

    for(unsigned int i=0 ; i<m_pRunParametersTableWidget->rowCount() ; i++)
    {
        QTableWidgetItem *pKeyItem = m_pRunParametersTableWidget->item(i, 0);
        QTableWidgetItem *pValueItem = m_pRunParametersTableWidget->item(i, 1);

        if(!pKeyItem || !pValueItem)
            continue;

        pRun->setParameter(pKeyItem->text().toStdString(), pValueItem->text().toStdString());
    }

    bool ok = true;

    QString password = QInputDialog::getText(this,
    		"Run control password",
			"Password : ",
			QLineEdit::Password,
			"",
			&ok);

    if( ! ok )
    {
    	delete pRun;
    	return;
    }

    try
    {
    	THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_pRunControlClient->sendStartNewRun(pRun, password.toStdString()));
    }
    catch(StatusCodeException &exception)
    {
        if(m_pLogger)
            m_pLogger->log(ERROR, "Couldn't start new run : " + exception.toString());

        if( pRun )
        	delete pRun;

        return;
    }

    this->tidyButtonState();
}

void DQMRunControlInterfaceWidget::sendEndCurrentRun()
{
    bool ok = true;

    QString password = QInputDialog::getText(this,
    		"Run control password",
			"Password : ",
			QLineEdit::Password,
			"",
			&ok);

    if( ! ok )
    	return;

    try
    {
    	THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_pRunControlClient->sendEndCurrentRun(password.toStdString()));
    }
    catch(StatusCodeException &exception)
    {
        if(m_pLogger)
            m_pLogger->log(ERROR, "Couldn't end current run : " + exception.toString());

        return;
    }

    this->tidyButtonState();
}





void DQMRunControlInterfaceWidget::addNewRow()
{
	m_pRunParametersTableWidget->insertRow(m_pRunParametersTableWidget->rowCount());
}

void DQMRunControlInterfaceWidget::deleteRow()
{
    QList<QTableWidgetSelectionRange> selectedRanges = m_pRunParametersTableWidget->selectedRanges();
    for (int i = 0; i< selectedRanges.size(); i++ )
    {
        int topRow = selectedRanges.at(i).topRow();
        int botRow = selectedRanges.at(i).bottomRow();
        int inc = topRow;
        while(inc <= botRow)
        {
            m_pRunParametersTableWidget->removeRow(topRow);
            inc++;
        }
    }
    m_pRunParametersTableWidget->setCurrentCell(-1,-1);
}


void DQMRunControlInterfaceWidget::updateElapsedTime()
{
	if( ! m_pRunControlClient->isRunning() )
		m_elapsedTimeTimer.stop();

	std::string elapsedTime( this->getElapsedTime() );
	m_pRunElapsedTimeLabel->setText( elapsedTime.c_str() );
}

void DQMRunControlInterfaceWidget::tidyButtonState()
{
	if( m_pRunControlClient->isRunning() )
	{
		m_pStartOfRunButton->setEnabled( false );
		m_pEndOfRunButton->setEnabled( true );
	}
	else
	{
		m_pStartOfRunButton->setEnabled( true );
		m_pEndOfRunButton->setEnabled( false );
	}
}


void DQMRunControlInterfaceWidget::setWindowTitle()
{
	QString name = m_pRunControlClient->getRunControlName().c_str();
//    QString stateColor = m_pRunControlClient->isRunning() ? "green" : "red";
//    QString state = QString("<span style=\"color:%1;\">%2</span>").arg(stateColor).arg(this->getStatus().c_str());
	QString state = this->getStatus().c_str();
    QString windowTitle = QString("Run control (%1) , state = [%2]").arg(name).arg(state);

    QWidget::setWindowTitle( windowTitle );
}

}

