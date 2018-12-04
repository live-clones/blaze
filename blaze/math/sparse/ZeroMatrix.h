//=================================================================================================
/*!
//  \file blaze/math/sparse/ZeroMatrix.h
//  \brief Implementation of a zero matrix
//
//  Copyright (C) 2012-2018 Klaus Iglberger - All Rights Reserved
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

#ifndef _BLAZE_MATH_SPARSE_ZEROMATRIX_H_
#define _BLAZE_MATH_SPARSE_ZEROMATRIX_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/Aliases.h>
#include <blaze/math/Exception.h>
#include <blaze/math/expressions/SparseMatrix.h>
#include <blaze/math/Forward.h>
#include <blaze/math/sparse/ValueIndexPair.h>
#include <blaze/math/traits/AddTrait.h>
#include <blaze/math/traits/DivTrait.h>
#include <blaze/math/traits/MapTrait.h>
#include <blaze/math/traits/MultTrait.h>
#include <blaze/math/traits/SchurTrait.h>
#include <blaze/math/traits/SubTrait.h>
#include <blaze/math/typetraits/HighType.h>
#include <blaze/math/typetraits/IsIdentity.h>
#include <blaze/math/typetraits/IsMatrix.h>
#include <blaze/math/typetraits/IsResizable.h>
#include <blaze/math/typetraits/IsSMPAssignable.h>
#include <blaze/math/typetraits/IsSparseMatrix.h>
#include <blaze/math/typetraits/IsUniform.h>
#include <blaze/math/typetraits/IsZero.h>
#include <blaze/math/typetraits/LowType.h>
#include <blaze/math/typetraits/StorageOrder.h>
#include <blaze/math/typetraits/YieldsZero.h>
#include <blaze/system/StorageOrder.h>
#include <blaze/util/Assert.h>
#include <blaze/util/constraints/Const.h>
#include <blaze/util/constraints/Pointer.h>
#include <blaze/util/constraints/Reference.h>
#include <blaze/util/constraints/Volatile.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/FunctionTrace.h>
#include <blaze/util/mpl/If.h>
#include <blaze/util/TrueType.h>
#include <blaze/util/Types.h>
#include <blaze/util/Unused.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\defgroup zero_matrix ZeroMatrix
// \ingroup sparse_matrix
*/
/*!\brief Efficient implementation of an \f$ M \times N \f$ zero matrix.
// \ingroup zero_matrix
//
// The ZeroMatrix class template is the representation of an immutable, arbitrary sized zero
// matrix with \f$ M \cdot N \f$ elements of arbitrary type. The type of the elements and the
// storage order of the matrix can be specified via the two template parameters:

   \code
   template< typename Type, bool SO >
   class ZeroMatrix;
   \endcode

//  - Type: specifies the type of the matrix elements. ZeroMatrix can be used with any
//          non-cv-qualified, non-reference, non-pointer element type.
//  - SO  : specifies the storage order (blaze::rowMajor, blaze::columnMajor) of the matrix.
//          The default value is blaze::rowMajor.
//
// It is not possible to insert, erase or modify the elements of a zero matrix. It is only
// possible to read from the elements:

   \code
   using blaze::rowMajor;

   // Creating a row-major 4x6 zero matrix with 4 rows and 6 columns
   ZeroMatrix<double,rowMajor> A( 4, 6 );

   // The function call operator provides access to all possible elements of the zero matrix,
   // including the zero elements.
   A(1,2) = 2.0;       // Compilation error: It is not possible to write to a zero matrix
   double d = A(2,1);  // Access to the element (2,1)

   // In order to traverse all non-zero elements currently stored in the matrix, the begin()
   // and end() functions can be used. In the example, all non-zero elements of the 2nd row
   // of A are traversed.
   for( ZeroMatrix<double,rowMajor>::Iterator i=A.begin(1); i!=A.end(1); ++i ) {
      ... = i->value();  // Access to the value of the non-zero element
      ... = i->index();  // Access to the index of the non-zero element
   }
   \endcode

// The use of ZeroMatrix is very natural and intuitive. All operations (addition, subtraction,
// multiplication, ...) can be performed on all possible combinations of row-major and column-major
// dense and sparse matrices with fitting element types. The following example gives an impression
// of the use of ZeroMatrix:

   \code
   using blaze::ZeroMatrix;
   using blaze::CompressedMatrix;
   using blaze::DynamicMatrix;
   using blaze::rowMajor;
   using blaze::columnMajor;

   ZeroMatrix<double,rowMajor> A( 3, 3 );  // Row-major 3x3 zero matrix

   DynamicMatrix<double,columnMajor> B( 3, 3 );  // Column-major 3x3 dynamic dense matrix
   CompressedMatrix<double,rowMajor> C( 3, 3 );  // Row-major 3x3 compressed sparse matrix
   CompressedMatrix<double,rowMajor> D( 3, 5 );  // Row-major 3x5 compressed sparse matrix
   // ... Initialization of B, C, and D

   DynamicMatrix<double,rowMajor>       E( A );  // Creation of a new row-major matrix as a copy of A
   CompressedMatrix<double,columnMajor> F;       // Creation of a default column-major matrix

   E = A + B;    // Addition of a zero matrix and a dense matrix
   E = C - A;    // Subtraction of a sparse matrix and a zero matrix
   F = A * D;    // Matrix multiplication between two matrices of different element types

   E = 2.0 * A;  // Scaling of a zero matrix
   F = A * 2.0;  // Scaling of a zero matrix
   \endcode
*/
template< typename Type                    // Data type of the matrix
        , bool SO = defaultStorageOrder >  // Storage order
