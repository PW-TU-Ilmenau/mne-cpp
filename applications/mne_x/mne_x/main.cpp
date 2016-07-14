//=============================================================================================================
/**
* @file     main.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     February, 2013
*
* @section  LICENSE
*
* Copyright (C) 2013, Christoph Dinh and Matti Hamalainen. All rights reserved.
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
* @brief    Implements the main() application function.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "mainsplashscreen.h"
#include "mainwindow.h"


#include <xMeas/measurementtypes.h>
#include <xMeas/newrealtimemultisamplearray.h>
#include <xMeas/newnumeric.h>

#include <xShared/Management/pluginconnectorconnection.h>
#include <xShared/Management/pluginoutputdata.h>
#include <xShared/Management/plugininputdata.h>
#include <xShared/Interfaces/IPlugin.h>


#include <Eigen/Core>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtGui>
#include <QApplication>
#include <QSharedPointer>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace XMEASLIB;
using namespace MNEX;
using namespace Eigen;


//*************************************************************************************************************
//=============================================================================================================
// global var
//=============================================================================================================


QSharedPointer<MainWindow> mainWin;

//=============================================================================================================
/**
* Custom Qt message handler.
*
* @param [in] type      enum to identify the various message types
* @param [in] context   additional information about a log message
* @param [in] msg       the message to log
*/
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

     QString dt = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
     QString txt = QString("[%1] ").arg(dt);

     if(mainWin)
     {
         switch (type)
         {
            case QtDebugMsg:
               txt += QString("{Debug} \t\t %1").arg(msg);
               mainWin->writeToLog(txt,_LogKndMessage, _LogLvMax);
               break;
            case QtWarningMsg:
               txt += QString("{Warning} \t %1").arg(msg);
               mainWin->writeToLog(txt,_LogKndWarning, _LogLvNormal);
               break;
            case QtCriticalMsg:
               txt += QString("{Critical} \t %1").arg(msg);
               mainWin->writeToLog(txt,_LogKndError, _LogLvMin);
               break;
            case QtFatalMsg:
               txt += QString("{Fatal} \t\t %1").arg(msg);
               mainWin->writeToLog(txt,_LogKndError, _LogLvMin);
               abort();
               break;
         }
     }

//     QFile outFile("LogFile.log");
//     outFile.open(QIODevice::WriteOnly | QIODevice::Append);

//     QTextStream textStream(&outFile);
//     textStream << txt << endl;

}



//void debugTest(QSharedPointer<NewRealTimeMultiSampleArray> testData)
//{
//    qDebug() << "Here in debug Test Callback new";

//    QVector< VectorXd > matSamples = testData->getMultiSampleArray();
//    qDebug() << "Received data:";
//    for(qint32 i = 0; i < matSamples.size(); ++i)
//        qDebug() << matSamples[i][0];


//}

//*************************************************************************************************************
//=============================================================================================================
// MAIN
//=============================================================================================================

//=============================================================================================================
/**
* The function main marks the entry point of the program.
* By default, main has the storage class extern.
*
* @param [in] argc (argument count) is an integer that indicates how many arguments were entered on the command line when the program was started.
* @param [in] argv (argument vector) is an array of pointers to arrays of character objects. The array objects are null-terminated strings, representing the arguments that were entered on the command line when the program was started.
* @return the value that was set to exit() (which is 0 if exit() is called via quit()).
*/
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //Store application info to use QSettings
    QCoreApplication::setOrganizationName("MNE-CPP");
    QCoreApplication::setOrganizationDomain("www.tu-ilmenau.de/mne-cpp");
    QCoreApplication::setApplicationName(CInfo::AppNameShort());

    XMEASLIB::MeasurementTypes::registerTypes();

    QPixmap pixmap(":/images/splashscreen.png");
    MainSplashScreen::SPtr splashscreen(new MainSplashScreen(pixmap));
    splashscreen->show();

    //ToDo Debug Some waiting stuff to see splash screen -> remove this in final release
    int time = 100;
    for(int i=0; i < time;++i)
    {
        int p = (i*100)/time;
        splashscreen->showMessage("Loading modules.."+ QString::number(p)+"%");
    }

    mainWin = QSharedPointer<MainWindow>(new MainWindow);
    mainWin->show();

    splashscreen->finish(mainWin.data());

    //ToDo Check the message handler and FiffSimulator

//    qInstallMessageHandler(customMessageHandler);


//    //DEBUG
//    NewRealTimeMultiSampleArray RTSATest;
//    NewNumeric NumericTest;

//    IPlugin* pluginInterface = NULL;

//    QSharedPointer< PluginOutputData<NewRealTimeMultiSampleArray> > pluginOutputData(new PluginOutputData<NewRealTimeMultiSampleArray>(pluginInterface, QString("TestPlugin"), QString("No Descr")));
//    QSharedPointer< PluginInputData<NewRealTimeMultiSampleArray> > pluginInputData(new PluginInputData<NewRealTimeMultiSampleArray>(pluginInterface, QString("TestPlugin2"), QString("No Descr2")));
//    pluginInputData->setCallbackMethod(&debugTest);

//    PluginConnectorConnection outInConnection(pluginOutputData, pluginInputData);

//    pluginOutputData->data()->init(2);
//    pluginOutputData->data()->setMultiArraySize(2);

//    VectorXd v = VectorXd::Zero(2);
//    v[0] = 2.3;
//    pluginOutputData->data()->setValue(v);
//    v[0] = 4.1;
//    pluginOutputData->data()->setValue(v);
//    //DEBUG

    return app.exec();
}
