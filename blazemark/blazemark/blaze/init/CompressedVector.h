//=================================================================================================
/*!
//  \file blazemark/blaze/init/CompressedVector.h
//  \brief Header file for the Blaze compressed vector initialization functions
//
//  Copyright (C) 2012-2020 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================

#ifndef _BLAZEMARK_BLAZE_INIT_COMPRESSEDVECTOR_H_
#define _BLAZEMARK_BLAZE_INIT_COMPRESSEDVECTOR_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <vector>
#include <blaze/math/CompressedVector.h>
#include <blaze/util/Indices.h>
#include <blaze/util/Random.h>
#include <blazemark/system/Types.h>


namespace blazemark {

namespace blaze {

//=================================================================================================
//
//  INITIALIZATION FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\name Blaze initialization functions */
//@{
template< typename Type, bool TF >
void init( ::blaze::CompressedVector<Type,TF>& v, size_t nonzeros );

template< typename Type, bool TF >
void init( ::std::vector< ::blaze::CompressedVector<Type,TF> >& v, size_t nonzeros );
//@}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Random initialization of the given compressed vector.
//
// \param v The compressed vector to be initialized.
// \param nonzeros The number of non-zero elements.
// \return void
//
// This function initializes the given compressed vector with random values. The vector will
// be filled with \a nonzeros non-zero elements, whose indices will be randomly determined.
*/
template< typename Type  // Data type of the vector
        , bool TF >      // Transpose flag
void init( ::blaze::CompressedVector<Type,TF>& v, size_t nonzeros )
{
   const size_t N( v.size() );

   ::blaze::Indices<size_t> indices( 0UL, N-1UL, nonzeros );
   for( auto it=indices.begin(); it!=indices.end(); ++it ) {
      v[*it] = ::blaze::rand<Type>( 0, 10 );
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Random initialization of the given vector of compressed vectors.
//
// \param v The vector of compressed vectors to be initialized.
// \param nonzeros The number of non-zero elements.
// \return void
//
// This function initializes all compressed vectors of the given std::vector with random values.
// All compressed vectors will be filled with \a nonzeros non-zero elements, whose indices will
// be randomly determined.
*/
template< typename Type  // Data type of the vector
        , bool TF >      // Transpose flag
void init( ::std::vector< ::blaze::CompressedVector<Type,TF> >& v, size_t nonzeros )
{
   const size_t size( v.size() );

   for( size_t i=0UL; i<size; ++i ) {
      init( v[i], nonzeros );
   }
}
//*************************************************************************************************

} // namespace blaze

} // namespace blazemark

#endif
