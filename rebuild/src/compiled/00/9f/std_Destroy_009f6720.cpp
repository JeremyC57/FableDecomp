// std::_Destroy<CInputEvent*>(first, last) -- STLport 4.6 range destroy: run ~CInputEvent over [first, last)
// Leaf: ??$_Destroy@PAVCInputEvent@@@std@@YIXPAVCInputEvent@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F6720, plain `ret` (__fastcall: first in ecx, last in edx).
// NF72 (round 7, q10874): was the descriptive stand-in _Destroy_Record34 (catalog _global::_Destroy_Record34). Descriptive-by-library: retail is STLport 4.6 (both PDBs are Dinkumware); its only retail caller,
//   vector<CInputEvent>::operator= 0x009F6890 (NF68 port on stlport/fable_containers.h), binds this symbol. Re-authored as the explicit
//   instantiation of the fable_containers.h body (FABLE_RETAIL_STLP_DESTROY_RANGE_NOINLINE) on engine/CInputEvent.h, with the macro block of 0x009F6890; the local
//   CInputEvent / tag / allocator models are gone.
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL
#define FABLE_RETAIL_STLP_COPY_PTRS_FALSE_NOINLINE
#define FABLE_RETAIL_STLP_DESTROY_RANGE_NOINLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "stlport/fable_containers.h"
#include "engine/CInputEvent.h"

template void __fastcall std::_Destroy<CInputEvent*>(CInputEvent*, CInputEvent*);
