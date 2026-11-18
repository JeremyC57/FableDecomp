// std::vector<CInputEvent>::_M_allocate_and_copy<const CInputEvent*>(unsigned int n, const CInputEvent* first, const CInputEvent* last) -- STLport 4.6: allocate n slots, copy-construct [first, last) into them, return the block
// Leaf: ??$_M_allocate_and_copy@PBVCInputEvent@@@?$vector@VCInputEvent@@V?$allocator@VCInputEvent@@@std@@@std@@IAEPAVCInputEvent@@IPBV2@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F65E0, 77 bytes, `ret 0Ch` (thiscall; malloc 0x00BFEA0E, CInputEvent copy ctor 0x009E4850; __uninitialized_copy expanded inline).
// NF72 (round 7, q10877): was the bare leaf _M_allocate_and_copy (catalog std::vector<CInputEvent>::_M_allocate_and_copy). Descriptive-by-library: retail is STLport 4.6 (both PDBs are Dinkumware); its only retail caller,
//   vector<CInputEvent>::operator= 0x009F6890 (NF68 port on stlport/fable_containers.h), binds this symbol. Re-authored as the explicit
//   instantiation of the fable_containers.h body (the in-class STLport body (FABLE_RETAIL_STLP_VECTOR_C with _ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22: protected, in-class)) on engine/CInputEvent.h, with the macro block of 0x009F6890; the local
//   CInputEvent / tag / allocator models are gone.
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_ORIGINAL_SCOPE_PASS22
#define FABLE_RETAIL_CINPUTEVENT_COPY
#include "stlport/fable_containers.h"
#include "engine/CInputEvent.h"

template CInputEvent* std::vector<CInputEvent>::_M_allocate_and_copy<const CInputEvent*>(unsigned int, const CInputEvent*, const CInputEvent*);