class ZeroMatrix
   : public SparseMatrix< ZeroMatrix<Type,SO>, SO >
{
 private:
   //**Type definitions****************************************************************************
   using Element = ValueIndexPair<Type>;  //!< Value-index-pair for the ZeroMatrix class.
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   using This           = ZeroMatrix<Type,SO>;    //!< Type of this ZeroMatrix instance.
   using BaseType       = SparseMatrix<This,SO>;  //!< Base type of this ZeroMatrix instance.
   using ResultType     = This;                   //!< Result type for expression template evaluations.
   using OppositeType   = ZeroMatrix<Type,!SO>;   //!< Result type with opposite storage order for expression template evaluations.
   using TransposeType  = ZeroMatrix<Type,!SO>;   //!< Transpose type for expression template evaluations.
   using ElementType    = Type;                   //!< Type of the zero matrix elements.
   using ReturnType     = const Type&;            //!< Return type for expression template evaluations.
   using CompositeType  = const This&;            //!< Data type for composite expression templates.
   using Reference      = const Type&;            //!< Reference to a zero matrix element.
   using ConstReference = const Type&;            //!< Reference to a constant zero matrix element.
   using Iterator       = Element*;               //!< Iterator over non-constant elements.
   using ConstIterator  = const Element*;         //!< Iterator over constant elements.
   //**********************************************************************************************

   //**Rebind struct definition********************************************************************
   /*!\brief Rebind mechanism to obtain a ZeroMatrix with different data/element type.
   */
   template< typename NewType >  // Data type of the other matrix
   struct Rebind {
      using Other = ZeroMatrix<NewType,SO>;  //!< The type of the other ZeroMatrix.
   };
   //**********************************************************************************************

   //**Resize struct definition********************************************************************
   /*!\brief Resize mechanism to obtain a ZeroMatrix with different fixed dimensions.
   */
   template< size_t NewM    // Number of rows of the other matrix
           , size_t NewN >  // Number of columns of the other matrix
   struct Resize {
      using Other = ZeroMatrix<Type,SO>;  //!< The type of the other ZeroMatrix.
   };
   //**********************************************************************************************

   //**Compilation flags***************************************************************************
   //! Compilation flag for SMP assignments.
   /*! The \a smpAssignable compilation flag indicates whether the matrix can be used in SMP
       (shared memory parallel) assignments (both on the left-hand and right-hand side of the
       assignment). */
   static constexpr bool smpAssignable = !IsSMPAssignable_v<Type>;
   //**********************************************************************************************

   //**Constructors********************************************************************************
   /*!\name Constructors */
   //@{
   explicit inline constexpr ZeroMatrix() noexcept;
   explicit inline constexpr ZeroMatrix( size_t m, size_t n ) noexcept;

   template< typename MT, bool SO2 >
   explicit inline ZeroMatrix( const Matrix<MT,SO2>& m );

   ZeroMatrix( const ZeroMatrix& ) = default;
   ZeroMatrix( ZeroMatrix&& ) = default;
   //@}
   //**********************************************************************************************

   //**Destructor**********************************************************************************
   /*!\name Destructor */
   //@{
   ~ZeroMatrix() = default;
   //@}
   //**********************************************************************************************

   //**Data access functions***********************************************************************
   /*!\name Data access functions */
   //@{
   inline constexpr ConstReference operator()( size_t i, size_t j ) const noexcept;
   inline           ConstReference at( size_t i, size_t j ) const;
   inline constexpr ConstIterator  begin ( size_t i ) const noexcept;
   inline constexpr ConstIterator  cbegin( size_t i ) const noexcept;
   inline constexpr ConstIterator  end   ( size_t i ) const noexcept;
   inline constexpr ConstIterator  cend  ( size_t i ) const noexcept;
   //@}
   //**********************************************************************************************

   //**Assignment operators************************************************************************
   /*!\name Assignment operators */
   //@{
   template< typename MT, bool SO2 >
   inline ZeroMatrix& operator=( const Matrix<MT,SO2>& rhs );

   ZeroMatrix& operator=( const ZeroMatrix& ) = default;
   ZeroMatrix& operator=( ZeroMatrix&& ) = default;
   //@}
   //**********************************************************************************************

   //**Utility functions***************************************************************************
   /*!\name Utility functions */
   //@{
   inline constexpr size_t rows() const noexcept;
   inline constexpr size_t columns() const noexcept;
   inline constexpr size_t capacity() const noexcept;
   inline constexpr size_t capacity( size_t i ) const noexcept;
   inline constexpr size_t nonZeros() const noexcept;
   inline constexpr size_t nonZeros( size_t i ) const noexcept;
   inline constexpr void   clear() noexcept;
          constexpr void   resize( size_t m, size_t n ) noexcept;
   inline constexpr void   swap( ZeroMatrix& m ) noexcept;
   //@}
   //**********************************************************************************************

   //**Lookup functions****************************************************************************
   /*!\name Lookup functions */
   //@{
   inline ConstIterator find      ( size_t i, size_t j ) const;
   inline ConstIterator lowerBound( size_t i, size_t j ) const;
   inline ConstIterator upperBound( size_t i, size_t j ) const;
   //@}
   //**********************************************************************************************

   //**Numeric functions***************************************************************************
   /*!\name Numeric functions */
   //@{
   inline constexpr ZeroMatrix& transpose() noexcept;
   inline constexpr ZeroMatrix& ctranspose() noexcept;
   //@}
   //**********************************************************************************************

   //**Expression template evaluation functions****************************************************
   /*!\name Expression template evaluation functions */
   //@{
   template< typename Other > inline bool canAlias ( const Other* alias ) const noexcept;
   template< typename Other > inline bool isAliased( const Other* alias ) const noexcept;

   inline bool canSMPAssign() const noexcept;
   //@}
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   /*!\name Member variables */
   //@{
   size_t m_;  //!< The current number of rows of the zero matrix.
   size_t n_;  //!< The current number of columns of the zero matrix.

   static const Type zero_;  //!< The zero element.
   //@}
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   /*! \cond BLAZE_INTERNAL */
   BLAZE_CONSTRAINT_MUST_NOT_BE_POINTER_TYPE  ( Type );
   BLAZE_CONSTRAINT_MUST_NOT_BE_REFERENCE_TYPE( Type );
   BLAZE_CONSTRAINT_MUST_NOT_BE_CONST         ( Type );
   BLAZE_CONSTRAINT_MUST_NOT_BE_VOLATILE      ( Type );
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************




//=================================================================================================
//
//  DEFINITION AND INITIALIZATION OF THE STATIC MEMBER VARIABLES
//
//=================================================================================================

template< typename Type, bool SO >
const Type ZeroMatrix<Type,SO>::zero_{};




//=================================================================================================
//
//  CONSTRUCTORS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief The default constructor for ZeroMatrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr ZeroMatrix<Type,SO>::ZeroMatrix() noexcept
   : m_( 0UL )  // The current number of rows of the zero matrix
   , n_( 0UL )  // The current number of columns of the zero matrix
{}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Constructor for a zero matrix of size \f$ M \times N \f$.
//
// \param m The number of rows of the matrix.
// \param n The number of columns of the matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr ZeroMatrix<Type,SO>::ZeroMatrix( size_t m, size_t n ) noexcept
   : m_( m )  // The current number of rows of the zero matrix
   , n_( n )  // The current number of columns of the zero matrix
{}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Conversion constructor for different zero matrices.
//
// \param m Zero matrix to be copied.
// \exception std::invalid_argument Invalid setup of zero matrix.
//
// The matrix is sized according to the given \f$ M \times N \f$ zero matrix and initialized
// as a copy of this matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
template< typename MT    // Type of the foreign zero matrix
        , bool SO2 >     // Storage order of the foreign zero matrix
inline ZeroMatrix<Type,SO>::ZeroMatrix( const Matrix<MT,SO2>& m )
   : m_( (~m).rows()    )  // The current number of rows of the zero matrix
   , n_( (~m).columns() )  // The current number of columns of the zero matrix
{
   if( !IsZero_v<MT> && !isZero( ~m ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid setup of zero matrix" );
   }
}
//*************************************************************************************************




//=================================================================================================
//
//  DATA ACCESS FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief 2D-access to the zero matrix elements.
//
// \param i Access index for the row. The index has to be in the range \f$[0..M-1]\f$.
// \param j Access index for the column. The index has to be in the range \f$[0..N-1]\f$.
// \return Reference to the accessed value.
//
// This function only performs an index check in case BLAZE_USER_ASSERT() is active. In contrast,
// the at() function is guaranteed to perform a check of the given access indices.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr typename ZeroMatrix<Type,SO>::ConstReference
   ZeroMatrix<Type,SO>::operator()( size_t i, size_t j ) const noexcept
{
   BLAZE_USER_ASSERT( i < rows()   , "Invalid zero matrix row access index"    );
   BLAZE_USER_ASSERT( j < columns(), "Invalid zero matrix column access index" );

   return zero_;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checked access to the matrix elements.
//
// \param i Access index for the row. The index has to be in the range \f$[0..M-1]\f$.
// \param j Access index for the column. The index has to be in the range \f$[0..N-1]\f$.
// \return Reference to the accessed value.
// \exception std::out_of_range Invalid matrix access index.
//
// In contrast to the subscript operator this function always performs a check of the given
// access indices.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline typename ZeroMatrix<Type,SO>::ConstReference
   ZeroMatrix<Type,SO>::at( size_t i, size_t j ) const
{
   if( i >= m_ ) {
      BLAZE_THROW_OUT_OF_RANGE( "Invalid row access index" );
   }
   if( j >= n_ ) {
      BLAZE_THROW_OUT_OF_RANGE( "Invalid column access index" );
   }

   return zero_;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator to the first non-zero element of row/column \a i.
//
// \param i The row/column index.
// \return Iterator to the first non-zero element of row/column \a i.
//
// This function returns a row/column iterator to the first non-zero element of row/column \a i.
// In case the storage order is set to \a rowMajor the function returns an iterator to the first
// non-zero element of row \a i, in case the storage flag is set to \a columnMajor the function
// returns an iterator to the first non-zero element of column \a i.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr typename ZeroMatrix<Type,SO>::ConstIterator
   ZeroMatrix<Type,SO>::begin( size_t i ) const noexcept
{
   BLAZE_USER_ASSERT( i < ( SO ? n_ : m_ ), "Invalid zero matrix row/column access index" );

   return nullptr;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator to the first non-zero element of row/column \a i.
//
// \param i The row/column index.
// \return Iterator to the first non-zero element of row/column \a i.
//
// This function returns a row/column iterator to the first non-zero element of row/column \a i.
// In case the storage order is set to \a rowMajor the function returns an iterator to the first
// non-zero element of row \a i, in case the storage flag is set to \a columnMajor the function
// returns an iterator to the first non-zero element of column \a i.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr typename ZeroMatrix<Type,SO>::ConstIterator
   ZeroMatrix<Type,SO>::cbegin( size_t i ) const noexcept
{
   BLAZE_USER_ASSERT( i < ( SO ? n_ : m_ ), "Invalid zero matrix row/column access index" );

   return nullptr;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator just past the last non-zero element of row/column \a i.
//
// \param i The row/column index.
// \return Iterator just past the last non-zero element of row/column \a i.
//
// This function returns an row/column iterator just past the last non-zero element of row/column
// \a i. In case the storage order is set to \a rowMajor the function returns an iterator just
// past the last non-zero element of row \a i, in case the storage flag is set to \a columnMajor
// the function returns an iterator just past the last non-zero element of column \a i.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr typename ZeroMatrix<Type,SO>::ConstIterator
   ZeroMatrix<Type,SO>::end( size_t i ) const noexcept
{
   BLAZE_USER_ASSERT( i < ( SO ? n_ : m_ ), "Invalid zero matrix row/column access index" );

   return nullptr;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator just past the last non-zero element of row/column \a i.
//
// \param i The row/column index.
// \return Iterator just past the last non-zero element of row/column \a i.
//
// This function returns an row/column iterator just past the last non-zero element of row/column
// \a i. In case the storage order is set to \a rowMajor the function returns an iterator just
// past the last non-zero element of row \a i, in case the storage flag is set to \a columnMajor
// the function returns an iterator just past the last non-zero element of column \a i.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr typename ZeroMatrix<Type,SO>::ConstIterator
   ZeroMatrix<Type,SO>::cend( size_t i ) const noexcept
{
   BLAZE_USER_ASSERT( i < ( SO ? n_ : m_ ), "Invalid zero matrix row/column access index" );

   return nullptr;
}
//*************************************************************************************************




//=================================================================================================
//
//  ASSIGNMENT OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Assignment operator for different zero matrices.
//
// \param rhs Zero matrix to be copied.
// \return Reference to the assigned matrix.
// \exception std::invalid_argument Invalid assignment to zero matrix.
//
// The matrix is resized according to the given \f$ M \times N \f$ zero matrix and initialized
// as a copy of this matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
template< typename MT    // Type of the right-hand side zero matrix
        , bool SO2 >     // Storage order of the right-hand side zero matrix
inline ZeroMatrix<Type,SO>&
   ZeroMatrix<Type,SO>::operator=( const Matrix<MT,SO2>& rhs )
{
   if( !IsZero_v<MT> && !isZero( ~rhs ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid assignment of zero matrix" );
   }

   const size_t m( (~rhs).rows() );
   const size_t n( (~rhs).columns() );

   m_ = m;
   n_ = n;

   return *this;
}
//*************************************************************************************************




//=================================================================================================
//
//  UTILITY FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Returns the current number of rows of the zero matrix.
//
// \return The number of rows of the zero matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr size_t ZeroMatrix<Type,SO>::rows() const noexcept
{
   return m_;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns the current number of columns of the zero matrix.
//
// \return The number of columns of the zero matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr size_t ZeroMatrix<Type,SO>::columns() const noexcept
{
   return n_;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns the maximum capacity of the zero matrix.
//
// \return The capacity of the zero matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr size_t ZeroMatrix<Type,SO>::capacity() const noexcept
{
   return 0UL;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns the current capacity of the specified row/column.
//
// \param i The index of the row/column.
// \return The current capacity of row/column \a i.
//
// This function returns the current capacity of the specified row/column. In case the
// storage order is set to \a rowMajor the function returns the capacity of row \a i,
// in case the storage flag is set to \a columnMajor the function returns the capacity
// of column \a i.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr size_t ZeroMatrix<Type,SO>::capacity( size_t i ) const noexcept
{
   UNUSED_PARAMETER( i );

   BLAZE_USER_ASSERT( i < ( SO ? n_ : m_ ), "Invalid zero matrix row/column access index" );

   return 0UL;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns the number of non-zero elements in the zero matrix
//
// \return The number of non-zero elements in the zero matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr size_t ZeroMatrix<Type,SO>::nonZeros() const noexcept
{
   return 0UL;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns the number of non-zero elements in the specified row/column.
//
// \param i The index of the row/column.
// \return The number of non-zero elements of row/column \a i.
//
// This function returns the current number of non-zero elements in the specified row/column.
// In case the storage order is set to \a rowMajor the function returns the number of non-zero
// elements in row \a i, in case the storage flag is set to \a columnMajor the function returns
// the number of non-zero elements in column \a i.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr size_t ZeroMatrix<Type,SO>::nonZeros( size_t i ) const noexcept
{
   UNUSED_PARAMETER( i );

   BLAZE_USER_ASSERT( i < ( SO ? n_ : m_ ), "Invalid zero matrix row/column access index" );

   return 0UL;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Clearing the zero matrix.
//
// \return void
//
// After the clear() function, the size of the zero matrix is 0.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr void ZeroMatrix<Type,SO>::clear() noexcept
{
   m_ = 0UL;
   n_ = 0UL;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Changing the size of the zero matrix.
//
// \param m The new number of rows of the zero matrix.
// \param n The new number of columns of the zero matrix.
// \return void
//
// This function resizes the matrix using the given size to \f$ m \times n \f$. Note that this
// function may invalidate all existing views (submatrices, rows, columns, ...) on the matrix if
// it is used to shrink the matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
void constexpr ZeroMatrix<Type,SO>::resize( size_t m, size_t n ) noexcept
{
   m_ = m;
   n_ = n;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Swapping the contents of two zero matrices.
//
// \param m The zero matrix to be swapped.
// \return void
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr void ZeroMatrix<Type,SO>::swap( ZeroMatrix& m ) noexcept
{
   const size_t tmp1( m_ );
   m_ = m.m_;
   m.m_ = tmp1;

   const size_t tmp2( n_ );
   n_ = m.n_;
   m.n_ = tmp2;
}
//*************************************************************************************************




//=================================================================================================
//
//  LOOKUP FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Searches for a specific matrix element.
//
// \param i The row index of the search element. The index has to be in the range \f$[0..M-1]\f$.
// \param j The column index of the search element. The index has to be in the range \f$[0..N-1]\f$.
// \return Iterator to the element in case the index is found, end() iterator otherwise.
//
// This function can be used to check whether a specific element is contained in the sparse
// matrix. It specifically searches for the element with row index \a i and column index \a j.
// In case the element is found, the function returns an row/column iterator to the element.
// Otherwise an iterator just past the last non-zero element of row \a i or column \a j (the
// end() iterator) is returned.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline typename ZeroMatrix<Type,SO>::ConstIterator
   ZeroMatrix<Type,SO>::find( size_t i, size_t j ) const
{
   BLAZE_USER_ASSERT( SO  || i < rows()   , "Invalid zero matrix row access index"    );
   BLAZE_USER_ASSERT( !SO || j < columns(), "Invalid zero matrix column access index" );

   return nullptr;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator to the first index not less then the given index.
//
// \param i The row index of the search element. The index has to be in the range \f$[0..M-1]\f$.
// \param j The column index of the search element. The index has to be in the range \f$[0..N-1]\f$.
// \return Iterator to the first index not less then the given index, end() iterator otherwise.
//
// In case of a row-major matrix, this function returns a row iterator to the first element with
// an index not less then the given column index. In case of a column-major matrix, the function
// returns a column iterator to the first element with an index not less then the given row
// index. In combination with the upperBound() function this function can be used to create a
// pair of iterators specifying a range of indices.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline typename ZeroMatrix<Type,SO>::ConstIterator
   ZeroMatrix<Type,SO>::lowerBound( size_t i, size_t j ) const
{
   BLAZE_USER_ASSERT( SO  || i < rows()   , "Invalid zero matrix row access index"    );
   BLAZE_USER_ASSERT( !SO || j < columns(), "Invalid zero matrix column access index" );

   return nullptr;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator to the first index greater then the given index.
//
// \param i The row index of the search element. The index has to be in the range \f$[0..M-1]\f$.
// \param j The column index of the search element. The index has to be in the range \f$[0..N-1]\f$.
// \return Iterator to the first index greater then the given index, end() iterator otherwise.
//
// In case of a row-major matrix, this function returns a row iterator to the first element with
// an index greater then the given column index. In case of a column-major matrix, the function
// returns a column iterator to the first element with an index greater then the given row
// index. In combination with the lowerBound() function this function can be used to create a
// pair of iterators specifying a range of indices.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline typename ZeroMatrix<Type,SO>::ConstIterator
   ZeroMatrix<Type,SO>::upperBound( size_t i, size_t j ) const
{
   BLAZE_USER_ASSERT( SO  || i < rows()   , "Invalid zero matrix row access index"    );
   BLAZE_USER_ASSERT( !SO || j < columns(), "Invalid zero matrix column access index" );

   return nullptr;
}
//*************************************************************************************************




//=================================================================================================
//
//  NUMERIC FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief In-place transpose of the matrix.
//
// \return Reference to the transposed matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr ZeroMatrix<Type,SO>& ZeroMatrix<Type,SO>::transpose() noexcept
{
   const size_t tmp( m_ );
   m_ = n_;
   n_ = tmp;

   return *this;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief In-place conjugate transpose of the matrix.
//
// \return Reference to the transposed matrix.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr ZeroMatrix<Type,SO>& ZeroMatrix<Type,SO>::ctranspose() noexcept
{
   const size_t tmp( m_ );
   m_ = n_;
   n_ = tmp;

   return *this;
}
//*************************************************************************************************




//=================================================================================================
//
//  EXPRESSION TEMPLATE EVALUATION FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Returns whether the matrix can alias with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this matrix, \a false if not.
//
// This function returns whether the given address can alias with the matrix. In contrast
// to the isAliased() function this function is allowed to use compile time expressions
// to optimize the evaluation.
*/
template< typename Type     // Data type of the matrix
        , bool SO >         // Storage order
template< typename Other >  // Data type of the foreign expression
inline bool ZeroMatrix<Type,SO>::canAlias( const Other* alias ) const noexcept
{
   UNUSED_PARAMETER( alias );

   return false;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns whether the matrix is aliased with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this matrix, \a false if not.
//
// This function returns whether the given address is aliased with the matrix. In contrast
// to the canAlias() function this function is not allowed to use compile time expressions
// to optimize the evaluation.
*/
template< typename Type     // Data type of the matrix
        , bool SO >         // Storage order
template< typename Other >  // Data type of the foreign expression
inline bool ZeroMatrix<Type,SO>::isAliased( const Other* alias ) const noexcept
{
   UNUSED_PARAMETER( alias );

   return false;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns whether the matrix can be used in SMP assignments.
//
// \return \a true in case the matrix can be used in SMP assignments, \a false if not.
//
// This function returns whether the matrix can be used in SMP assignments. In contrast to the
// \a smpAssignable member enumeration, which is based solely on compile time information, this
// function additionally provides runtime information (as for instance the current number of
// rows and/or columns of the matrix).
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline bool ZeroMatrix<Type,SO>::canSMPAssign() const noexcept
{
   return false;
}
//*************************************************************************************************








//=================================================================================================
//
//  ZEROMATRIX OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*!\name ZeroMatrix operators */
//@{
template< typename Type, bool SO >
constexpr void reset( ZeroMatrix<Type,SO>& m ) noexcept;

template< typename Type, bool SO >
constexpr void reset( ZeroMatrix<Type,SO>& m, size_t i ) noexcept;

template< typename Type, bool SO >
constexpr void clear( ZeroMatrix<Type,SO>& m ) noexcept;

template< bool RF, typename Type, bool SO >
constexpr bool isDefault( const ZeroMatrix<Type,SO>& m ) noexcept;

template< typename Type, bool SO >
constexpr bool isIntact( const ZeroMatrix<Type,SO>& m ) noexcept;

template< typename Type, bool SO >
constexpr void swap( ZeroMatrix<Type,SO>& a, ZeroMatrix<Type,SO>& b ) noexcept;
//@}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Resetting the given zero matrix.
// \ingroup zero_matrix
//
// \param m The matrix to be resetted.
// \return void
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr void reset( ZeroMatrix<Type,SO>& m ) noexcept
{
   UNUSED_PARAMETER( m );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Reset the specified row/column of the given zero matrix.
// \ingroup zero_matrix
//
// \param m The matrix to be resetted.
// \param i The index of the row/column to be resetted.
// \return void
//
// This function resets the values in the specified row/column of the given zero matrix to their
// default value. In case the given matrix is a \a rowMajor matrix the function resets the values
// in row \a i, if it is a \a columnMajor matrix the function resets the values in column \a i.
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr void reset( ZeroMatrix<Type,SO>& m, size_t i ) noexcept
{
   UNUSED_PARAMETER( m, i );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Clearing the given zero matrix.
// \ingroup zero_matrix
//
// \param m The matrix to be cleared.
// \return void
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr void clear( ZeroMatrix<Type,SO>& m ) noexcept
{
   m.clear();
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns whether the given zero matrix is in default state.
// \ingroup zero_matrix
//
// \param m The matrix to be tested for its default state.
// \return \a true in case the given matrix's rows and columns are zero, \a false otherwise.
//
// This function checks whether the zero matrix is in default (constructed) state, i.e. if
// it's number of rows and columns is 0. In case it is in default state, the function returns
// \a true, else it will return \a false. The following example demonstrates the use of the
// \a isDefault() function:

   \code
   blaze::ZeroMatrix<int> Z;
   // ... Resizing and initialization
   if( isDefault( Z ) ) { ... }
   \endcode

// Optionally, it is possible to switch between strict semantics (blaze::strict) and relaxed
// semantics (blaze::relaxed):

   \code
   if( isDefault<relaxed>( Z ) ) { ... }
   \endcode
*/
template< bool RF        // Relaxation flag
        , typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr bool isDefault( const ZeroMatrix<Type,SO>& m ) noexcept
{
   return ( m.rows() == 0UL && m.columns() == 0UL );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns whether the invariants of the given zero matrix are intact.
// \ingroup zero_matrix
//
// \param m The zero matrix to be tested.
// \return \a true in case the given matrix's invariants are intact, \a false otherwise.
//
// This function checks whether the invariants of the zero matrix are intact, i.e. if its state
// is valid. In case the invariants are intact, the function returns \a true, else it will return
// \a false. The following example demonstrates the use of the \a isIntact() function:

   \code
   blaze::ZeroMatrix<int> Z;
   // ... Resizing and initialization
   if( isIntact( Z ) ) { ... }
   \endcode
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr bool isIntact( const ZeroMatrix<Type,SO>& m ) noexcept
{
   UNUSED_PARAMETER( m );

   return true;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Swapping the contents of two zero matrices.
// \ingroup zero_matrix
//
// \param a The first matrix to be swapped.
// \param b The second matrix to be swapped.
// \return void
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline constexpr void swap( ZeroMatrix<Type,SO>& a, ZeroMatrix<Type,SO>& b ) noexcept
{
   a.swap( b );
}
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Erasing an element from the given zero matrix.
// \ingroup zero_matrix
//
// \param m The given zero matrix.
// \param i The row index of the element to be erased. The index has to be in the range \f$[0..M-1]\f$.
// \param j The column index of the element to be erased. The index has to be in the range \f$[0..N-1]\f$.
// \return void
*/
template< typename Type  // Data type of the matrix
        , bool SO >      // Storage order
inline void erase( ZeroMatrix<Type,SO>& m, size_t i, size_t j )
{
   UNUSED_PARAMETER( m, i, j );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Erasing an element from the given zero matrix.
// \ingroup zero_matrix
//
// \param m The given zero matrix.
// \param i The row/column index of the element to be erased. The index has to be in the range \f$[0..M-1]\f$.
// \param pos Iterator to the element to be erased.
// \return Iterator to the element after the erased element.
//
// This function erases an element from the given zero matrix. In case the storage order is set
// to \a rowMajor the function erases an element from row \a i, in case the storage flag is set
// to \a columnMajor the function erases an element from column \a i.
*/
template< typename Type        // Data type of the matrix
        , bool SO              // Storage order
        , typename Iterator >  // Type of the matrix iterator
inline Iterator erase( ZeroMatrix<Type,SO>& m, size_t i, Iterator pos )
{
   UNUSED_PARAMETER( m, i, pos );
   return Iterator();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Erasing a range of elements from the given zero matrix.
// \ingroup zero_matrix
//
// \param m The given zero matrix.
// \param i The row/column index of the elements to be erased. The index has to be in the range \f$[0..M-1]\f$.
// \param first Iterator to first element to be erased.
// \param last Iterator just past the last element to be erased.
// \return Iterator to the element after the erased element.
//
// This function erases a range of elements from the given zero matrix. In case the storage
// order is set to \a rowMajor the function erases a range of elements from row \a i, in case
// the storage flag is set to \a columnMajor the function erases a range of elements from column
// \a i.
*/
template< typename Type        // Data type of the matrix
        , bool SO              // Storage order
        , typename Iterator >  // Type of the matrix iterator
inline Iterator erase( ZeroMatrix<Type,SO>& m, size_t i, Iterator first, Iterator last )
{
   UNUSED_PARAMETER( m, i, first, last );
   return Iterator();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Erasing specific elements from the given zero matrix.
// \ingroup zero_matrix
//
// \param m The given zero matrix.
// \param predicate The unary predicate for the element selection.
// \return void.
//
// This function erases specific elements from the given zero matrix. The elements are selected
// by the given unary predicate \a predicate, which is expected to accept a single argument of
// the type of the elements and to be pure. The following example demonstrates how to remove
// all elements that are smaller than a certain threshold value:

   \code
   blaze::ZeroMatrix<double,blaze::rowMajor> Z;
   // ... Resizing and initialization

   erase( Z, []( double value ){ return value < 1E-8; } );
   \endcode

// \note The predicate is required to be pure, i.e. to produce deterministic results for elements
// with the same value. The attempt to use an impure predicate leads to undefined behavior!
*/
template< typename Type    // Data type of the matrix
        , bool SO          // Storage order
        , typename Pred >  // Type of the unary predicate
inline void erase( ZeroMatrix<Type,SO>& m, Pred predicate )
{
   UNUSED_PARAMETER( m, predicate );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Erasing specific elements from a range of the given zero matrix.
// \ingroup zero_matrix
//
// \param m The given zero matrix.
// \param i The row/column index of the elements to be erased. The index has to be in the range \f$[0..M-1]\f$.
// \param first Iterator to first element of the range.
// \param last Iterator just past the last element of the range.
// \param predicate The unary predicate for the element selection.
// \return void
//
// This function erases specific elements from a range of elements of the zero matrix. The
// elements are selected by the given unary predicate \a predicate, which is expected to accept
// a single argument of the type of the elements and to be pure. In case the storage order is
// set to \a rowMajor the function erases a range of elements from row \a i, in case the storage
// flag is set to \a columnMajor the function erases a range of elements from column \a i. The
// following example demonstrates how to remove all elements that are smaller than a certain
// threshold value:

   \code
   blaze::ZeroMatrix<double,blaze::rowMajor> Z;
   // ... Resizing and initialization

   erase( Z, 2UL, Z.begin(2UL), Z.end(2UL), []( double value ){ return value < 1E-8; } );
   \endcode

// \note The predicate is required to be pure, i.e. to produce deterministic results for elements
// with the same value. The attempt to use an impure predicate leads to undefined behavior!
*/
template< typename Type      // Data type of the matrix
        , bool SO            // Storage order
        , typename Iterator  // Type of the matrix iterator
        , typename Pred >    // Type of the unary predicate
inline void erase( ZeroMatrix<Type,SO>& m, size_t i, Iterator first, Iterator last, Pred predicate )
{
   UNUSED_PARAMETER( m, i, first, last, predicate );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL BINARY ARITHMETIC OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition operator for the addition of a zero matrix and a dense matrix (\f$ A=B+C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the addition.
// \param rhs The right-hand side dense matrix for the addition.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the addition of a zero matrix and a dense matrix:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A;
   blaze::DynamicMatrix<double,rowMajor> B, C;
   // ... Resizing and initialization
   C = A + B;
   \endcode

// The operator returns a reference to the given dense matrix. In case the current sizes of the
// two given matrices don't match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side dense matrix
        , bool SO2      // Storage order of the right-hand side dense matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> &&
                      IsSame_v< ElementType_t<MT1>, ElementType_t<MT2> > >* = nullptr >
inline decltype(auto)
   operator+( const MT1& lhs, const DenseMatrix<MT2,SO2>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.rows() != (~rhs).rows() || lhs.columns() != (~rhs).columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   return (~rhs);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition operator for the addition of a dense matrix and a zero matrix (\f$ A=B+C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side dense matrix for the addition.
// \param rhs The right-hand side zero matrix for the addition.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the addition of a dense matrix and a zero matrix:

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> A, C;
   blaze::ZeroMatrix<double,rowMajor> B;
   // ... Resizing and initialization
   C = A + B;
   \endcode

// The operator returns a reference to the given dense matrix. In case the current sizes of the
// two given matrices don't match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side dense matrix
        , bool SO1      // Storage order of the left-hand side dense matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT2> && IsZero_v<MT2> &&
                      IsSame_v< ElementType_t<MT1>, ElementType_t<MT2> > >* = nullptr >
inline decltype(auto)
   operator+( const DenseMatrix<MT1,SO1>& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).rows() != rhs.rows() || (~lhs).columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   return (~lhs);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition operator for the addition of a zero matrix and a sparse matrix (\f$ A=B+C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the addition.
// \param rhs The right-hand side sparse matrix for the addition.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the addition of a zero matrix and a sparse matrix:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A;
   blaze::DynamicMatrix<double,rowMajor> B, C;
   // ... Resizing and initialization
   C = A + B;
   \endcode

// The operator returns a reference to the given sparse matrix. In case the current sizes of the
// two given matrices don't match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side sparse matrix
        , bool SO2      // Storage order of the right-hand side sparse matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> &&
                      IsSame_v< ElementType_t<MT1>, ElementType_t<MT2> > >* = nullptr >
inline decltype(auto)
   operator+( const MT1& lhs, const SparseMatrix<MT2,SO2>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.rows() != (~rhs).rows() || lhs.columns() != (~rhs).columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   return (~rhs);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition operator for the addition of a sparse matrix and a zero matrix (\f$ A=B+C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side sparse matrix for the addition.
// \param rhs The right-hand side zero matrix for the addition.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the addition of a sparse matrix and a zero matrix:

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> A, C;
   blaze::ZeroMatrix<double,rowMajor> B;
   // ... Resizing and initialization
   C = A + B;
   \endcode

// The operator returns a reference to the given sparse matrix. In case the current sizes of the
// two given matrices don't match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side sparse matrix
        , bool SO1      // Storage order of the left-hand side sparse matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT2> && IsZero_v<MT2> &&
                      IsSame_v< ElementType_t<MT1>, ElementType_t<MT2> > >* = nullptr >
inline decltype(auto)
   operator+( const SparseMatrix<MT1,SO1>& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).rows() != rhs.rows() || (~lhs).columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   return (~lhs);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition operator for the addition of two zero matrices (\f$ A=B+C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the addition.
// \param rhs The right-hand side zero matrix for the addition.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the addition of two zero matrices:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A, B, C;
   // ... Resizing and initialization
   C = A + B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices
// don't match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> &&
                      IsMatrix_v<MT2> && IsZero_v<MT2> >* = nullptr >
inline decltype(auto)
   operator+( const MT1& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.rows() != rhs.rows() || lhs.columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = AddTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT1>;

   return ZeroMatrix<ET,SO>( lhs.rows(), lhs.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subtraction operator for the subtraction of a dense matrix and a zero matrix
//        (\f$ A=B-C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side dense matrix for the subtraction.
// \param rhs The right-hand side zero matrix for the subtraction.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the subtraction of a dense matrix and a zero matrix:

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> A, C;
   blaze::ZeroMatrix<double,rowMajor> B;
   // ... Resizing and initialization
   C = A - B;
   \endcode

// The operator returns a reference to the given dense matrix. In case the current sizes of the
// two given matrices don't match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side dense matrix
        , bool SO1      // Storage order of the left-hand side dense matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT2> && IsZero_v<MT2> &&
                      IsSame_v< ElementType_t<MT1>, ElementType_t<MT2> > >* = nullptr >
inline decltype(auto)
   operator-( const DenseMatrix<MT1,SO1>& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).rows() != rhs.rows() || (~lhs).columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   return (~lhs);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subtraction operator for the subtraction of a sparse matrix and a zero matrix
//        (\f$ A=B-C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side sparse matrix for the subtraction.
// \param rhs The right-hand side zero matrix for the subtraction.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the subtraction of a sparse matrix and a zero matrix:

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> A, C;
   blaze::ZeroMatrix<double,rowMajor> B;
   // ... Resizing and initialization
   C = A - B;
   \endcode

// The operator returns a reference to the given sparse matrix. In case the current sizes of the
// two given matrices don't match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side sparse matrix
        , bool SO1      // Storage order of the left-hand side sparse matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT2> && IsZero_v<MT2> &&
                      IsSame_v< ElementType_t<MT1>, ElementType_t<MT2> > >* = nullptr >
inline decltype(auto)
   operator-( const SparseMatrix<MT1,SO1>& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).rows() != rhs.rows() || (~lhs).columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   return (~lhs);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subtraction operator for the subtraction of two zero matrices (\f$ A=B-C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the subtraction.
// \param rhs The right-hand side zero matrix for the subtraction.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the subtraction of two zero matrices:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A, B, C;
   // ... Resizing and initialization
   C = A - B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices
// don't match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> &&
                      IsMatrix_v<MT2> && IsZero_v<MT2> >* = nullptr >
inline decltype(auto)
   operator-( const MT1& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.rows() != rhs.rows() || lhs.columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = SubTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT1>;

   return ZeroMatrix<ET,SO>( lhs.rows(), lhs.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Operator for the Schur product of a zero matrix and a dense matrix (\f$ A=B%C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the Schur product.
// \param rhs The right-hand side dense matrix for the Schur product.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the Schur product of a zero matrix and a dense matrix:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A;
   blaze::DynamicMatrix<double,rowMajor> B, C;
   // ... Resizing and initialization
   C = A % B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices don't
// match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side dense matrix
        , bool SO2      // Storage order of the right-hand side dense matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> >* = nullptr >
inline decltype(auto)
   operator%( const MT1& lhs, const DenseMatrix<MT2,SO2>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.rows() != (~rhs).rows() || lhs.columns() != (~rhs).columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT1>;

   return ZeroMatrix<ET,SO>( lhs.rows(), lhs.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Operator for the Schur product of a dense matrix and a zero matrix (\f$ A=B%C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side dense matrix for the Schur product.
// \param rhs The right-hand side zero matrix for the Schur product.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the Schur product of a dense matrix and a zero matrix:

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> A, C;
   blaze::ZeroMatrix<double,rowMajor> B;
   // ... Resizing and initialization
   C = A % B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices don't
// match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side dense matrix
        , bool SO1      // Storage order of the left-hand side dense matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT2> && IsZero_v<MT2> >* = nullptr >
inline decltype(auto)
   operator%( const DenseMatrix<MT1,SO1>& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).rows() != rhs.rows() || (~lhs).columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT2>;

   return ZeroMatrix<ET,SO>( rhs.rows(), rhs.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Operator for the Schur product of a zero matrix and a sparse matrix (\f$ A=B%C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the Schur product.
// \param rhs The right-hand side sparse matrix for the Schur product.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the Schur product of a zero matrix and a sparse matrix:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A;
   blaze::DynamicMatrix<double,rowMajor> B, C;
   // ... Resizing and initialization
   C = A % B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices don't
// match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side sparse matrix
        , bool SO2      // Storage order of the right-hand side sparse matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> >* = nullptr >
inline decltype(auto)
   operator%( const MT1& lhs, const SparseMatrix<MT2,SO2>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.rows() != (~rhs).rows() || lhs.columns() != (~rhs).columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT1> && SO2;

   return ZeroMatrix<ET,SO>( lhs.rows(), lhs.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Operator for the Schur product of a sparse matrix and a zero matrix (\f$ A=B%C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side sparse matrix for the Schur product.
// \param rhs The right-hand side zero matrix for the Schur product.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the Schur product of a sparse matrix and a zero matrix:

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> A, C;
   blaze::ZeroMatrix<double,rowMajor> B;
   // ... Resizing and initialization
   C = A % B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices don't
// match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side sparse matrix
        , bool SO1      // Storage order of the left-hand side sparse matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT2> && IsZero_v<MT2> >* = nullptr >
inline decltype(auto)
   operator%( const SparseMatrix<MT1,SO1>& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).rows() != rhs.rows() || (~lhs).columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = SO1 && StorageOrder_v<MT2>;

   return ZeroMatrix<ET,SO>( rhs.rows(), rhs.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Operator for the Schur product of two zero matrices (\f$ A=B%C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the Schur product.
// \param rhs The right-hand side zero matrix for the Schur product.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the Schur product of two zero matrices:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A, B, C;
   // ... Resizing and initialization
   C = A % B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices don't
// match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> &&
                      IsMatrix_v<MT2> && IsZero_v<MT2> >* = nullptr >
inline decltype(auto)
   operator%( const MT1& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.rows() != rhs.rows() || lhs.columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT1> && StorageOrder_v<MT2>;

   return ZeroMatrix<ET,SO>( lhs.rows(), lhs.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication operator for the multiplication of a zero matrix and a scalar value
//        (\f$ A=B*s \f$).
// \ingroup zero_matrix
//
// \param mat The left-hand side zero matrix for the multiplication.
// \param scalar The right-hand side scalar value for the multiplication.
// \return The scaled result matrix.
//
// This operator represents the multiplication between a zero matrix and a scalar value:

   \code
   blaze::ZeroMatrix<double> A, B;
   // ... Resizing and initialization
   B = A * 1.25;
   \endcode

// The operator returns a zero matrix of the higher-order element type of the involved data types
// \a MT::ElementType and \a ST. Note that this operator only works for scalar values of built-in
// data type.
*/
template< typename MT  // Type of the left-hand side zero matrix
        , typename ST  // Type of the right-hand side scalar
        , EnableIf_t< IsMatrix_v<MT> && IsZero_v<MT> && IsNumeric_v<ST> >* = nullptr >
inline decltype(auto)
   operator*( const MT& mat, ST scalar )
{
   BLAZE_FUNCTION_TRACE;

   UNUSED_PARAMETER( scalar );

   using ET = MultTrait_t< ElementType_t<MT>, ST >;
   constexpr bool SO = StorageOrder_v<MT>;

   return ZeroMatrix<ET,SO>( mat.rows(), mat.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication operator for the multiplication of a scalar value and a zero matrix
//        (\f$ A=s*B \f$).
// \ingroup zero_matrix
//
// \param scalar The left-hand side scalar value for the multiplication.
// \param mat The right-hand side zero matrix for the multiplication.
// \return The scaled result matrix.
//
// This operator represents the multiplication between a scalar value and a zero matrix:

   \code
   blaze::ZeroMatrix<double> A, B;
   // ... Resizing and initialization
   B = 1.25 * A;
   \endcode

// The operator returns a zero matrix of the higher-order element type of the involved data types
// \a ST and \a MT::ElementType. Note that this operator only works for scalar values of built-in
// data type.
*/
template< typename ST  // Type of the left-hand side scalar
        , typename MT  // Type of the right-hand side zero matrix
        , EnableIf_t< IsNumeric_v<ST> && IsMatrix_v<MT> && IsZero_v<MT> >* = nullptr >
inline decltype(auto)
   operator*( ST scalar, const MT& mat )
{
   BLAZE_FUNCTION_TRACE;

   UNUSED_PARAMETER( scalar );

   using ET = MultTrait_t< ST, ElementType_t<MT> >;
   constexpr bool SO = StorageOrder_v<MT>;

   return ZeroMatrix<ET,SO>( mat.rows(), mat.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication operator for the multiplication of a zero matrix and a dense matrix
//        (\f$ A=B*C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the multiplication.
// \param rhs The right-hand side dense matrix for the multiplication.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the multiplication of a zero matrix and a dense matrix:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A;
   blaze::DynamicMatrix<double,rowMajor> B, C;
   // ... Resizing and initialization
   C = A * B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices don't
// match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side dense matrix
        , bool SO2      // Storage order of the right-hand side dense matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> >* = nullptr >
inline decltype(auto)
   operator*( const MT1& lhs, const DenseMatrix<MT2,SO2>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.columns() != (~rhs).rows() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT1>;

   return ZeroMatrix<ET,SO>( lhs.rows(), (~rhs).columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication operator for the multiplication of a dense matrix and a zero matrix
//        (\f$ A=B*C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side dense matrix for the multiplication.
// \param rhs The right-hand side zero matrix for the multiplication.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the multiplication of a dense matrix and a zero matrix:

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> A, C;
   blaze::ZeroMatrix<double,rowMajor> B;
   // ... Resizing and initialization
   C = A * B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices don't
// match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side dense matrix
        , bool SO1      // Storage order of the left-hand side dense matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT2> && IsZero_v<MT2> >* = nullptr >
inline decltype(auto)
   operator*( const DenseMatrix<MT1,SO1>& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).columns() != rhs.rows() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT2>;

   return ZeroMatrix<ET,SO>( (~lhs).rows(), rhs.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication operator for the multiplication of a zero matrix and a sparse matrix
//        (\f$ A=B*C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the multiplication.
// \param rhs The right-hand side sparse matrix for the multiplication.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the multiplication of a zero matrix and a sparse matrix:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A;
   blaze::DynamicMatrix<double,rowMajor> B, C;
   // ... Resizing and initialization
   C = A * B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices don't
// match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side sparse matrix
        , bool SO2      // Storage order of the right-hand side sparse matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> >* = nullptr >
inline decltype(auto)
   operator*( const MT1& lhs, const SparseMatrix<MT2,SO2>& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.columns() != (~rhs).rows() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT1>;

   return ZeroMatrix<ET,SO>( lhs.rows(), (~rhs).columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication operator for the multiplication of a sparse matrix and a zero matrix
//        (\f$ A=B*C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side sparse matrix for the multiplication.
// \param rhs The right-hand side zero matrix for the multiplication.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the multiplication of a sparse matrix and a zero matrix:

   \code
   using blaze::rowMajor;

   blaze::DynamicMatrix<double,rowMajor> A, C;
   blaze::ZeroMatrix<double,rowMajor> B;
   // ... Resizing and initialization
   C = A * B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices don't
// match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side sparse matrix
        , bool SO1      // Storage order of the left-hand side sparse matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT2> && IsZero_v<MT2> >* = nullptr >
inline decltype(auto)
   operator*( const SparseMatrix<MT1,SO1>& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( (~lhs).columns() != rhs.rows() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT2>;

   return ZeroMatrix<ET,SO>( (~lhs).rows(), rhs.columns() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication operator for the multiplication of two zero matrices (\f$ A=B*C \f$).
// \ingroup zero_matrix
//
// \param lhs The left-hand side zero matrix for the multiplication.
// \param rhs The right-hand side zero matrix for the multiplication.
// \return The resulting matrix.
// \exception std::invalid_argument Matrix sizes do not match.
//
// This operator represents the multiplication of two zero matrices:

   \code
   using blaze::rowMajor;

   blaze::ZeroMatrix<double,rowMajor> A, B, C;
   // ... Resizing and initialization
   C = A * B;
   \endcode

// The operator returns a zero matrix. In case the current sizes of the two given matrices
// don't match, a \a std::invalid_argument is thrown.
*/
template< typename MT1  // Type of the left-hand side zero matrix
        , typename MT2  // Type of the right-hand side zero matrix
        , EnableIf_t< IsMatrix_v<MT1> && IsZero_v<MT1> &&
                      IsMatrix_v<MT2> && IsZero_v<MT2> >* = nullptr >
inline decltype(auto)
   operator*( const MT1& lhs, const MT2& rhs )
{
   BLAZE_FUNCTION_TRACE;

   if( lhs.rows() != rhs.rows() || lhs.columns() != rhs.columns() ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Matrix sizes do not match" );
   }

   using ET = MultTrait_t< ElementType_t<MT1>, ElementType_t<MT2> >;
   constexpr bool SO = StorageOrder_v<MT1>;

   return ZeroMatrix<ET,SO>( lhs.rows(), rhs.columns() );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Declares the given matrix expression \a m as zero matrix.
// \ingroup zero_matrix
//
// \param m The input matrix.
// \return The redeclared matrix.
//
// The \a declzero function declares the given dense or sparse matrix expression \a m as zero
// matrix. The following example demonstrates the use of the \a declzero function:

   \code
   blaze::CompressedMatrix<double> A, B;
   // ... Resizing and initialization
   B = declzero( A );
   \endcode
*/
template< typename MT  // Type of the sparse matrix
        , bool SO >    // Storage order
inline ZeroMatrix<ElementType_t<MT>,SO>
   declzero( const Matrix<MT,SO>& m )
{
   BLAZE_FUNCTION_TRACE;

   return ZeroMatrix<ElementType_t<MT>,SO>( (~m).rows(), (~m).columns() );
}
//*************************************************************************************************




//=================================================================================================
//
//  ISUNIFORM SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename Type, bool SO >
struct IsUniform< ZeroMatrix<Type,SO> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISZERO SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename MT, bool SO >
struct IsZero< ZeroMatrix<MT,SO> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISRESIZABLE SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T, bool SO >
struct IsResizable< ZeroMatrix<T,SO> >
   : public TrueType
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ADDTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, typename T2 >
struct AddTraitEval1< T1, T2
                    , EnableIf_t< IsMatrix_v<T1> &&
                                  IsMatrix_v<T2> &&
                                  !IsZero_v<T1> && IsZero_v<T2> > >
{
   using Type = Rebind_t< ResultType_t<T1>, AddTrait_t< ElementType_t<T1>, ElementType_t<T2> > >;
};

template< typename T1, typename T2 >
struct AddTraitEval1< T1, T2
                    , EnableIf_t< IsMatrix_v<T1> &&
                                  IsMatrix_v<T2> &&
                                  IsZero_v<T1> && !IsZero_v<T2> > >
{
   using Type = Rebind_t< ResultType_t<T2>, AddTrait_t< ElementType_t<T1>, ElementType_t<T2> > >;
};

template< typename T1, typename T2 >
struct AddTraitEval1< T1, T2
                    , EnableIf_t< IsMatrix_v<T1> &&
                                  IsMatrix_v<T2> &&
                                  IsZero_v<T1> && IsZero_v<T2> > >
{
   using ET1 = ElementType_t<T1>;
   using ET2 = ElementType_t<T2>;

   using Type = ZeroMatrix< AddTrait_t<ET1,ET2>, StorageOrder_v<T1> >;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  SUBTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, typename T2 >
struct SubTraitEval1< T1, T2
                    , EnableIf_t< IsMatrix_v<T1> &&
                                  IsMatrix_v<T2> &&
                                  !IsZero_v<T1> && IsZero_v<T2> > >
{
   using Type = Rebind_t< ResultType_t<T1>, SubTrait_t< ElementType_t<T1>, ElementType_t<T2> > >;
};

template< typename T1, typename T2 >
struct SubTraitEval1< T1, T2
                    , EnableIf_t< IsMatrix_v<T1> &&
                                  IsMatrix_v<T2> &&
                                  IsZero_v<T1> && !IsZero_v<T2> && !IsIdentity_v<T2> > >
{
   using Type = Rebind_t< ResultType_t<T2>, SubTrait_t< ElementType_t<T1>, ElementType_t<T2> > >;
};

template< typename T1, typename T2 >
struct SubTraitEval1< T1, T2
                    , EnableIf_t< IsMatrix_v<T1> &&
                                  IsMatrix_v<T2> &&
                                  IsZero_v<T1> && IsZero_v<T2> > >
{
   using ET1 = ElementType_t<T1>;
   using ET2 = ElementType_t<T2>;

   using Type = ZeroMatrix< SubTrait_t<ET1,ET2>, StorageOrder_v<T1> >;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  SCHURTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, typename T2 >
struct SchurTraitEval1< T1, T2
                      , EnableIf_t< IsMatrix_v<T1> &&
                                    IsMatrix_v<T2> &&
                                    ( IsZero_v<T1> || IsZero_v<T2> ) > >
{
   using ET1 = ElementType_t<T1>;
   using ET2 = ElementType_t<T2>;

   static constexpr bool SO1 = StorageOrder_v<T1>;
   static constexpr bool SO2 = StorageOrder_v<T2>;

   static constexpr bool SO = ( IsSparseMatrix_v<T1> && IsSparseMatrix_v<T2>
                                ? ( SO1 && SO2 )
                                : ( IsSparseMatrix_v<T1>
                                    ? SO1
                                    : SO2 ) );

   using Type = ZeroMatrix< MultTrait_t<ET1,ET2>, SO >;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  MULTTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, typename T2 >
struct MultTraitEval1< T1, T2
                     , EnableIf_t< IsMatrix_v<T1> && IsZero_v<T1> && IsNumeric_v<T2> > >
{
   using Type = ZeroMatrix< MultTrait_t< ElementType_t<T1>, T2 >, StorageOrder_v<T1> >;
};

template< typename T1, typename T2 >
struct MultTraitEval1< T1, T2
                     , EnableIf_t< IsNumeric_v<T1> && IsMatrix_v<T2> && IsZero_v<T2> > >
{
   using Type = ZeroMatrix< MultTrait_t< T1, ElementType_t<T2> >, StorageOrder_v<T2> >;
};

template< typename T1, typename T2 >
struct MultTraitEval1< T1, T2
                     , EnableIf_t< IsMatrix_v<T1> &&
                                   IsMatrix_v<T2> &&
                                   ( IsZero_v<T1> || IsZero_v<T2> ) > >
{
   using ET1 = ElementType_t<T1>;
   using ET2 = ElementType_t<T2>;

   static constexpr bool SO = ( IsZero_v<T1> ? StorageOrder_v<T1> : StorageOrder_v<T2> );

   using Type = ZeroMatrix< MultTrait_t<ET1,ET2>, SO >;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  DIVTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, typename T2 >
struct DivTraitEval1< T1, T2
                    , EnableIf_t< IsMatrix_v<T1> &&
                                  IsNumeric_v<T2> &&
                                  IsZero_v<T1> > >
{
   using ET1 = ElementType_t<T1>;

   using Type = ZeroMatrix< DivTrait_t<ET1,T2>, StorageOrder_v<T1> >;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  MAPTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T, typename OP >
struct UnaryMapTraitEval1< T, OP
                         , EnableIf_t< IsMatrix_v<T> &&
                                       YieldsZero_v<OP,T> > >
{
   using ET = ElementType_t<T>;

   using Type = ZeroMatrix< MapTrait_t<ET,OP>, StorageOrder_v<T> >;
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, typename T2, typename OP >
struct BinaryMapTraitEval1< T1, T2, OP
                          , EnableIf_t< IsMatrix_v<T1> &&
                                        IsMatrix_v<T2> &&
                                        YieldsZero_v<OP,T1,T2> > >
{
   using ET1 = ElementType_t<T1>;
   using ET2 = ElementType_t<T2>;

   static constexpr bool SO = StorageOrder_v<T1> && StorageOrder_v<T2>;

   using Type = ZeroMatrix< MapTrait_t<ET1,ET2,OP>, SO >;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  HIGHTYPE SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, bool SO, typename T2 >
struct HighType< ZeroMatrix<T1,SO>, ZeroMatrix<T2,SO> >
{
   using Type = ZeroMatrix< typename HighType<T1,T2>::Type, SO >;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  LOWTYPE SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, bool SO, typename T2 >
struct LowType< ZeroMatrix<T1,SO>, ZeroMatrix<T2,SO> >
{
   using Type = ZeroMatrix< typename LowType<T1,T2>::Type, SO >;
};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
