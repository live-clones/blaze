//=================================================================================================
/*!
//  \file blaze/math/sparse/SparseMatrix.h
//  \brief Header file for utility functions for sparse matrices
//
//  Copyright (C) 2012-2018 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the foyllowing disclaimer.
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

#ifndef _BLAZE_MATH_SPARSE_SPARSEMATRIX_H_
#define _BLAZE_MATH_SPARSE_SPARSEMATRIX_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/Aliases.h>
#include <blaze/math/constraints/RequiresEvaluation.h>
#include <blaze/math/constraints/Triangular.h>
#include <blaze/math/constraints/UniTriangular.h>
#include <blaze/math/expressions/SparseMatrix.h>
#include <blaze/math/shims/Conjugate.h>
#include <blaze/math/shims/Equal.h>
#include <blaze/math/shims/IsDefault.h>
#include <blaze/math/shims/IsNaN.h>
#include <blaze/math/shims/IsOne.h>
#include <blaze/math/shims/IsReal.h>
#include <blaze/math/shims/IsZero.h>
#include <blaze/math/StorageOrder.h>
#include <blaze/math/traits/DivTrait.h>
#include <blaze/math/typetraits/IsExpression.h>
#include <blaze/math/typetraits/IsDiagonal.h>
#include <blaze/math/typetraits/IsHermitian.h>
#include <blaze/math/typetraits/IsIdentity.h>
#include <blaze/math/typetraits/IsInvertible.h>
#include <blaze/math/typetraits/IsLower.h>
#include <blaze/math/typetraits/IsResizable.h>
#include <blaze/math/typetraits/IsRestricted.h>
#include <blaze/math/typetraits/IsSquare.h>
#include <blaze/math/typetraits/IsStrictlyLower.h>
#include <blaze/math/typetraits/IsStrictlyUpper.h>
#include <blaze/math/typetraits/IsSymmetric.h>
#include <blaze/math/typetraits/IsTriangular.h>
#include <blaze/math/typetraits/IsUniform.h>
#include <blaze/math/typetraits/IsUniLower.h>
#include <blaze/math/typetraits/IsUniTriangular.h>
#include <blaze/math/typetraits/IsUniUpper.h>
#include <blaze/math/typetraits/IsUpper.h>
#include <blaze/math/typetraits/UnderlyingBuiltin.h>
#include <blaze/math/typetraits/UnderlyingNumeric.h>
#include <blaze/util/algorithms/Max.h>
#include <blaze/util/algorithms/Min.h>
#include <blaze/util/Assert.h>
#include <blaze/util/DecltypeAuto.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/FalseType.h>
#include <blaze/util/mpl/And.h>
#include <blaze/util/mpl/If.h>
#include <blaze/util/mpl/Or.h>
#include <blaze/util/TrueType.h>
#include <blaze/util/Types.h>
#include <blaze/util/typetraits/IsBuiltin.h>
#include <blaze/util/typetraits/IsComplex.h>
#include <blaze/util/typetraits/IsFloatingPoint.h>
#include <blaze/util/typetraits/IsNumeric.h>
#include <blaze/util/typetraits/RemoveReference.h>


namespace blaze {

//=================================================================================================
//
//  GLOBAL OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*!\name SparseMatrix operators */
//@{
template< typename T1, typename T2, bool SO >
inline bool operator==( const SparseMatrix<T1,false>& lhs, const SparseMatrix<T2,false>& rhs );

template< typename T1, typename T2, bool SO >
inline bool operator==( const SparseMatrix<T1,true>& lhs, const SparseMatrix<T2,true>& rhs );

template< typename T1, typename T2, bool SO >
inline bool operator==( const SparseMatrix<T1,SO>& lhs, const SparseMatrix<T2,!SO>& rhs );

template< typename T1, bool SO1, typename T2, bool SO2 >
inline bool operator!=( const SparseMatrix<T1,SO1>& lhs, const SparseMatrix<T2,SO2>& rhs );

template< typename MT, bool SO, typename ST >
inline EnableIf_< IsNumeric<ST>, MT& > operator*=( SparseMatrix<MT,SO>& mat, ST scalar );

template< typename MT, bool SO, typename ST >
inline EnableIf_< IsNumeric<ST>, MT& > operator*=( SparseMatrix<MT,SO>&& mat, ST scalar );

template< typename MT, bool SO, typename ST >
inline EnableIf_< IsNumeric<ST>, MT& > operator/=( SparseMatrix<MT,SO>& mat, ST scalar );

template< typename MT, bool SO, typename ST >
inline EnableIf_< IsNumeric<ST>, MT& > operator/=( SparseMatrix<MT,SO>&& mat, ST scalar );
//@}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Equality operator for the comparison of two row-major sparse matrices.
// \ingroup sparse_matrix
//
// \param lhs The left-hand side sparse matrix for the comparison.
// \param rhs The right-hand side sparse matrix for the comparison.
// \return \a true if the two sparse matrices are equal, \a false if not.
*/
template< typename T1    // Type of the left-hand side sparse matrix
        , typename T2 >  // Type of the right-hand side sparse matrix
