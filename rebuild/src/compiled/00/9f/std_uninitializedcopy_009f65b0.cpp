// std::__uninitialized_copy<const CInputEvent*, CInputEvent*>(first, last, result, const __false_type&) -- STLport 4.6: copy-construct [first, last) into raw storage
// Leaf: ??$__uninitialized_copy@PBVCInputEvent@@PAV1@@std@@YIPAVCInputEvent@@PBV1@0PAV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F65B0, `ret 8` (__fastcall: first in ecx, last in edx; result + the tag on the stack).
// NF72 (round 7, q10876): was the descriptive stand-in UninitCopy_CInputEvent_009f65b0 (catalog _global::). Descriptive-by-library: retail is STLport 4.6 (both PDBs are Dinkumware); its only retail caller,
//   vector<CInputEvent>::operator= 0x009F6890 (NF68 port on stlport/fable_containers.h), binds this symbol. Re-authored as the explicit
//   instantiation of the fable_containers.h body (FABLE_STLP_OUTOFLINE_UNINIT_COPY; FABLE_RETAIL_CINPUTEVENT_COPY declares the out-of-line copy ctor 0x009E4850) on engine/CInputEvent.h, with the macro block of 0x009F6890; the local
//   CInputEvent / tag / allocator models are gone.
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL
#define FABLE_RETAIL_STLP_COPY_PTRS_FALSE_NOINLINE
#define FABLE_RETAIL_STLP_DESTROY_RANGE_NOINLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_RETAIL_CINPUTEVENT_COPY
#include "stlport/fable_containers.h"
#include "engine/CInputEvent.h"

template CInputEvent* __fastcall std::__uninitialized_copy<const CInputEvent*, CInputEvent*>(const CInputEvent*, const CInputEvent*, CInputEvent*, const std::__false_type&);
