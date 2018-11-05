//=================================================================================================
/*!
//  \file src/mathtest/dvecdvecouter/VHaVUa.cpp
//  \brief Source file for the VHaVUa dense vector/dense vector outer product math test
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


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <cstdlib>
#include <iostream>
#include <blaze/math/HybridVector.h>
#include <blaze/math/UniformVector.h>
#include <blazetest/mathtest/Creator.h>
#include <blazetest/mathtest/dvecdvecouter/OperationTest.h>
#include <blazetest/system/MathTest.h>


//=================================================================================================
//
//  MAIN FUNCTION
//
//=================================================================================================

//*************************************************************************************************
int main()
{
   std::cout << "   Running 'VHaVUa'..." << std::endl;

   using blazetest::mathtest::TypeA;

   try
   {
      // Vector type definitions
      using VHa = blaze::HybridVector<TypeA,128UL>;
      using VUa = blaze::UniformVector<TypeA>;

      // Creator type definitions
      using CVHa = blazetest::Creator<VHa>;
      using CVUa = blazetest::Creator<VUa>;

      // Running tests with small vectors
      for( size_t i=0UL; i<=6UL; ++i ) {
         for( size_t j=0UL; j<=6UL; ++j ) {
            RUN_DVECDVECOUTER_OPERATION_TEST( CVHa( i ), CVUa( j ) );
         }
      }

      // Running tests with large vectors
      RUN_DVECDVECOUTER_OPERATION_TEST( CVHa(  67UL ), CVUa(  67UL ) );
      RUN_DVECDVECOUTER_OPERATION_TEST( CVHa(  67UL ), CVUa( 127UL ) );
      RUN_DVECDVECOUTER_OPERATION_TEST( CVHa( 127UL ), CVUa(  67UL ) );
      RUN_DVECDVECOUTER_OPERATION_TEST( CVHa( 127UL ), CVUa( 127UL ) );
      RUN_DVECDVECOUTER_OPERATION_TEST( CVHa(  64UL ), CVUa(  64UL ) );
      RUN_DVECDVECOUTER_OPERATION_TEST( CVHa(  64UL ), CVUa( 128UL ) );
      RUN_DVECDVECOUTER_OPERATION_TEST( CVHa( 128UL ), CVUa(  64UL ) );
      RUN_DVECDVECOUTER_OPERATION_TEST( CVHa( 128UL ), CVUa( 128UL ) );
   }
   catch( std::exception& ex ) {
      std::cerr << "\n\n ERROR DETECTED during dense vector/dense vector outer product:\n"
                << ex.what() << "\n";
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
//*************************************************************************************************