inline bool operator==( const SparseMatrix<T1,false>& lhs, const SparseMatrix<T2,false>& rhs )
{
   using CT1 = CompositeType_t<T1>;
   using CT2 = CompositeType_t<T2>;
   using LhsConstIterator = ConstIterator_t< RemoveReference_t<CT1> >;
   using RhsConstIterator = ConstIterator_t< RemoveReference_t<CT2> >;

   // Early exit in case the matrix sizes don't match
   if( (~lhs).rows() != (~rhs).rows() || (~lhs).columns() != (~rhs).columns() )
      return false;

   // Evaluation of the two sparse matrix operands
   CT1 A( ~lhs );
   CT2 B( ~rhs );

   // In order to compare the two matrices, the data values of the lower-order data
   // type are converted to the higher-order data type within the equal function.
   for( size_t i=0UL; i<A.rows(); ++i )
   {
      const LhsConstIterator lend( A.end(i) );
      const RhsConstIterator rend( B.end(i) );

      LhsConstIterator lelem( A.begin(i) );
      RhsConstIterator relem( B.begin(i) );

      while( lelem != lend && relem != rend )
      {
         if( lelem->index() < relem->index() ) {
            if( !isDefault( lelem->value() ) )
               return false;
            ++lelem;
         }
         else if( lelem->index() > relem->index() ) {
            if( !isDefault( relem->value() ) )
               return false;
            ++relem;
         }
         else if( !equal( lelem->value(), relem->value() ) ) {
            return false;
         }
         else {
            ++lelem;
            ++relem;
         }
      }

      while( lelem != lend ) {
         if( !isDefault( lelem->value() ) )
            return false;
         ++lelem;
      }

      while( relem != rend ) {
         if( !isDefault( relem->value() ) )
            return false;
         ++relem;
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Equality operator for the comparison of two column-major sparse matrices.
// \ingroup sparse_matrix
//
// \param lhs The left-hand side sparse matrix for the comparison.
// \param rhs The right-hand side sparse matrix for the comparison.
// \return \a true if the two sparse matrices are equal, \a false if not.
*/
template< typename T1    // Type of the left-hand side sparse matrix
        , typename T2 >  // Type of the right-hand side sparse matrix
inline bool operator==( const SparseMatrix<T1,true>& lhs, const SparseMatrix<T2,true>& rhs )
{
   using CT1 = CompositeType_t<T1>;
   using CT2 = CompositeType_t<T2>;
   using LhsConstIterator = ConstIterator_t< RemoveReference_t<CT1> >;
   using RhsConstIterator = ConstIterator_t< RemoveReference_t<CT2> >;

   // Early exit in case the matrix sizes don't match
   if( (~lhs).rows() != (~rhs).rows() || (~lhs).columns() != (~rhs).columns() )
      return false;

   // Evaluation of the two sparse matrix operands
   CT1 A( ~lhs );
   CT2 B( ~rhs );

   // In order to compare the two matrices, the data values of the lower-order data
   // type are converted to the higher-order data type within the equal function.
   for( size_t j=0UL; j<A.columns(); ++j )
   {
      const LhsConstIterator lend( A.end(j) );
      const RhsConstIterator rend( B.end(j) );

      LhsConstIterator lelem( A.begin(j) );
      RhsConstIterator relem( B.begin(j) );

      while( lelem != lend && relem != rend )
      {
         if( lelem->index() < relem->index() ) {
            if( !isDefault( lelem->value() ) )
               return false;
            ++lelem;
         }
         else if( lelem->index() > relem->index() ) {
            if( !isDefault( relem->value() ) )
               return false;
            ++relem;
         }
         else if( !equal( lelem->value(), relem->value() ) ) {
            return false;
         }
         else {
            ++lelem;
            ++relem;
         }
      }

      while( lelem != lend ) {
         if( !isDefault( lelem->value() ) )
            return false;
         ++lelem;
      }

      while( relem != rend ) {
         if( !isDefault( relem->value() ) )
            return false;
         ++relem;
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Equality operator for the comparison of two sparse matrices with different storage order.
// \ingroup sparse_matrix
//
// \param lhs The left-hand side sparse matrix for the comparison.
// \param rhs The right-hand side sparse matrix for the comparison.
// \return \a true if the two sparse matrices are equal, \a false if not.
*/
template< typename T1  // Type of the left-hand side sparse matrix
        , typename T2  // Type of the right-hand side sparse matrix
        , bool SO >    // Storage order
inline bool operator==( const SparseMatrix<T1,SO>& lhs, const SparseMatrix<T2,!SO>& rhs )
{
   const OppositeType_t<T2> tmp( ~rhs );
   return ( ~lhs == tmp );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Inequality operator for the comparison of two sparse matrices.
// \ingroup sparse_matrix
//
// \param lhs The left-hand side sparse matrix for the comparison.
// \param rhs The right-hand side sparse matrix for the comparison.
// \return \a true if the two sparse matrices are not equal, \a false if they are equal.
*/
template< typename T1  // Type of the left-hand side sparse matrix
        , bool SO1     // Storage order of the left-hand side sparse matrix
        , typename T2  // Type of the right-hand side sparse matrix
        , bool SO2 >   // Storage order of the right-hand side sparse matrix
inline bool operator!=( const SparseMatrix<T1,SO1>& lhs, const SparseMatrix<T2,SO2>& rhs )
{
   return !( lhs == rhs );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Multiplication assignment operator for the multiplication of a sparse matrix and
//        a scalar value (\f$ A*=s \f$).
// \ingroup sparse_matrix
//
// \param mat The left-hand side sparse matrix for the multiplication.
// \param scalar The right-hand side scalar value for the multiplication.
// \return Reference to the left-hand side sparse matrix.
// \exception std::invalid_argument Invalid scaling of restricted matrix.
//
// In case the matrix \a MT is restricted and the assignment would violate an invariant of the
// matrix, a \a std::invalid_argument exception is thrown.
*/
template< typename MT    // Type of the left-hand side sparse matrix
        , bool SO        // Storage order
        , typename ST >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<ST>, MT& > operator*=( SparseMatrix<MT,SO>& mat, ST scalar )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   if( IsRestricted_v<MT> ) {
      if( !tryMult( ~mat, 0UL, 0UL, (~mat).rows(), (~mat).columns(), scalar ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid scaling of restricted matrix" );
      }
   }

   if( !IsResizable_v< ElementType_t<MT> > && isZero( scalar ) )
   {
      reset( ~mat );
   }
   else
   {
      BLAZE_DECLTYPE_AUTO( left, derestrict( ~mat ) );

      const size_t iend( SO == rowMajor ? (~mat).rows() : (~mat).columns() );
      for( size_t i=0UL; i<iend; ++i ) {
         const auto last( left.end(i) );
         for( auto element=left.begin(i); element!=last; ++element ) {
            element->value() *= scalar;
         }
      }
   }

   BLAZE_INTERNAL_ASSERT( isIntact( ~mat ), "Invariant violation detected" );

   return ~mat;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Multiplication assignment operator for the multiplication of a temporary sparse matrix
//        and a scalar value (\f$ A*=s \f$).
// \ingroup sparse_matrix
//
// \param mat The left-hand side temporary sparse matrix for the multiplication.
// \param scalar The right-hand side scalar value for the multiplication.
// \return Reference to the left-hand side sparse matrix.
// \exception std::invalid_argument Invalid scaling of restricted matrix.
//
// In case the matrix \a MT is restricted and the assignment would violate an invariant of the
// matrix, a \a std::invalid_argument exception is thrown.
*/
template< typename MT    // Type of the left-hand side sparse matrix
        , bool SO        // Storage order
        , typename ST >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<ST>, MT& > operator*=( SparseMatrix<MT,SO>&& mat, ST scalar )
{
   return operator*=( ~mat, scalar );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Division assignment operator for the division of a sparse matrix by a scalar value
//        (\f$ A/=s \f$).
// \ingroup sparse_matrix
//
// \param mat The left-hand side sparse matrix for the division.
// \param scalar The right-hand side scalar value for the division.
// \return Reference to the left-hand side sparse matrix.
// \exception std::invalid_argument Invalid scaling of restricted matrix.
//
// In case the matrix \a MT is restricted and the assignment would violate an invariant of the
// matrix, a \a std::invalid_argument exception is thrown.
//
// \note A division by zero is only checked by an user assert.
*/
template< typename MT    // Type of the left-hand side sparse matrix
        , bool SO        // Storage order
        , typename ST >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<ST>, MT& > operator/=( SparseMatrix<MT,SO>& mat, ST scalar )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_UNITRIANGULAR_MATRIX_TYPE( MT );

   BLAZE_USER_ASSERT( !isZero( scalar ), "Division by zero detected" );

   if( IsRestricted_v<MT> ) {
      if( !tryDiv( ~mat, 0UL, 0UL, (~mat).rows(), (~mat).columns(), scalar ) ) {
         BLAZE_THROW_INVALID_ARGUMENT( "Invalid scaling of restricted matrix" );
      }
   }

   using ScalarType = If_< Or< IsFloatingPoint< UnderlyingBuiltin_t<MT> >
                             , IsFloatingPoint< UnderlyingBuiltin_t<ST> > >
                         , If_< And< IsComplex< UnderlyingNumeric_t<MT> >
                                   , IsBuiltin<ST> >
                              , DivTrait_t< UnderlyingBuiltin_t<MT>, ST >
                              , DivTrait_t< UnderlyingNumeric_t<MT>, ST > >
                         , ST >;

   BLAZE_DECLTYPE_AUTO( left, derestrict( ~mat ) );

   if( IsInvertible_v<ScalarType> ) {
      const ScalarType tmp( ScalarType(1)/static_cast<ScalarType>( scalar ) );
      const size_t iend( SO == rowMajor ? (~mat).rows() : (~mat).columns() );
      for( size_t i=0UL; i<iend; ++i ) {
         const auto last( left.end(i) );
         for( auto element=left.begin(i); element!=last; ++element ) {
            element->value() *= tmp;
         }
      }
   }
   else {
      const size_t iend( SO == rowMajor ? (~mat).rows() : (~mat).columns() );
      for( size_t i=0UL; i<iend; ++i ) {
         const auto last( left.end(i) );
         for( auto element=left.begin(i); element!=last; ++element ) {
            element->value() /= scalar;
         }
      }
   }

   BLAZE_INTERNAL_ASSERT( isIntact( ~mat ), "Invariant violation detected" );

   return ~mat;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Division assignment operator for the division of a temporary sparse matrix by a scalar
//        value (\f$ A/=s \f$).
// \ingroup sparse_matrix
//
// \param mat The left-hand side temporary sparse matrix for the division.
// \param scalar The right-hand side scalar value for the division.
// \return Reference to the left-hand side sparse matrix.
// \exception std::invalid_argument Invalid scaling of restricted matrix.
//
// In case the matrix \a MT is restricted and the assignment would violate an invariant of the
// matrix, a \a std::invalid_argument exception is thrown.
//
// \note A division by zero is only checked by an user assert.
*/
template< typename MT    // Type of the left-hand side sparse matrix
        , bool SO        // Storage order
        , typename ST >  // Data type of the right-hand side scalar
inline EnableIf_< IsNumeric<ST>, MT& > operator/=( SparseMatrix<MT,SO>&& mat, ST scalar )
{
   return operator/=( ~mat, scalar );
}
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\name SparseMatrix functions */
//@{
template< typename MT, bool SO >
bool isnan( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isSymmetric( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isHermitian( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isUniform( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isLower( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isUniLower( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isStrictlyLower( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isUpper( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isUniUpper( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isStrictlyUpper( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isDiagonal( const SparseMatrix<MT,SO>& sm );

template< bool RF, typename MT, bool SO >
bool isIdentity( const SparseMatrix<MT,SO>& sm );

template< typename MT, bool SO >
const ElementType_t<MT> min( const SparseMatrix<MT,SO>& sm );

template< typename MT, bool SO >
const ElementType_t<MT> max( const SparseMatrix<MT,SO>& sm );
//@}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks the given sparse matrix for not-a-number elements.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked for not-a-number elements.
// \return \a true if at least one element of the sparse matrix is not-a-number, \a false otherwise.
//
// This function checks the sparse matrix for not-a-number (NaN) elements. If at least one
// element of the matrix is not-a-number, the function returns \a true, otherwise it returns
// \a false.

   \code
   blaze::CompressedMatrix<double> A( 3UL, 4UL );
   // ... Initialization
   if( isnan( A ) ) { ... }
   \endcode

// Note that this function only works for matrices with floating point elements. The attempt to
// use it for a matrix with a non-floating point element type results in a compile time error.
*/
template< typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isnan( const SparseMatrix<MT,SO>& sm )
{
   using CT = CompositeType_t<MT>;
   using ConstIterator = ConstIterator_t< RemoveReference_t<CT> >;

   CT A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows(); ++i ) {
         for( ConstIterator element=A.begin(i); element!=A.end(i); ++element )
            if( isnan( element->value() ) ) return true;
      }
   }
   else {
      for( size_t j=0UL; j<A.columns(); ++j ) {
         for( ConstIterator element=A.begin(j); element!=A.end(j); ++element )
            if( isnan( element->value() ) ) return true;
      }
   }

   return false;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the given sparse matrix is symmetric.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is symmetric, \a false if not.
//
// This function checks if the given sparse matrix is symmetric. The matrix is considered to be
// symmetric if it is a square matrix whose transpose is equal to itself (\f$ A = A^T \f$). The
// following code example demonstrates the use of the function:

   \code
   blaze::CompressedMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isSymmetric( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isSymmetric<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in a symmetric matrix:

   \code
   if( isSymmetric( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isSymmetric( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsSymmetric_v<MT> )
      return true;

   if( !isSquare( ~sm ) )
      return false;

   if( IsUniform_v<MT> || (~sm).rows() < 2UL )
      return true;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows(); ++i ) {
         for( ConstIterator element=A.begin(i); element!=A.end(i); ++element )
         {
            const size_t j( element->index() );

            if( i == j || isDefault<RF>( element->value() ) )
               continue;

            const ConstIterator pos( A.find( j, i ) );
            if( pos == A.end(j) || !equal<RF>( pos->value(), element->value() ) )
               return false;
         }
      }
   }
   else {
      for( size_t j=0UL; j<A.columns(); ++j ) {
         for( ConstIterator element=A.begin(j); element!=A.end(j); ++element )
         {
            const size_t i( element->index() );

            if( j == i || isDefault<RF>( element->value() ) )
               continue;

            const ConstIterator pos( A.find( j, i ) );
            if( pos == A.end(i) || !equal<RF>( pos->value(), element->value() ) )
               return false;
         }
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the given sparse matrix is Hermitian.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is Hermitian, \a false if not.
//
// This function checks if the given sparse matrix is an Hermitian matrix. The matrix is considered
// to be an Hermitian matrix if it is a square matrix whose conjugate transpose is equal to itself
// (\f$ A = \overline{A^T} \f$), i.e. each matrix element \f$ a_{ij} \f$ is equal to the complex
// conjugate of the element \f$ a_{ji} \f$. The following code example demonstrates the use of the
// function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isHermitian( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isHermitian<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in an Hermitian matrix:

   \code
   if( isHermitian( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isHermitian( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using ET  = ElementType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsHermitian_v<MT> )
      return true;

   if( !IsNumeric_v<ET> || !isSquare( ~sm ) )
      return false;

   if( IsBuiltin_v<ET> && IsUniform_v<MT> )
      return true;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows(); ++i ) {
         for( ConstIterator element=A.begin(i); element!=A.end(i); ++element )
         {
            const size_t j( element->index() );

            if( isDefault<RF>( element->value() ) )
               continue;

            if( i == j && !isReal<RF>( element->value() ) )
               return false;

            const ConstIterator pos( A.find( j, i ) );
            if( pos == A.end(j) || !equal<RF>( pos->value(), conj( element->value() ) ) )
               return false;
         }
      }
   }
   else {
      for( size_t j=0UL; j<A.columns(); ++j ) {
         for( ConstIterator element=A.begin(j); element!=A.end(j); ++element )
         {
            const size_t i( element->index() );

            if( isDefault<RF>( element->value() ) )
               continue;

            if( j == i && !isReal<RF>( element->value() ) )
               return false;

            const ConstIterator pos( A.find( j, i ) );
            if( pos == A.end(i) || !equal<RF>( pos->value(), conj( element->value() ) ) )
               return false;
         }
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given row-major triangular sparse matrix is a uniform matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is a uniform matrix, \a false if not.
*/
template< bool RF        // Relaxation flag
        , typename MT >  // Type of the sparse matrix
bool isUniform_backend( const SparseMatrix<MT,false>& sm, TrueType )
{
   BLAZE_CONSTRAINT_MUST_BE_TRIANGULAR_MATRIX_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( MT );

   BLAZE_INTERNAL_ASSERT( (~sm).rows()    != 0UL, "Invalid number of rows detected"    );
   BLAZE_INTERNAL_ASSERT( (~sm).columns() != 0UL, "Invalid number of columns detected" );

   using ConstIterator = ConstIterator_t<MT>;

   const size_t ibegin( ( IsStrictlyLower_v<MT> )?( 1UL ):( 0UL ) );
   const size_t iend  ( ( IsStrictlyUpper_v<MT> )?( (~sm).rows()-1UL ):( (~sm).rows() ) );

   for( size_t i=ibegin; i<iend; ++i ) {
      for( ConstIterator element=(~sm).begin(i); element!=(~sm).end(i); ++element ) {
         if( !isDefault<RF>( element->value() ) )
            return false;
      }
   }

   return true;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given column-major triangular sparse matrix is a uniform matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is a uniform matrix, \a false if not.
*/
template< bool RF        // Relaxation flag
        , typename MT >  // Type of the sparse matrix
bool isUniform_backend( const SparseMatrix<MT,true>& sm, TrueType )
{
   BLAZE_CONSTRAINT_MUST_BE_TRIANGULAR_MATRIX_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( MT );

   BLAZE_INTERNAL_ASSERT( (~sm).rows()    != 0UL, "Invalid number of rows detected"    );
   BLAZE_INTERNAL_ASSERT( (~sm).columns() != 0UL, "Invalid number of columns detected" );

   using ConstIterator = ConstIterator_t<MT>;

   const size_t jbegin( ( IsStrictlyUpper_v<MT> )?( 1UL ):( 0UL ) );
   const size_t jend  ( ( IsStrictlyLower_v<MT> )?( (~sm).columns()-1UL ):( (~sm).columns() ) );

   for( size_t j=jbegin; j<jend; ++j ) {
      for( ConstIterator element=(~sm).begin(j); element!=(~sm).end(j); ++element ) {
         if( !isDefault<RF>( element->value() ) )
            return false;
      }
   }

   return true;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given row-major general sparse matrix is a uniform matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is a uniform matrix, \a false if not.
*/
template< bool RF        // Relaxation flag
        , typename MT >  // Type of the sparse matrix
bool isUniform_backend( const SparseMatrix<MT,false>& sm, FalseType )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_TRIANGULAR_MATRIX_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( MT );

   BLAZE_INTERNAL_ASSERT( (~sm).rows()    != 0UL, "Invalid number of rows detected"    );
   BLAZE_INTERNAL_ASSERT( (~sm).columns() != 0UL, "Invalid number of columns detected" );

   using ConstReference = ConstReference_t<MT>;
   using ConstIterator  = ConstIterator_t<MT>;

   const size_t maxElements( (~sm).rows() * (~sm).columns() );

   if( (~sm).nonZeros() != maxElements )
   {
      for( size_t i=0UL; i<(~sm).rows(); ++i ) {
         for( ConstIterator element=(~sm).begin(i); element!=(~sm).end(i); ++element ) {
            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }
   else
   {
      BLAZE_INTERNAL_ASSERT( (~sm).find(0UL,0UL) != (~sm).end(0UL), "Missing element detected" );

      ConstReference cmp( (~sm)(0UL,0UL) );

      for( size_t i=0UL; i<(~sm).rows(); ++i ) {
         for( ConstIterator element=(~sm).begin(i); element!=(~sm).end(i); ++element ) {
            if( element->value() != cmp )
               return false;
         }
      }
   }

   return true;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Checks if the given column-major general sparse matrix is a uniform matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is a uniform matrix, \a false if not.
*/
template< bool RF        // Relaxation flag
        , typename MT >  // Type of the sparse matrix
bool isUniform_backend( const SparseMatrix<MT,true>& sm, FalseType )
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_TRIANGULAR_MATRIX_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION( MT );

   BLAZE_INTERNAL_ASSERT( (~sm).rows()    != 0UL, "Invalid number of rows detected"    );
   BLAZE_INTERNAL_ASSERT( (~sm).columns() != 0UL, "Invalid number of columns detected" );

   using ConstReference = ConstReference_t<MT>;
   using ConstIterator  = ConstIterator_t<MT>;

   const size_t maxElements( (~sm).rows() * (~sm).columns() );

   if( (~sm).nonZeros() != maxElements )
   {
      for( size_t j=0UL; j<(~sm).columns(); ++j ) {
         for( ConstIterator element=(~sm).begin(j); element!=(~sm).end(j); ++element ) {
            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }
   else
   {
      BLAZE_INTERNAL_ASSERT( (~sm).find(0UL,0UL) != (~sm).end(0UL), "Missing element detected" );

      ConstReference cmp( (~sm)(0UL,0UL) );

      for( size_t j=0UL; j<(~sm).columns(); ++j ) {
         for( ConstIterator element=(~sm).begin(j); element!=(~sm).end(j); ++element ) {
            if( element->value() != cmp )
               return false;
         }
      }
   }

   return true;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the given sparse matrix is a uniform matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is a uniform matrix, \a false if not.
//
// This function checks if the given sparse matrix is a uniform matrix. The matrix is considered
// to be uniform if all its elements are identical. The following code example demonstrates the
// use of the function:

   \code
   blaze::DynamicMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isUniform( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isUniform<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in a uniform matrix:

   \code
   if( isUniform( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isUniform( const SparseMatrix<MT,SO>& sm )
{
   if( IsUniTriangular_v<MT> )
      return false;

   if( IsUniform_v<MT> ||
       (~sm).rows() == 0UL || (~sm).columns() == 0UL ||
       ( (~sm).rows() == 1UL && (~sm).columns() == 1UL ) )
      return true;

   CompositeType_t<MT> A( ~sm );  // Evaluation of the sparse matrix operand

   return isUniform_backend<RF>( A, typename IsTriangular<MT>::Type() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the given sparse matrix is a lower triangular matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is a lower triangular matrix, \a false if not.
//
// This function checks if the given sparse matrix is a lower triangular matrix. The matrix is
// considered to be lower triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        l_{0,0} & 0       & 0       & \cdots & 0       \\
                        l_{1,0} & l_{1,1} & 0       & \cdots & 0       \\
                        l_{2,0} & l_{2,1} & l_{2,2} & \cdots & 0       \\
                        \vdots  & \vdots  & \vdots  & \ddots & \vdots  \\
                        l_{N,0} & l_{N,1} & l_{N,2} & \cdots & l_{N,N} \\
                        \end{array}\right).\f]

// \f$ 0 \times 0 \f$ or \f$ 1 \times 1 \f$ matrices are considered as trivially lower triangular.
// The following code example demonstrates the use of the function:

   \code
   blaze::CompressedMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isLower( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isLower<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in a lower triangular matrix:

   \code
   if( isLower( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isLower( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsLower_v<MT> )
      return true;

   if( !isSquare( ~sm ) )
      return false;

   if( (~sm).rows() < 2UL )
      return true;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows()-1UL; ++i ) {
         for( ConstIterator element=A.lowerBound(i,i+1UL); element!=A.end(i); ++element )
         {
            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }
   else {
      for( size_t j=1UL; j<A.columns(); ++j ) {
         for( ConstIterator element=A.begin(j); element!=A.end(j); ++element )
         {
            if( element->index() >= j )
               break;

            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the given sparse matrix is a lower unitriangular matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is a lower unitriangular matrix, \a false if not.
//
// This function checks if the given sparse matrix is a lower unitriangular matrix. The matrix is
// considered to be lower unitriangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        1       & 0       & 0       & \cdots & 0      \\
                        l_{1,0} & 1       & 0       & \cdots & 0      \\
                        l_{2,0} & l_{2,1} & 1       & \cdots & 0      \\
                        \vdots  & \vdots  & \vdots  & \ddots & \vdots \\
                        l_{N,0} & l_{N,1} & l_{N,2} & \cdots & 1      \\
                        \end{array}\right).\f]

// The following code example demonstrates the use of the function:

   \code
   blaze::CompressedMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isUniLower( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isUniLower<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in a lower unitriangular matrix:

   \code
   if( isUniLower( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isUniLower( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsUniLower_v<MT> )
      return true;

   if( !isSquare( ~sm ) )
      return false;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows(); ++i )
      {
         ConstIterator element( A.lowerBound(i,i) );

         if( element == A.end(i) || element->index() != i || !isOne<RF>( element->value() ) )
            return false;

         ++element;

         for( ; element!=A.end(i); ++element ) {
            if( !isZero<RF>( element->value() ) )
               return false;
         }
      }
   }
   else {
      for( size_t j=0UL; j<A.columns(); ++j )
      {
         bool hasDiagonalElement( false );

         for( ConstIterator element=A.begin(j); element!=A.end(j); ++element )
         {
            if( element->index() >= j ) {
               if( element->index() != j || !isOne<RF>( element->value() ) )
                  return false;
               hasDiagonalElement = true;
               break;
            }

            if( !isZero<RF>( element->value() ) )
               return false;
         }

         if( !hasDiagonalElement ) {
            return false;
         }
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the given sparse matrix is a strictly lower triangular matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is a strictly lower triangular matrix, \a false if not.
//
// This function checks if the given sparse matrix is a strictly lower triangular matrix. The
// matrix is considered to be strictly lower triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        0       & 0       & 0       & \cdots & 0      \\
                        l_{1,0} & 0       & 0       & \cdots & 0      \\
                        l_{2,0} & l_{2,1} & 0       & \cdots & 0      \\
                        \vdots  & \vdots  & \vdots  & \ddots & \vdots \\
                        l_{N,0} & l_{N,1} & l_{N,2} & \cdots & 0      \\
                        \end{array}\right).\f]

// The following code example demonstrates the use of the function:

   \code
   blaze::CompressedMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isStrictlyLower( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isStrictlyLower<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in a strictly lower triangular
// matrix:

   \code
   if( isStrictlyLower( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isStrictlyLower( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsStrictlyLower_v<MT> )
      return true;

   if( IsUniLower_v<MT> || IsUniUpper_v<MT> || !isSquare( ~sm ) )
      return false;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows(); ++i ) {
         for( ConstIterator element=A.lowerBound(i,i); element!=A.end(i); ++element )
         {
            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }
   else {
      for( size_t j=0UL; j<A.columns(); ++j ) {
         for( ConstIterator element=A.begin(j); element!=A.end(j); ++element )
         {
            if( element->index() > j )
               break;

            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the given sparse matrix is an upper triangular matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is an upper triangular matrix, \a false if not.
//
// This function checks if the given sparse matrix is an upper triangular matrix. The matrix is
// considered to be upper triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        u_{0,0} & u_{0,1} & u_{0,2} & \cdots & u_{0,N} \\
                        0       & u_{1,1} & u_{1,2} & \cdots & u_{1,N} \\
                        0       & 0       & u_{2,2} & \cdots & u_{2,N} \\
                        \vdots  & \vdots  & \vdots  & \ddots & \vdots  \\
                        0       & 0       & 0       & \cdots & u_{N,N} \\
                        \end{array}\right).\f]

// \f$ 0 \times 0 \f$ or \f$ 1 \times 1 \f$ matrices are considered as trivially upper triangular.
// The following code example demonstrates the use of the function:

   \code
   blaze::CompressedMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isUpper( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isUpper<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in an upper triangular matrix:

   \code
   if( isUpper( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isUpper( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsUpper_v<MT> )
      return true;

   if( !isSquare( ~sm ) )
      return false;

   if( (~sm).rows() < 2UL )
      return true;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=1UL; i<A.rows(); ++i ) {
         for( ConstIterator element=A.begin(i); element!=A.end(i); ++element )
         {
            if( element->index() >= i )
               break;

            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }
   else {
      for( size_t j=0UL; j<A.columns()-1UL; ++j ) {
         for( ConstIterator element=A.lowerBound(j+1UL,j); element!=A.end(j); ++element )
         {
            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the given sparse matrix is an upper unitriangular matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is an upper unitriangular matrix, \a false if not.
//
// This function checks if the given sparse matrix is an upper unitriangular matrix. The matrix is
// considered to be upper unitriangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        1      & u_{0,1} & u_{0,2} & \cdots & u_{0,N} \\
                        0      & 1       & u_{1,2} & \cdots & u_{1,N} \\
                        0      & 0       & 1       & \cdots & u_{2,N} \\
                        \vdots & \vdots  & \vdots  & \ddots & \vdots  \\
                        0      & 0       & 0       & \cdots & 1       \\
                        \end{array}\right).\f]

// The following code example demonstrates the use of the function:

   \code
   blaze::CompressedMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isUniUpper( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isUniUpper<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in an upper unitriangular matrix:

   \code
   if( isUniUpper( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isUniUpper( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsUniUpper_v<MT> )
      return true;

   if( !isSquare( ~sm ) )
      return false;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows(); ++i )
      {
         bool hasDiagonalElement( false );

         for( ConstIterator element=A.begin(i); element!=A.end(i); ++element )
         {
            if( element->index() >= i ) {
               if( element->index() != i || !isOne<RF>( element->value() ) )
                  return false;
               hasDiagonalElement = true;
               break;
            }
            else if( !isZero<RF>( element->value() ) ) {
               return false;
            }
         }

         if( !hasDiagonalElement ) {
            return false;
         }
      }
   }
   else {
      for( size_t j=0UL; j<A.columns(); ++j )
      {
         ConstIterator element( A.lowerBound(j,j) );

         if( element == A.end(j) || element->index() != j || !isOne<RF>( element->value() ) )
            return false;

         ++element;

         for( ; element!=A.end(j); ++element ) {
            if( !isZero<RF>( element->value() ) )
               return false;
         }
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the given sparse matrix is a strictly upper triangular matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is a strictly upper triangular matrix, \a false if not.
//
// This function checks if the given sparse matrix is a strictly upper triangular matrix. The
// matrix is considered to be strictly upper triangular if it is a square matrix of the form

                        \f[\left(\begin{array}{*{5}{c}}
                        0      & u_{0,1} & u_{0,2} & \cdots & u_{0,N} \\
                        0      & 0       & u_{1,2} & \cdots & u_{1,N} \\
                        0      & 0       & 0       & \cdots & u_{2,N} \\
                        \vdots & \vdots  & \vdots  & \ddots & \vdots  \\
                        0      & 0       & 0       & \cdots & 0       \\
                        \end{array}\right).\f]

// The following code example demonstrates the use of the function:

   \code
   blaze::CompressedMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isStrictlyUpper( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isStrictlyUpper<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in a strictly upper triangular
// matrix:

   \code
   if( isStrictlyUpper( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isStrictlyUpper( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsStrictlyUpper_v<MT> )
      return true;

   if( IsUniLower_v<MT> || IsUniUpper_v<MT> || !isSquare( ~sm ) )
      return false;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows(); ++i ) {
         for( ConstIterator element=A.begin(i); element!=A.end(i); ++element )
         {
            if( element->index() > i )
               break;

            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }
   else {
      for( size_t j=0UL; j<A.columns(); ++j ) {
         for( ConstIterator element=A.lowerBound(j,j); element!=A.end(j); ++element )
         {
            if( !isDefault<RF>( element->value() ) )
               return false;
         }
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the give sparse matrix is diagonal.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is diagonal, \a false if not.
//
// This function tests whether the matrix is diagonal, i.e. if the non-diagonal elements are
// default elements. In case of integral or floating point data types, a diagonal matrix has
// the form

                        \f[\left(\begin{array}{*{5}{c}}
                        aa     & 0      & 0      & \cdots & 0  \\
                        0      & bb     & 0      & \cdots & 0  \\
                        0      & 0      & cc     & \cdots & 0  \\
                        \vdots & \vdots & \vdots & \ddots & 0  \\
                        0      & 0      & 0      & 0      & xx \\
                        \end{array}\right)\f]

// The following example demonstrates the use of the function:

   \code
   blaze::CompressedMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isDiagonal( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isDiagonal<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in a diagonal matrix:

   \code
   if( isDiagonal( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isDiagonal( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsDiagonal_v<MT> )
      return true;

   if( !isSquare( ~sm ) )
      return false;

   if( (~sm).rows() < 2UL )
      return true;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows(); ++i ) {
         for( ConstIterator element=A.begin(i); element!=A.end(i); ++element )
            if( element->index() != i && !isDefault<RF>( element->value() ) )
               return false;
      }
   }
   else {
      for( size_t j=0UL; j<A.columns(); ++j ) {
         for( ConstIterator element=A.begin(j); element!=A.end(j); ++element )
            if( element->index() != j && !isDefault<RF>( element->value() ) )
               return false;
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks if the give sparse matrix is an identity matrix.
// \ingroup sparse_matrix
//
// \param sm The sparse matrix to be checked.
// \return \a true if the matrix is an identity matrix, \a false if not.
//
// This function tests whether the matrix is an identity matrix, i.e. if the diagonal elements
// are 1 and the non-diagonal elements are 0. In case of integral or floating point data types,
// an identity matrix has the form

                        \f[\left(\begin{array}{*{5}{c}}
                        1      & 0      & 0      & \cdots & 0 \\
                        0      & 1      & 0      & \cdots & 0 \\
                        0      & 0      & 1      & \cdots & 0 \\
                        \vdots & \vdots & \vdots & \ddots & 0 \\
                        0      & 0      & 0      & 0      & 1 \\
                        \end{array}\right)\f]

// The following example demonstrates the use of the function:

   \code
   blaze::CompressedMatrix<int,blaze::rowMajor> A, B;
   // ... Initialization
   if( isIdentity( A ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isIdentity<relaxed>( A ) ) { ... }
   \endcode

// It is also possible to check if a matrix expression results in an identity matrix:

   \code
   if( isIdentity( A * B ) ) { ... }
   \endcode

// However, note that this might require the complete evaluation of the expression, including
// the generation of a temporary matrix.
*/
template< bool RF      // Relaxation flag
        , typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
bool isIdentity( const SparseMatrix<MT,SO>& sm )
{
   using RT  = ResultType_t<MT>;
   using RN  = ReturnType_t<MT>;
   using CT  = CompositeType_t<MT>;
   using Tmp = If_< IsExpression<RN>, const RT, CT >;
   using ConstIterator = ConstIterator_t< RemoveReference_t<Tmp> >;

   if( IsIdentity_v<MT> )
      return true;

   if( !isSquare( ~sm ) )
      return false;

   Tmp A( ~sm );  // Evaluation of the sparse matrix operand

   if( SO == rowMajor ) {
      for( size_t i=0UL; i<A.rows(); ++i )
      {
         bool hasDiagonalElement( false );

         for( ConstIterator element=A.begin(i); element!=A.end(i); ++element )
         {
            if( element->index() == i ) {
               if( !isOne<RF>( element->value() ) )
                  return false;
               hasDiagonalElement = true;
            }
            else if( !isZero<RF>( element->value() ) ) {
               return false;
            }
         }

         if( !hasDiagonalElement ) {
            return false;
         }
      }
   }
   else {
      for( size_t j=0UL; j<A.columns(); ++j )
      {
         bool hasDiagonalElement( false );

         for( ConstIterator element=A.begin(j); element!=A.end(j); ++element )
         {
            if( element->index() == j ) {
               if( !isOne<RF>( element->value() ) )
                  return false;
               hasDiagonalElement = true;
            }
            else if( !isZero<RF>( element->value() ) ) {
               return false;
            }
         }

         if( !hasDiagonalElement ) {
            return false;
         }
      }
   }

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns the smallest element of the sparse matrix.
// \ingroup sparse_matrix
//
// \param sm The given sparse matrix.
// \return The smallest sparse matrix element.
//
// This function returns the smallest element of the given sparse matrix. This function can
// only be used for element types that support the smaller-than relationship. In case the
// matrix currently has either 0 rows or 0 columns, the returned value is the default value
// (e.g. 0 in case of fundamental data types).
*/
template< typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
const ElementType_t<MT> min( const SparseMatrix<MT,SO>& sm )
{
   using blaze::min;

   using ET = ElementType_t<MT>;
   using CT = CompositeType_t<MT>;
   using ConstIterator = ConstIterator_t< RemoveReference_t<CT> >;

   CT A( ~sm );  // Evaluation of the sparse matrix operand

   const size_t nonzeros( A.nonZeros() );

   if( nonzeros == 0UL ) {
      return ET();
   }

   ET minimum{};
   if( nonzeros == A.rows() * A.columns() ) {
      minimum = A.begin( 0UL )->value();
   }

   const size_t index( ( SO == rowMajor )?( A.rows() ):( A.columns() ) );

   for( size_t i=0UL; i<index; ++i ) {
      const ConstIterator end( A.end( i ) );
      ConstIterator element( A.begin( i ) );
      for( ; element!=end; ++element )
         minimum = min( minimum, element->value() );
   }

   return minimum;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns the largest element of the sparse matrix.
// \ingroup sparse_matrix
//
// \param sm The given sparse matrix.
// \return The largest sparse matrix element.
//
// This function returns the largest element of the given sparse matrix. This function can
// only be used for element types that support the smaller-than relationship. In case the
// matrix currently has either 0 rows or 0 columns, the returned value is the default value
// (e.g. 0 in case of fundamental data types).
*/
template< typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
const ElementType_t<MT> max( const SparseMatrix<MT,SO>& sm )
{
   using blaze::max;

   using ET = ElementType_t<MT>;
   using CT = CompositeType_t<MT>;
   using ConstIterator = ConstIterator_t< RemoveReference_t<CT> >;

   CT A( ~sm );  // Evaluation of the sparse matrix operand

   const size_t nonzeros( A.nonZeros() );

   if( nonzeros == 0UL ) {
      return ET();
   }

   ET maximum{};
   if( nonzeros == A.rows() * A.columns() ) {
      maximum = A.begin( 0UL )->value();
   }

   const size_t index( ( SO == rowMajor )?( A.rows() ):( A.columns() ) );

   for( size_t i=0UL; i<index; ++i ) {
      const ConstIterator end( A.end( i ) );
      ConstIterator element( A.begin( i ) );
      for( ; element!=end; ++element )
         maximum = max( maximum, element->value() );
   }

   return maximum;
}
//*************************************************************************************************

} // namespace blaze

#endif
