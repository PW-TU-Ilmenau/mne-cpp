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
, m_probotorAMTInputOne(NULL)
, m_probotorAMTInputTwo(NULL)
, m_probotorAMTInputThree(NULL)
, m_probotorAMTInputFour(NULL)
, m_probotorAMTInputFive(NULL)
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
    m_probotorAMTInputOne = PluginInputData<NewRealTimeSampleArray>::create(this, "Forward", "robotorAMT input data 1");
    connect(m_probotorAMTInputOne.data(), &PluginInputConnector::notify, this, &robotorAMT::updateForward, Qt::DirectConnection);
    m_inputConnectors.append(m_probotorAMTInputOne);

    m_probotorAMTInputTwo = PluginInputData<NewRealTimeSampleArray>::create(this, "Reverse", "robotorAMT input data 2");
    connect(m_probotorAMTInputTwo.data(), &PluginInputConnector::notify, this, &robotorAMT::updateReverse, Qt::DirectConnection);
    m_inputConnectors.append(m_probotorAMTInputTwo);

    m_probotorAMTInputThree = PluginInputData<NewRealTimeSampleArray>::create(this, "Left", "robotorAMT input data 3");
    connect(m_probotorAMTInputTwo.data(), &PluginInputConnector::notify, this, &robotorAMT::updateLeft, Qt::DirectConnection);
    m_inputConnectors.append(m_probotorAMTInputThree);

    m_probotorAMTInputFour = PluginInputData<NewRealTimeSampleArray>::create(this, "Right", "robotorAMT input data 4");
    connect(m_probotorAMTInputTwo.data(), &PluginInputConnector::notify, this, &robotorAMT::updateRight, Qt::DirectConnection);
    m_inputConnectors.append(m_probotorAMTInputFour);

    m_probotorAMTInputFive = PluginInputData<NewRealTimeSampleArray>::create(this, "Stop", "robotorAMT input data 5");
    connect(m_probotorAMTInputTwo.data(), &PluginInputConnector::notify, this, &robotorAMT::updateStop, Qt::DirectConnection);
    m_inputConnectors.append(m_probotorAMTInputFive);
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

void robotorAMT::updateForward(SCMEASLIB::NewMeasurement::SPtr pMeasurement)
{
    QSharedPointer<NewRealTimeSampleArray> pRTSA = pMeasurement.dynamicCast<NewRealTimeSampleArray>();

    if(pRTSA) {
        QString command = pRTSA->getName();
        QVector<double>  t_vec = pRTSA->getSampleArray();
        int residual = t_vec.size() - t_vec.count(0.0);
            for(int j = 0; j < residual; ++j) {
                m_lCommands << command;
            }
        if(residual > 0){
            qDebug()<<residual;
        }
//        qDebug()<<m_lCommands;
//        qDebug()<<command;
    }
}

//*************************************************************************************************************

void robotorAMT::updateReverse(SCMEASLIB::NewMeasurement::SPtr pMeasurement)
{
    QSharedPointer<NewRealTimeSampleArray> pRTSA = pMeasurement.dynamicCast<NewRealTimeSampleArray>();

    if(pRTSA) {
        QString command = pRTSA->getName();
        QVector<double>  t_vec = pRTSA->getSampleArray();
        int residual = t_vec.size() - t_vec.count(0.0);
            for(int j = 0; j < residual; ++j) {
                m_lCommands << command;
            }
        if(residual > 0){
            qDebug()<<residual;
        }
        qDebug()<<m_lCommands;
        qDebug()<<command;
    }
    qDebug()<<m_lCommands;
    qDebug()<<"Test";
}

//*************************************************************************************************************

void robotorAMT::updateLeft(SCMEASLIB::NewMeasurement::SPtr pMeasurement)
{
    QSharedPointer<NewRealTimeSampleArray> pRTSA = pMeasurement.dynamicCast<NewRealTimeSampleArray>();

    if(pRTSA) {
        QString command = pRTSA->getName();
        QVector<double>  t_vec = pRTSA->getSampleArray();
        int residual = t_vec.size() - t_vec.count(0.0);
            for(int j = 0; j < residual; ++j) {
                m_lCommands << command;
            }
        if(residual > 0){
            qDebug()<<residual;
        }
        qDebug()<<m_lCommands;
        qDebug()<<command;
    }
}

