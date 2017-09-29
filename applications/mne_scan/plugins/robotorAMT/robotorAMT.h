//=============================================================================================================
/**
* @file     robotorAMT.h
* @author   Paul Wenzel <paul.wenzel@tu-ilmenau.de>;;
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
* @brief    Contains the declaration of the robotorAMT class.
*
*/

#ifndef robotorAMT_H
#define robotorAMT_H


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "robotorAMT_global.h"

#include <scShared/Interfaces/IAlgorithm.h>
#include <generics/circularmatrixbuffer.h>
#include <scMeas/newrealtimesamplearray.h>
#include <fiff/fiff_info.h>

#include "FormFiles/robotorAMTsetupwidget.h"
#include "FormFiles/robotorAMTyourwidget.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QtWidgets>
#include <QtCore/QtPlugin>
#include <QDebug>


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE DummyToolboxPlugin
//=============================================================================================================

namespace robotorAMTPLUGIN
{


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace SCSHAREDLIB;


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* DECLARE CLASS robotorAMT
*
* @brief The DummyToolbox class provides a dummy algorithm structure.
*/
class robotorAMTSHARED_EXPORT robotorAMT : public IAlgorithm
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "scsharedlib/1.0" FILE "robotorAMT.json") //NEw Qt5 Plugin system replaces Q_EXPORT_PLUGIN2 macro
    // Use the Q_INTERFACES() macro to tell Qt's meta-object system about the interfaces
    Q_INTERFACES(SCSHAREDLIB::IAlgorithm)

public:
    //=========================================================================================================
    /**
    * Constructs a DummyToolbox.
    */
    robotorAMT();

    //=========================================================================================================
    /**
    * Destroys the DummyToolbox.
    */
    ~robotorAMT();

    //=========================================================================================================
    /**
    * IAlgorithm functions
    */
    virtual QSharedPointer<IPlugin> clone() const;
    virtual void init();
    virtual void unload();
    virtual bool start();
    virtual bool stop();
    virtual IPlugin::PluginType getType() const;
    virtual QString getName() const;
    virtual QWidget* setupWidget();

    //=========================================================================================================
    /**
    * Udates the plugin with new (incoming) data.
    *
    * @param[in] pMeasurement    The incoming data in form of a generalized NewMeasurement.
    */
    void update(SCMEASLIB::NewMeasurement::SPtr pMeasurement);

protected:
    //=========================================================================================================
    /**
    * IAlgorithm function
    */
    virtual void run();

    void showYourWidget();

private:
    bool                                            m_bIsRunning;           /**< Flag whether thread is running.*/
    QSharedPointer<robotorAMTYourWidget>            m_pYourWidget;          /**< flag whether thread is running.*/
    QAction*                                        m_pActionShowYourWidget;/**< flag whether thread is running.*/

    QStringList m_lCommands;

    SCSHAREDLIB::PluginInputData<SCMEASLIB::NewRealTimeSampleArray>::SPtr      m_probotorAMTInput;        /**< The first RealTimeSampleArray of the BCI output.*/

signals:
    //=========================================================================================================
    /**
    * Emitted when fiffInfo is available
    */
    void fiffInfoAvailable();
};

} // NAMESPACE

#endif // robotorAMT_H
