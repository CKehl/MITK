/*===================================================================

 The Medical Imaging Interaction Toolkit (MITK)

 Copyright (c) German Cancer Research Center,
 Division of Medical and Biological Informatics.
 All rights reserved.

 This software is distributed WITHOUT ANY WARRANTY; without
 even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.

 See LICENSE.txt or http://www.mitk.org for details.

 ===================================================================*/

#include "mitkException.h"
#include "mitkInteractionEvent.h"

mitk::InteractionEvent::InteractionEvent(BaseRenderer* baseRenderer, const std::string& eventClass)
: m_Sender(baseRenderer)
, m_EventClass(eventClass)
{
}

void mitk::InteractionEvent::SetSender(mitk::BaseRenderer* sender)
{
  m_Sender = sender;
}

mitk::BaseRenderer* mitk::InteractionEvent::GetSender() const
{
  return m_Sender;
}

bool mitk::InteractionEvent::IsEqual(const InteractionEvent&) const
{
  return true;
}

mitk::InteractionEvent::~InteractionEvent()
{
}

bool mitk::InteractionEvent::IsSuperClassOf(const InteractionEvent::Pointer& baseClass) const
{
  return (dynamic_cast<InteractionEvent*>(baseClass.GetPointer()) != NULL) ;
}

std::string mitk::InteractionEvent::GetEventClass() const
{
  return m_EventClass;
}

bool mitk::operator==(const InteractionEvent& a, const InteractionEvent& b)
{
  return (typeid(a) == typeid(b) && a.IsEqual(b));
}

bool mitk::operator!=(const InteractionEvent& a, const InteractionEvent& b)
{
  return !(a == b);
}
