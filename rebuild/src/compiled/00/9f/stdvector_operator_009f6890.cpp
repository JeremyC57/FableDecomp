// std::vector<CInputEvent>::operator=(const vector& x) -- STLport 4.6 copy assignment: if x is not *this, grow into a fresh block (allocate-and-copy, then _M_clear the old one) when x is larger than the capacity, else assign over the live elements and destroy the surplus or copy-construct the tail; size becomes x.size()
// Leaf: ??4?$vector@VCInputEvent@@V?$allocator@VCInputEvent@@@std@@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6890, 288 bytes (incl. int3 padding), thiscall, `ret 4`; caller 0x009F54EF.
// Identity: STLport 4.6 stl/_vector.c operator= (descriptive-by-library: retail is STLport, both PDBs are Dinkumware builds) over
//   CInputEvent (52 bytes: Ego_r.pdb sizeof, engine/CInputEvent.h asserts 0x34; its empty destructor 0x00A03B30 is out of line). The
//   catalog label "CCombatWheel::operator=" was propagated; the old row called the type CElement34 / CElement34Vector.
// Port (NF68, round 7): the local CElement34 / CElement34Vector / __false_type stand-ins are replaced by engine/CInputEvent.h and
//   stlport/fable_containers.h's _vector.c operator= (FABLE_RETAIL_STLP_VECTOR_C). The five helpers retail keeps out of line (callees
//   below) come from the header: __uninitialized_copy (FABLE_STLP_OUTOFLINE_UNINIT_COPY) and the NF68 overlay opt-ins
//   FABLE_RETAIL_STLP_COPY_PTRS_FALSE_NOINLINE, FABLE_RETAIL_STLP_DESTROY_RANGE_NOINLINE (same-TU noinline __fastcall bodies) and
//   FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL / FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL (protected members, declaration only).
// Callees: _M_allocate_and_copy<const CInputEvent*> 0x009F65E0 (thiscall), _M_clear 0x009F6850 (thiscall), __copy_ptrs(first, last,
//   result, const __false_type&) 0x009F63B0, _Destroy(first, last) 0x009F6720 and __uninitialized_copy 0x009F65B0 (__fastcall).
// Shape: VC7.1 tail-duplicates the final `_M_finish = _M_start + n; return *this;` into every branch; the empty tag objects live in
//   the dead argument slot.
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_VECTOR_ALLOCATE_COPY_DECL
#define FABLE_RETAIL_STLP_VECTOR_CLEAR_DECL
#define FABLE_RETAIL_STLP_COPY_PTRS_FALSE_NOINLINE
#define FABLE_RETAIL_STLP_DESTROY_RANGE_NOINLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#include "stlport/fable_containers.h"
#include "engine/CInputEvent.h"

template std::vector<CInputEvent>& std::vector<CInputEvent>::operator=(const std::vector<CInputEvent>&);
