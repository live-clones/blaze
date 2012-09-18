//=================================================================================================
/*!
//  \file blaze/math/DynamicMatrix.h
//  \brief Header file for the complete DynamicMatrix implementation
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

#ifndef _BLAZE_MATH_DYNAMICMATRIX_H_
#define _BLAZE_MATH_DYNAMICMATRIX_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/dense/DynamicMatrix.h>
#include <blaze/math/dense/DynamicVector.h>
#include <blaze/system/Precision.h>


namespace blaze {

//=================================================================================================
//
//  TYPE DEFINITIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief MxN single precision matrix.
// \ingroup dynamic_matrix
*/
typedef DynamicMatrix<float,false>  MatMxNf;
//*************************************************************************************************


//*************************************************************************************************
/*!\brief MxN double precision matrix.
// \ingroup dynamic_matrix
*/
typedef DynamicMatrix<double,false>  MatMxNd;
//*************************************************************************************************


//*************************************************************************************************
/*!\brief MxN matrix with system-specific precision.
// \ingroup dynamic_matrix
*/
typedef DynamicMatrix<real,false>  MatMxN;
//*************************************************************************************************

} // namespace blaze

#endif
