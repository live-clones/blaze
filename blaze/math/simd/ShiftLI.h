//=================================================================================================
/*!
//  \file blaze/math/simd/ShiftLI.h
//  \brief Header file for the SIMD left-shift functionality
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

#ifndef _BLAZE_MATH_SIMD_SHIFTLI_H_
#define _BLAZE_MATH_SIMD_SHIFTLI_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/simd/BasicTypes.h>
#include <blaze/system/Inline.h>
#include <blaze/system/Vectorization.h>


namespace blaze {

//=================================================================================================
//
//  16-BIT INTEGRAL SIMD TYPES
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Uniform left-shift of a vector of 16-bit signed integral SIMD values.
// \ingroup simd
//
// \param a The vector of 16-bit signed integral values to be shifted.
// \param count The number of bits to shift \f$[0..15]\f$.
// \return The result of the left-shift.
//
// This operation is only available for SSE2, AVX2, and AVX-512.
*/
BLAZE_ALWAYS_INLINE const SIMDint16
   operator<<( const SIMDint16& a, int count ) noexcept
#if BLAZE_AVX512BW_MODE
{
   return _mm512_slli_epi16( a.value, static_cast<unsigned int>( count ) );
}
#elif BLAZE_AVX2_MODE
{
   return _mm256_slli_epi16( a.value, count );
}
#elif BLAZE_SSE2_MODE
{
   return _mm_slli_epi16( a.value, count );
}
#else
= delete;
#endif
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Uniform left-shift of a vector of 16-bit unsigned integral SIMD values.
// \ingroup simd
//
// \param a The vector of 16-bit unsigned integral values to be shifted.
// \param count The number of bits to shift \f$[0..15]\f$.
// \return The result of the left-shift.
//
// This operation is only available for SSE2, AVX2, and AVX-512.
*/
BLAZE_ALWAYS_INLINE const SIMDuint16
   operator<<( const SIMDuint16& a, int count ) noexcept
#if BLAZE_AVX512BW_MODE
{
   return _mm512_slli_epi16( a.value, static_cast<unsigned int>( count ) );
}
#elif BLAZE_AVX2_MODE
{
   return _mm256_slli_epi16( a.value, count );
}
#elif BLAZE_SSE2_MODE
{
   return _mm_slli_epi16( a.value, count );
}
#else
= delete;
#endif
//*************************************************************************************************




//=================================================================================================
//
//  32-BIT INTEGRAL SIMD TYPES
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Uniform left-shift of a vector of 32-bit signed integral SIMD values.
// \ingroup simd
//
// \param a The vector of 32-bit signed integral values to be shifted.
// \param count The number of bits to shift \f$[0..31]\f$.
// \return The result of the left-shift.
//
// This operation is only available for SSE2, AVX2, MIC, and AVX-512.
*/
BLAZE_ALWAYS_INLINE const SIMDint32
   operator<<( const SIMDint32& a, int count ) noexcept
#if BLAZE_AVX512F_MODE || BLAZE_MIC_MODE
{
   return _mm512_slli_epi32( a.value, static_cast<unsigned int>( count ) );
}
#elif BLAZE_AVX2_MODE
{
   return _mm256_slli_epi32( a.value, count );
}
#elif BLAZE_SSE2_MODE
{
   return _mm_slli_epi32( a.value, count );
}
#else
= delete;
#endif
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Uniform left-shift of a vector of 32-bit unsigned integral SIMD values.
// \ingroup simd
//
// \param a The vector of 32-bit unsigned integral values to be shifted.
// \param count The number of bits to shift \f$[0..31]\f$.
// \return The result of the left-shift.
//
// This operation is only available for SSE2, AVX2, MIC, and AVX-512.
*/
BLAZE_ALWAYS_INLINE const SIMDuint32
   operator<<( const SIMDuint32& a, int count ) noexcept
#if BLAZE_AVX512F_MODE || BLAZE_MIC_MODE
{
   return _mm512_slli_epi32( a.value, static_cast<unsigned int>( count ) );
}
#elif BLAZE_AVX2_MODE
{
   return _mm256_slli_epi32( a.value, count );
}
#elif BLAZE_SSE2_MODE
{
   return _mm_slli_epi32( a.value, count );
}
#else
= delete;
#endif
//*************************************************************************************************




//=================================================================================================
//
//  64-BIT INTEGRAL SIMD TYPES
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Uniform left-shift of a vector of 64-bit signed integral SIMD values.
// \ingroup simd
//
// \param a The vector of 64-bit signed integral values to be shifted.
// \param count The number of bits to shift \f$[0..63]\f$.
// \return The result of the left-shift.
//
// This operation is only available for SSE2, AVX2, and AVX-512.
*/
BLAZE_ALWAYS_INLINE const SIMDint64
   operator<<( const SIMDint64& a, int count ) noexcept
#if BLAZE_AVX512F_MODE
{
   return _mm512_slli_epi64( a.value, static_cast<unsigned int>( count ) );
}
#elif BLAZE_AVX2_MODE
{
   return _mm256_slli_epi64( a.value, count );
}
#elif BLAZE_SSE2_MODE
{
   return _mm_slli_epi64( a.value, count );
}
#else
= delete;
#endif
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Uniform left-shift of a vector of 64-bit unsigned integral SIMD values.
// \ingroup simd
//
// \param a The vector of 64-bit unsigned integral values to be shifted.
// \param count The number of bits to shift \f$[0..63]\f$.
// \return The result of the left-shift.
//
// This operation is only available for SSE2, AVX2, and AVX-512.
*/
BLAZE_ALWAYS_INLINE const SIMDuint64
   operator<<( const SIMDuint64& a, int count ) noexcept
#if BLAZE_AVX512F_MODE
{
   return _mm512_slli_epi64( a.value, static_cast<unsigned int>( count ) );
}
#elif BLAZE_AVX2_MODE
{
   return _mm256_slli_epi64( a.value, count );
}
#elif BLAZE_SSE2_MODE
{
   return _mm_slli_epi64( a.value, count );
}
#else
= delete;
#endif
//*************************************************************************************************

} // namespace blaze

#endif