//*************************************************************************************************************

void robotorAMT::updateRight(SCMEASLIB::NewMeasurement::SPtr pMeasurement)
{
    QSharedPointer<NewRealTimeSampleArray> pRTSA = pMeasurement.dynamicCast<NewRealTimeSampleArray>();

    if(pRTSA) {
        QString command = pRTSA->getName();
        QVector<double>  t_vec = pRTSA->getSampleArray();
        int residual = t_vec.size() - t_vec.count(0.0);
            for(int j = 0; j < residual; ++j) {
                m_lCommands << command;
            }
        if(residual > 0){
            qDebug()<<residual;
        }
        qDebug()<<m_lCommands;
        qDebug()<<command;
    }
}

//*************************************************************************************************************

void robotorAMT::updateStop(SCMEASLIB::NewMeasurement::SPtr pMeasurement)
{
    QSharedPointer<NewRealTimeSampleArray> pRTSA = pMeasurement.dynamicCast<NewRealTimeSampleArray>();

    if(pRTSA) {
        QString command = pRTSA->getName();
        QVector<double>  t_vec = pRTSA->getSampleArray();
        int residual = t_vec.size() - t_vec.count(0.0);
            for(int j = 0; j < residual; ++j) {
                m_lCommands << command;
            }
        if(residual > 0){
            qDebug()<<residual;
        }
        qDebug()<<m_lCommands;
        qDebug()<<command;
    }
}

//*************************************************************************************************************

void robotorAMT::run()
{
//    QFile commands;
//    QString file_location, putty, file_name, amt_command;
//    QProcess *process;
//    QStringList file_names, file_content, arguments;

//    file_location = "C:/Users/ricky/Desktop/paul_wenzel";
//    // file_location, path to the directory we are using
//    putty = "C:/Users/ricky/Downloads/putty.exe";
//    // path of the Putty.exe

//    // creating the txt-files with the commands the Pi should receive
//    file_names << "/AMT_forward.txt" << "/AMT_reverse.txt" << "/AMT_left.txt" << "/AMT_right.txt" << "/AMT_stop.txt";
//    file_content << "sudo python Motor_fwd.py" << "sudo python Motor_reverse.py" << "sudo python Motor_left.py" << "sudo python Motor_right.py" << "sudo python Motor_stop.py";

//    for (int i = 0; i<5; i++){
//        file_name = file_location+file_names[i];
//        commands.setFileName(file_name);
//        commands.open(QIODevice::WriteOnly | QIODevice::Text);
//        QTextStream stream(&commands);
//        stream << file_content[i];
//        commands.close();
//    }

//    while(m_bIsRunning)
//    {
//        if(!m_lCommands.isEmpty()) {
//            qDebug() << m_lCommands;
//            QString bci_input = m_lCommands.takeFirst();

//            if (bci_input == "Forward"){
//                amt_command = file_location+file_names[0];
//            } else if (bci_input == "Reverse"){
//                amt_command = file_location+file_names[1];
//            } else if (bci_input == "Left"){
//                amt_command = file_location+file_names[2];
//            } else if (bci_input == "Right"){
//                amt_command = file_location+file_names[3];
//            } else if (bci_input == "Stop"){
//                amt_command = file_location+file_names[4];
//            }
//            // amt_command, the complete path to the txt-file we intend to send

//            process = new QProcess();
//            arguments << "-ssh" << "pi@192.168.1.1" << "22" << "-pw" << "r4spberry" << "-m" << amt_command;
//            // -ssh: type of the connection
//            // pi@... Username and Ip-Adress
//            // Portnumber, we use 22 here
//            // -pw: the following string will be passed as the password
//            // -m: instead of a shell, putty will enter the content of the following txt-file into the command-console of the Pi
//            process->startDetached(putty, arguments);
//        }
//    }
}


//*************************************************************************************************************

void robotorAMT::showYourWidget()
{
    m_pYourWidget = robotorAMTYourWidget::SPtr(new robotorAMTYourWidget());
    m_pYourWidget->show();
}
