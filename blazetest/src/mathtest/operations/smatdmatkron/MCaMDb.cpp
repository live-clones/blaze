//=================================================================================================
/*!
//  \file src/mathtest/operations/smatdmatkron/MCaMDb.cpp
//  \brief Source file for the MCaMDb sparse matrix/dense matrix Kronecker product math test
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


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <cstdlib>
#include <iostream>
#include <blaze/math/CompressedMatrix.h>
#include <blaze/math/DynamicMatrix.h>
#include <blazetest/mathtest/Creator.h>
#include <blazetest/mathtest/operations/smatdmatkron/OperationTest.h>
#include <blazetest/system/MathTest.h>

#ifdef BLAZE_USE_HPX_THREADS
#  include <hpx/hpx_main.hpp>
#endif


//=================================================================================================
//
//  MAIN FUNCTION
//
//=================================================================================================

//*************************************************************************************************
int main()
{
   std::cout << "   Running 'MCaMDb'..." << std::endl;

   using blazetest::mathtest::TypeA;
   using blazetest::mathtest::TypeB;

   try
   {
      // Matrix type definitions
      using MCa = blaze::CompressedMatrix<TypeA>;
      using MDb = blaze::DynamicMatrix<TypeB>;

      // Creator type definitions
      using CMCa = blazetest::Creator<MCa>;
      using CMDb = blazetest::Creator<MDb>;

      // Running tests with small matrices
      for( size_t i=0UL; i<=4UL; ++i ) {
         for( size_t j=0UL; j<=4UL; ++j ) {
            for( size_t k=0UL; k<=i*j; ++k ) {
               for( size_t l=0UL; l<=4UL; ++l ) {
                  for( size_t m=0UL; m<=4UL; ++m ) {
                     RUN_SMATDMATKRON_OPERATION_TEST( CMCa( i, j, k ), CMDb( l, m ) );
                  }
               }
            }
         }
      }

      // Running tests with large matrices
      RUN_SMATDMATKRON_OPERATION_TEST( CMCa(  9UL, 15UL, 7UL ), CMDb( 16UL,  8UL ) );
      RUN_SMATDMATKRON_OPERATION_TEST( CMCa(  9UL, 15UL, 7UL ), CMDb(  8UL, 16UL ) );
      RUN_SMATDMATKRON_OPERATION_TEST( CMCa( 16UL,  8UL, 7UL ), CMDb(  9UL, 15UL ) );
      RUN_SMATDMATKRON_OPERATION_TEST( CMCa( 16UL,  8UL, 7UL ), CMDb( 15UL,  9UL ) );
   }
   catch( std::exception& ex ) {
      std::cerr << "\n\n ERROR DETECTED during sparse matrix/dense matrix Kronecker product:\n"
                << ex.what() << "\n";
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
//*************************************************************************************************
