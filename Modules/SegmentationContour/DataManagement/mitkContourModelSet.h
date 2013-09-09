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
#ifndef _mitkContourModelSet_H_
#define _mitkContourModelSet_H_

#include "mitkCommon.h"
#include "SegmentationContourExports.h"

#include "mitkContourModel.h"

#include <deque>

namespace mitk
{

  /** \brief
  */
  class SegmentationContour_EXPORT ContourModelSet : public itk::LightObject
  {

  public:

    mitkClassMacro(ContourModelSet, itk::LightObject);

    itkNewMacro(Self);

    mitkCloneMacro(Self);

    typedef std::deque<mitk::ContourModel::Pointer> ContourModelListType;
    typedef ContourModelListType::iterator ContourModelIterator;

    //  start of inline methods

    /** \brief Return an iterator a the front.
    */
    virtual ContourModelIterator Begin()
    {
      return this->m_Contours.begin();
    }

    /** \brief Returns the number of contained contours.
    */
    virtual int GetSize()
    {
      return this->m_Contours.size();
    }
    //   end of inline methods

    /** \brief Add a ContourModel to the container.
    */
    virtual void AddContourModel(mitk::ContourModel &ContourModel);

    /** \brief Returns the ContourModel a given index
    \param index
    */
    virtual mitk::ContourModel* GetContourModelAt(int index);

    /** \brief Returns the container of the contours.
    */
    ContourModelListType* GetContourModelList();

    /** \brief Returns a bool whether the container is empty or not.
    */
    bool IsEmpty();

    /** \brief Remove the given ContourModel from the container if exists.
    \param ContourModel - the ContourModel to be removed.
    */
    virtual bool RemoveContourModel(mitk::ContourModel* ContourModel);

    /** \brief Remove a ContourModel at given index within the container if exists.
    \param index - the index where the ContourModel should be removed.
    */
    virtual bool RemoveContourModelAt(int index);

    /** \brief Clear the storage container.
    */
    virtual void Clear();

  protected:

    ContourModelSet();
    ContourModelSet(const mitk::ContourModelSet &other);
    virtual ~ContourModelSet();

    ContourModelListType m_Contours;

  };
} // namespace mitk

#endif // _mitkContourModelSet_H_
