//=================================================================================================
/*!
//  \file blaze/util/typetraits/AddCV.h
//  \brief Header file for the AddCV type trait
//
//  Copyright (C) 2011 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. This library is free software; you can redistribute
//  it and/or modify it under the terms of the GNU General Public License as published by the
//  Free Software Foundation; either version 3, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with a special
//  exception for linking and compiling against the Blaze library, the so-called "runtime
//  exception"; see the file COPYING. If not, see http://www.gnu.org/licenses/.
*/
//=================================================================================================

#ifndef _BLAZE_UTIL_TYPETRAITS_ADDCV_H_
#define _BLAZE_UTIL_TYPETRAITS_ADDCV_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <boost/type_traits/add_cv.hpp>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Addition of a top level 'const' and 'volatile' qualifier.
// \ingroup type_traits
//
// The AddCV type trait adds both a top level 'const' and 'volatile' qualifier to the given
// type \a T.

   \code
   blaze::AddCV<int>::Type           // Results in 'int const volatile'
   blaze::AddCV<int*>::Type          // Results in 'int* const volatile'
   blaze::AddCV<int&>::Type          // Results in 'int&'
   blaze::AddCV<int const>::Type     // Results in 'int const volatile'
   blaze::AddCV<int volatile>::Type  // Results in 'int const volatile'
   \endcode
*/
template< typename T >
struct AddCV
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename boost::add_cv<T>::type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************

} // namespace blaze

#endif
