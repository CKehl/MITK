/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
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


#include "mitkBoundingObjectGroup.h"
#include "mitkBaseProcess.h"
#include <vtkLinearTransform.h>
#include <itkSmartPointerForwardReference.txx>


mitk::BoundingObjectGroup::BoundingObjectGroup()
:  m_Counter(0),
m_CSGMode(Union),// m_CSGMode(Difference) //m_CSGMode(Intersection)
m_BoundingObjects(0)
{
  GetTimeSlicedGeometry()->Initialize(1);
  GetGeometry(0)->SetIndexToWorldTransform(GetTimeSlicedGeometry()->GetIndexToWorldTransform());
  SetVtkPolyData(NULL);
}

mitk::BoundingObjectGroup::~BoundingObjectGroup()
{
} 

void mitk::BoundingObjectGroup::UpdateOutputInformation()
{  
  if ( this->GetSource() )
  {
    this->GetSource()->UpdateOutputInformation();
  }

  // calculate global bounding box
  if(m_BoundingObjects.size()  < 1 ) // if there is no BoundingObject, the bounding box is zero
  {
    mitk::BoundingBox::BoundsArrayType boundsArray;
    boundsArray.Fill(0);
    GetTimeSlicedGeometry()->Initialize(1);
    GetGeometry()->SetBounds(boundsArray);
    GetTimeSlicedGeometry()->UpdateInformation();
    return; 
  }

  // initialize container
  mitk::BoundingBox::PointsContainer::Pointer pointscontainer=mitk::BoundingBox::PointsContainer::New();

  mitk::BoundingBox::PointIdentifier pointid=0;
  mitk::Point3D point;

  mitk::AffineTransform3D* transform = GetTimeSlicedGeometry()->GetIndexToWorldTransform();
  mitk::AffineTransform3D::Pointer inverse = mitk::AffineTransform3D::New();
  transform->GetInverse(inverse);

  // calculate a bounding box that includes all BoundingObjects
  // \todo probably we should do this additionally for each time-step
  //while (boundingObjectsIterator != boundingObjectsIteratorEnd)
  for(int j = 0; j<m_BoundingObjects.size();j++)
  {
    const Geometry3D* geometry = m_BoundingObjects.at(j)->GetUpdatedTimeSlicedGeometry();
    unsigned char i;
    for(i=0; i<8; ++i)
    {
      point = inverse->TransformPoint(geometry->GetCornerPoint(i));
      if(point[0]*point[0]+point[1]*point[1]+point[2]*point[2] < mitk::large)
        pointscontainer->InsertElement( pointid++, point);
      else
      {
        itkGenericOutputMacro( << "Unrealistically distant corner point encountered. Ignored. BoundingObject: " << m_BoundingObjects.at(j) );
      }
    }
  }

  mitk::BoundingBox::Pointer boundingBox = mitk::BoundingBox::New();
  boundingBox->SetPoints(pointscontainer);
  boundingBox->ComputeBoundingBox();

  /* BoundingBox is centered around the center of all sub bounding objects */
  //Point3D center = boundingBox->GetCenter();

  //Point3D minimum, maximum;
  //minimum.Fill(0); maximum.Fill(0);
  //minimum += boundingBox->GetMinimum() - center;
  //maximum += boundingBox->GetMaximum() - center;

  //boundingBox->SetMinimum(minimum);
  //boundingBox->SetMaximum(maximum);

  Geometry3D* geometry3d = GetGeometry(0);
  geometry3d->SetIndexToWorldTransform(transform);
  geometry3d->SetBounds(boundingBox->GetBounds());
  /* the objects position is the center of all sub bounding objects */
  //geometry3d->SetOrigin(center);

  GetTimeSlicedGeometry()->InitializeEvenlyTimed(geometry3d, GetTimeSlicedGeometry()->GetTimeSteps());
}

void mitk::BoundingObjectGroup::AddBoundingObject(mitk::BoundingObject::Pointer boundingObject)
{
  if (boundingObject->GetPositive())
    m_BoundingObjects.push_front(boundingObject);
  else
    m_BoundingObjects.push_back(boundingObject);
  ++m_Counter;
  UpdateOutputInformation();
}

void mitk::BoundingObjectGroup::RemoveBoundingObject(mitk::BoundingObject::Pointer boundingObject)
{
  std::deque<mitk::BoundingObject::Pointer>::iterator it = m_BoundingObjects.begin();
  for (int i=0 ; i<m_BoundingObjects.size();i++)
  {
    if (m_BoundingObjects.at(i) == boundingObject)
      m_BoundingObjects.erase(it);
    ++it;
  }
  --m_Counter;
  UpdateOutputInformation();
}

bool mitk::BoundingObjectGroup::IsInside(const mitk::Point3D& p) const
{  
  bool inside; // initialize with true for intersection, with false for union
  bool posInside;
  bool negInside;

  for (int i = 0; i<m_BoundingObjects.size();i++)
  {
    switch(m_CSGMode)
    {
    case Intersection:
      inside = true;
      // calculate intersection: each point, that is inside each BoundingObject is considered inside the group
      inside = m_BoundingObjects.at(i)->IsInside(p) && inside;
      if (!inside) // shortcut, it is enough to find one object that does not contain the point
        i=m_BoundingObjects.size();
      break;

    case Union:
    case Difference:
      posInside = false;
      negInside = false;
      // calculate union: each point, that is inside least one BoundingObject is considered inside the group        
      if (m_BoundingObjects.at(i)->GetPositive())
        posInside = m_BoundingObjects.at(i)->IsInside(p) || posInside;
      else
        negInside = m_BoundingObjects.at(i)->IsInside(p) || negInside;

      if (posInside && !negInside)
        inside = true;
      else
        inside = false;
      break;

    default:
      inside = false;
      // calculate union: each point, that is inside least one BoundingObject is considered inside the group
      inside = m_BoundingObjects.at(i)->IsInside(p) || inside;  
      if (inside) // shortcut, it is enough to find one object that contains the point
        i=m_BoundingObjects.size();
      break;
    }
  }
  return inside;
}

unsigned int mitk::BoundingObjectGroup::GetCount() const
{
  return m_Counter;
}

bool mitk::BoundingObjectGroup::VerifyRequestedRegion()
{
  return m_Counter > 0;
}

mitk::Geometry3D *  mitk::BoundingObjectGroup::GetGeometry (int t) const
{
  //if ( m_BoundingObjects == NULL )
  return Superclass::GetGeometry(t);

  //mitk::BoundingObjectGroup::BoundingObjectContainer::ConstIterator boI = m_BoundingObjects->Begin();
  //const mitk::BoundingObjectGroup::BoundingObjectContainer::ConstIterator boIEnd = m_BoundingObjects->End();
  //mitk::Geometry3D* currentGeometry = NULL;

  //while ( boI != boIEnd )
  //{
  //  currentGeometry = boI.Value()->GetGeometry( t );
  //  boI++;
  //}

  //return currentGeometry;
}

void mitk::BoundingObjectGroup::SetBoundingObjects(const std::deque<mitk::BoundingObject::Pointer> boundingObjects)
{
  m_BoundingObjects = boundingObjects;
}

std::deque<mitk::BoundingObject::Pointer> mitk::BoundingObjectGroup::GetBoundingObjects()
{
  return m_BoundingObjects;
}
