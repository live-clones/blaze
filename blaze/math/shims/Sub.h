//=================================================================================================
/*!
//  \file blaze/math/shims/Sub.h
//  \brief Header file for the sub shim
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

#ifndef _BLAZE_MATH_SHIMS_SUB_H_
#define _BLAZE_MATH_SHIMS_SUB_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <utility>
#include <blaze/system/Inline.h>


namespace blaze {

//=================================================================================================
//
//  SUB SHIM
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Subtraction of the two given arguments.
// \ingroup math_shims
//
// \param lhs The left-hand side operand for the subtraction.
// \param rhs The right-hand side operand for the subtraction.
// \return The difference of the two operands.
//
// The \a sub() shim represents an abstract interface for the subtraction of two operands.
*/
template< typename T1, typename T2 >
BLAZE_ALWAYS_INLINE constexpr decltype(auto) sub( T1&& lhs, T2&& rhs )
   noexcept( noexcept( lhs - rhs ) )
{
   return std::forward<T1>( lhs ) - std::forward<T2>( rhs );
}
//*************************************************************************************************

} // namespace blaze

#endif
