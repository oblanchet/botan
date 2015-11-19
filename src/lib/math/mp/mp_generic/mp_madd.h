/*
* Lowest Level MPI Algorithms
* (C) 1999-2008,2013 Jack Lloyd
*     2006 Luca Piccarreta
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#ifndef BOTAN_MP_WORD_MULADD_H__
#define BOTAN_MP_WORD_MULADD_H__

#include <botan/mp_types.h>

namespace Botan {

#if (BOTAN_MP_WORD_BITS == 8)
  typedef u16bit dword;
  #define BOTAN_HAS_MP_DWORD
#elif (BOTAN_MP_WORD_BITS == 16)
  typedef u32bit dword;
  #define BOTAN_HAS_MP_DWORD
#elif (BOTAN_MP_WORD_BITS == 32)
  typedef u64bit dword;
  #define BOTAN_HAS_MP_DWORD
#elif (BOTAN_MP_WORD_BITS == 64)

  #include <botan/mul128.h>

  #if defined(BOTAN_TARGET_HAS_NATIVE_UINT128)
    typedef uint128_t dword;
    #define BOTAN_HAS_MP_DWORD
  #endif
#else
  #error BOTAN_MP_WORD_BITS must be 8, 16, 32, or 64
#endif

/*
* Word Multiply/Add
*/
inline word word_madd2(word a, word b, word* c)
   {
#if defined(BOTAN_HAS_MP_DWORD)
   const dword s = static_cast<dword>(a) * b + *c;
   *c = static_cast<word>(s >> BOTAN_MP_WORD_BITS);
   return static_cast<word>(s);
#else
   static_assert(BOTAN_MP_WORD_BITS == 64, "Unexpected word size");

   word hi = 0, lo = 0;

   mul64x64_128(a, b, &lo, &hi);

   lo += *c;
   hi += (lo < *c); // carry?

   *c = hi;
   return lo;
#endif
   }

/*
* Word Multiply/Add
*/
inline word word_madd3(word a, word b, word c, word* d)
   {
#if defined(BOTAN_HAS_MP_DWORD)
   const dword s = static_cast<dword>(a) * b + c + *d;
   *d = static_cast<word>(s >> BOTAN_MP_WORD_BITS);
   return static_cast<word>(s);
#else
   static_assert(BOTAN_MP_WORD_BITS == 64, "Unexpected word size");

   word hi = 0, lo = 0;

   mul64x64_128(a, b, &lo, &hi);

   lo += c;
   hi += (lo < c); // carry?

   lo += *d;
   hi += (lo < *d); // carry?

   *d = hi;
   return lo;
#endif
   }

}

#endif
