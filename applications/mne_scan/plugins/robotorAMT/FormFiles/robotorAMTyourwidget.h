//=============================================================================================================
/**
* @file     robotorAMTaboutwidget.h
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
* @brief    Contains the declaration of the robotorAMTYourWidget class.
*
*/

#ifndef robotorAMTYOURWIDGET_H
#define robotorAMTYOURWIDGET_H

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "../ui_robotorAMTyourtoolbarwidget.h"


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QWidget>


//*************************************************************************************************************
//=============================================================================================================
// DEFINE NAMESPACE DummyToolboxPlugin
//=============================================================================================================

namespace robotorAMTPLUGIN
{


//*************************************************************************************************************
//=============================================================================================================
// FORWARD DECLARATIONS
//=============================================================================================================


//=============================================================================================================
/**
* DECLARE CLASS robotorAMTYourWidget
*
* @brief The robotorAMT class provides a dummy toolbar widget structure.
*/
class robotorAMTYourWidget : public QWidget
{
    Q_OBJECT

public:
    typedef QSharedPointer<robotorAMTYourWidget> SPtr;         /**< Shared pointer type for DummyYourWidget. */
    typedef QSharedPointer<robotorAMTYourWidget> ConstSPtr;    /**< Const shared pointer type for DummyYourWidget. */

    //=========================================================================================================
    /**
    * Constructs a DummyToolbox.
    */
    explicit robotorAMTYourWidget(QWidget *parent = 0);

    //=========================================================================================================
    /**
    * Destroys the robotorAMT.
    */
    ~robotorAMTYourWidget();

private:
    Ui::robotorAMTYourToolbarWidget* ui;        /**< The UI class specified in the designer. */
};

}   //namespace

#endif // robotorAMTYOURWIDGET_H
