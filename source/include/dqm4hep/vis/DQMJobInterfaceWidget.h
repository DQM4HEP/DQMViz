/// \file DQMJobInterfaceWidget.h
/*
 *
 * DQMJobInterfaceWidget.h header template automatically generated by a class generator
 * Creation date : lun. ao�t 24 2015
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


#ifndef DQMJOBINTERFACEWIDGET_H
#define DQMJOBINTERFACEWIDGET_H

// -- dqm4hep headers
#include "dqm4hep/DQM4HEP.h"

// -- qt headers
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QTreeWidget>
#include <QPushButton>
#include <QMainWindow>
#include <QSpinBox>
#include <QTextEdit>
#include <QShortcut>
#include <QCheckBox>

// -- json headers
#include "json/json.h"

namespace dqm4hep
{

class DQMJobInterface;

/** DQMJobInterfaceWidget class
 */
class DQMJobInterfaceWidget : public QWidget
{
    Q_OBJECT

public:
    /** ItemType enum
     */
    enum ItemType
    {
        HOST_ITEM,
        JOB_ITEM,
        JOB_STATUS_ITEM,
        JOB_PROGRAM_NAME_ITEM,
        JOB_PID_ITEM,
        JOB_ENV_ITEM,
        JOB_ARGS_ITEM,
        VARIABLE_ITEM
    };

    /** Constructor
     */
    DQMJobInterfaceWidget(QWidget *pParent = NULL);

    /** Destructor
     */
    ~DQMJobInterfaceWidget();

    /** Get the current json file name
     */
    const std::string &getCurrentJsonFile() const;

    /** Get the job interface implementation
     */
    DQMJobInterface *getJobInterface() const;

public slots:
    /** Load json file. Clear the pocess table
     */
    void loadJsonFile(const std::string &fileName);

    /**
     */
    void startHostJobs();

    /**
     */
    void startAllJobs();

    /**
     */
    void startSelectedJob();

    /**
     */
    void clearHostJobs();

    /**
     */
    void clearAllJobs();

    /**
     */
    void killSelectedJob();

    /**
     */
    void restartSelectedJob();

    /**
     */
    void restartHostJobs();

    /**
     */
    void restartAllJobs();

    /**
     */
    void updateJobStatus();

private slots:
    /**
     */
    void loadJsonFile();

    /**
     */
    void reloadJsonFile();

    /**
     */
    void openLogFile();

    /**
     */
    void handleAutomaticModeButtonClicked();

    /**
     */
    void handleAutomaticModeValueChanged(int value);

    /**
     */
    void updateStatus(const QString &hostName);

private:

    enum ColumnType
    {
        NAME,
        PROG_NAME,
        PID,
        STATUS
    };

    /**
     */
    void loadJson(const Json::Value &root);

    /**
     */
    void updateStatus(const Json::Value &value);

    /**
     */
    void createActions();

    /**
     */
    void createMenus();

    /**
     */
    void contextMenuEvent(QContextMenuEvent *event);

    /**
     */
    bool jobControlExists(const std::string &hostName) const;

    /**
     */
    QStringList getNonRunningJobControls() const;

    /**
     */
    void popupMissingJobControl(const QString &hostName);

    /**
     */
    void popupMissingJobControls(const QStringList &hostName);

    /**
     */
    void closeEvent(QCloseEvent* closeEvent);

private:

    DQMJobInterface        *m_pJobInterface;
    std::string             m_currentJsonFile;

    QPushButton            *m_pAutomaticModeButton;
    QSpinBox               *m_pUpdatePeriodSpinBox;

    QComboBox              *m_pKillComboBoxWidget;

    QTreeWidget            *m_pTreeWidget;

    QPushButton            *m_pLoadFileButton;
    QPushButton            *m_pReloadFileButton;

    QPushButton            *m_pStartHostJobsButton;
    QPushButton            *m_pClearHostJobsButton;
    QPushButton            *m_pKillJobButton;
    QPushButton            *m_pRestartJobButton;

    QPushButton            *m_pClearAllJobsButton;
    QPushButton            *m_pRestartAllJobsButton;
    QPushButton            *m_pStartAllJobsButton;

    QPushButton            *m_pUpdateButton;
    QPushButton            *m_pOpenLogFileButton;

    QMenuBar                *m_pMenuBar;
    QMenu                   *m_pContextMenu;
    QMenu                   *m_pFileMenu;
    QMenu                   *m_pHelpMenu;

    QAction                 *m_pLoadFileAction;
    QAction                 *m_pReloadFileAction;

    QAction                 *m_pStartHostJobsAction;
    QAction                 *m_pRestartHostJobsAction;
    QAction                 *m_pClearHostJobsAction;

    QAction                 *m_pKillJobAction;
    QAction                 *m_pStartJobAction;
    QAction                 *m_pRestartJobAction;

    QAction                 *m_pClearAllJobsAction;
    QAction                 *m_pRestartAllJobsAction;
    QAction                 *m_pStartAllJobsAction;

    QAction                 *m_pUpdateAction;
    QAction                 *m_pOpenLogFileAction;
};

/** DQMJobInterfaceLogFileWidget class
 */
class DQMJobInterfaceLogFileWidget : public QWidget
{
    Q_OBJECT

public:
    /** Constructor
         */
    DQMJobInterfaceLogFileWidget(DQMJobInterface *pParent, QString jobHostName, QString jobName, QString pidStr);

    /** Destructor
     */
    ~DQMJobInterfaceLogFileWidget();


private:
    DQMJobInterface         *m_pJobInterface;

    QString                  m_pidStr;
    QString                  m_jobHostName;
    QTextEdit               *m_pLogFileEdit;
    QPushButton             *m_pUpdateLogButton;
    QLineEdit               *m_pLineNumberEdit;
    QCheckBox               *m_pFullLogCheckBox;
    unsigned int             m_linesToTail;
    QTextCursor              m_cursor;
    QWidget                 *m_pSearchWidget;
    QLineEdit               *m_pSearchEdit;
    QPushButton             *m_pStartSearchButton;
    QPushButton             *m_pStartBackSearchButton;
    QString                  m_searchStr;
    QCheckBox               *m_pCaseCheckBox;
    QTextDocument::FindFlags m_caseFlags;

private slots:
    /**
     */
    void showSearchWidget();

    /**
     */
    void searchString();

    /**
     */
    void updateLogFile();

    /**
     */
    void handleFullLogToggle(bool checked);
};

}

#endif  //  DQMJOBINTERFACEWIDGET_H
