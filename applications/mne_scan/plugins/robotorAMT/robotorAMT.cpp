//=============================================================================================================
/**
* @file     robotorAMT.cpp
* @author   Paul Wenzel <paul.wenzel@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2017
*
* @section  LICENSE
*
* Copyright (C) 2017, Paul Wenzel and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Contains the implementation of the robotorAMT class.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "robotorAMT.h"


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace robotorAMTPLUGIN;
using namespace SCSHAREDLIB;
using namespace SCMEASLIB;
using namespace IOBUFFER;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

robotorAMT::robotorAMT()
: m_bIsRunning(false)
, m_probotorAMTInput(NULL)
{
    //Add action which will be visible in the plugin's toolbar
    m_pActionShowYourWidget = new QAction(QIcon(":/images/options.png"), tr("Your Toolbar Widget"),this);
    m_pActionShowYourWidget->setShortcut(tr("F12"));
    m_pActionShowYourWidget->setStatusTip(tr("Your Toolbar Widget"));
    connect(m_pActionShowYourWidget, &QAction::triggered,
            this, &robotorAMT::showYourWidget);
    addPluginAction(m_pActionShowYourWidget);
}


//*************************************************************************************************************

robotorAMT::~robotorAMT()
{
    if(this->isRunning())
        stop();
}


//*************************************************************************************************************

QSharedPointer<IPlugin> robotorAMT::clone() const
{
    QSharedPointer<robotorAMT> probotorAMTClone(new robotorAMT);
    return probotorAMTClone;
}


//*************************************************************************************************************

void robotorAMT::init()
{
    // Input
    m_probotorAMTInput = PluginInputData<NewRealTimeSampleArray>::create(this, "Input", "Input");
    connect(m_probotorAMTInput.data(), &PluginInputConnector::notify, this, &robotorAMT::update, Qt::DirectConnection);
    m_inputConnectors.append(m_probotorAMTInput);
}


//*************************************************************************************************************

void robotorAMT::unload()
{

}


//*************************************************************************************************************

bool robotorAMT::start()
{
    //Check if the thread is already or still running. This can happen if the start button is pressed immediately after the stop button was pressed. In this case the stopping process is not finished yet but the start process is initiated.
    if(this->isRunning())
        QThread::wait();

    m_bIsRunning = true;

    //Start thread
    QThread::start();

    return true;
}


//*************************************************************************************************************

bool robotorAMT::stop()
{
    m_bIsRunning = false;
    return true;
}


//*************************************************************************************************************

IPlugin::PluginType robotorAMT::getType() const
{
    return _IAlgorithm;
}


//*************************************************************************************************************

QString robotorAMT::getName() const
{
    return "robotorAMT";
}


//*************************************************************************************************************

QWidget* robotorAMT::setupWidget()
{
    robotorAMTSetupWidget* setupWidget = new robotorAMTSetupWidget(this);//widget is later distroyed by CentralWidget - so it has to be created everytime new
    return setupWidget;
}


//*************************************************************************************************************

void robotorAMT::update(SCMEASLIB::NewMeasurement::SPtr pMeasurement)
{
    QSharedPointer<NewRealTimeSampleArray> pRTSA = pMeasurement.dynamicCast<NewRealTimeSampleArray>();

    if(pRTSA) {
        QVector<double>  t_vec = pRTSA->getSampleArray();
        for(int j = 0; j < t_vec.size(); ++j) {
            switch ((int)t_vec.at(j)) {
            case 10000:
                m_lCommands << "Forward";
                break;
            case 20000:
                m_lCommands << "Reverse";
                break;
            case 30000:
                m_lCommands << "Left";
                break;
            case 40000:
                m_lCommands << "Right";
                break;
            case 50000:
                m_lCommands << "Stop";
                break;
            default:
                break;
            }
        }
    }
}


//*************************************************************************************************************

void robotorAMT::run()
{
    QString com;

    while(m_bIsRunning)
    {
        if(!m_lCommands.isEmpty()) {
            qDebug() << m_lCommands;
            QString bci_input = m_lCommands.takeFirst();

            if (bci_input == "Forward"){
                com = "1";
            } else if (bci_input == "Reverse"){
                com = "2";
            } else if (bci_input == "Left"){
                com = "3";
            } else if (bci_input == "Right"){
                com = "4";
            } else if (bci_input == "Stop"){
                com = "5";
            }

            qDebug() << com;

            QStringList amt_command;
            amt_command << "BCItoPi_BCIClient.py " << com;

            qDebug() << amt_command;

            QString python = "C:/Users/ricky/AppData/Local/Programs/Python/Python36-32/python.exe";

            QProcess process;
            process.setWorkingDirectory("C:/Users/ricky/AppData/Local/Programs/Python/Python36-32");
            process.start(python, amt_command);
            process.waitForFinished(-1);
        }
    }
}


//*************************************************************************************************************

void robotorAMT::showYourWidget()
{
    m_pYourWidget = robotorAMTYourWidget::SPtr(new robotorAMTYourWidget());
    m_pYourWidget->show();
}
