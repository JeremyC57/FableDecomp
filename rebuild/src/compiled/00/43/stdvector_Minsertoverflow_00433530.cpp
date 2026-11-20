// 00433530 vector<CCharString>::_M_insert_overflow (catalog: std::vector::Reallocate)
// Flags: /O2 /Oy /W3
// Leaf: ?_M_insert_overflow@?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@IAEXPAVCCharString@@ABV3@ABU__false_type@2@I_N@Z
// Layout: vector pointers +0,+4,+8; element stride and constructor fields from retail call and copy sequence.
// Name fix (NF199 draft, finished by NF204, 2026-11-20, q6332 / q6341): the local one-parameter vector<T> / CCharString / __false_type
//   model is replaced by the shared STLport 4.6 vector (stlport/fable_containers.h stl/_vector.c body) over fable_string.h; the
//   explicit instantiation emits the protected member with its full allocator argument, the spelling 27 of the 39 landed
//   referencers bind (NF204 probe; e.g. 0x004B2850, 0x004B5500). Retail expands all three non-POD __uninitialized_copy /
//   __uninitialized_fill_n loops in line (no call; 316 bytes): the existing NF41 opt-in FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
//   (row 0x00618420 precedent) gives that; plain inline measured DIFFER(280v316), this row's `this` kept in ebx instead of
//   retail's frame slot.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_UNINIT_COPY_FORCEINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
template void std::vector<CCharString>::_M_insert_overflow(CCharString*, const CCharString&, const std::__false_type&, unsigned int, bool);
