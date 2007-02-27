/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef MITKPROGRESSBAR_H
#define MITKPROGRESSBAR_H
#include <itkObject.h>
#include "mitkProgressBarImplementation.h"


namespace mitk 
{

  //##Documentation
  //## @brief Sending a message to the applications ProgressBar
  //##
  //## Holds a GUI dependent ProgressBarImplementation and sends the progress further.
  //## All mitk-classes use this class to display progress on GUI-ProgressBar.
  //## The mainapplication has to set the internal held ProgressBarImplementation with SetImplementationInstance(..).
  //## @ingroup Interaction
  class ProgressBar : public itk::Object
  {

  public:
    itkTypeMacro(ProgressBar, itk::Object);
  
    //##Documentation
    //## @brief static method to get the GUI dependent ProgressBar-instance 
    //## so the methods for steps to do and progress can be called
    //## No reference counting, cause of decentral static use!
    static ProgressBar* GetInstance();

    //##Documentation
    //## @brief Supply a GUI- dependent ProgressBar. Has to be set by the application
    //## to connect the application dependent subclass of mitkProgressBar
    static void SetImplementationInstance(ProgressBarImplementation* implementation);

    //##Documentation
    //## @brief Adds steps to totalSteps.
    void AddStepsToDo(int steps);
  
    //##Documentation
    //## @brief Sets the current amount of progress to current progress + steps.
    //## @param: steps the number of steps done since last Progress(int steps) call.
    void Progress(int steps = 1);

    //##Documentation
    //## @brief Sets whether the current progress value is displayed.
    void SetPercentageVisible (bool visible);

  protected:
  
    ProgressBar();
  
    virtual ~ProgressBar();
  
    static ProgressBarImplementation* m_Implementation;

    static ProgressBar* m_Instance;
  };

}// end namespace mitk

#endif /* define MITKPROGRESSBAR_H */
