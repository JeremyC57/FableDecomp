// std::vector<const std::pair<unsigned long, CAnimationEntry>*>::_M_insert_overflow(pointer position, const value_type& x,
//   const __true_type&, size_type fillLength, bool atEnd) -- STLport POD growth insert: allocate size + max(size, fillLength)
//   elements, memmove the head, write fillLength copies of x, memmove the tail unless atEnd, free the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@PBU?$pair@KVCAnimationEntry@@@std@@V?$allocator@PBU?$pair@KVCAnimationEntry@@@std@@@2@@std@@IAEXPAPBU?$pair@KVCAnimationEntry@@@2@ABQBU32@ABU__true_type@2@I_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E1500, 216 bytes, thiscall, `ret 14h`. Callers: CAnimationSet::GetRandomAnim 0x005DCC00
//   (0x005DCC9C, push_back inline), CAnimationSet::GetRandomLoadedAnim 0x005DCD10 (0x005DCDB8, 0x005DCE0D) and this vector's
//   out-of-line push_back 0x005E1EF0 (0x005E1F1A).
// Identity (AUTH26, 2026-11-25; was the stand-in vector<CUnprovenElement_005e1500*>): the element is CVectorMap<unsigned long,
//   CAnimationEntry>::const_iterator, `const std::pair<unsigned long, CAnimationEntry>*` under STLport:
//   1. Ego_d.pdb: GetRandomAnim's found_anims and GetRandomLoadedAnim's found_anims / found_loaded_anims are type 0x3AEEC =
//      std::vector<std::_Vector_const_iterator<_Vector_val<pair<unsigned long,CAnimationEntry> > > >;
//   2. Ego_r.exe: both functions call only the _Vector_const_iterator<pair<unsigned long,CAnimationEntry>> vector instances
//      (reserve 0001:8337216), and the const CAnimationEntry* vector's overflow is a separate retail function (0x005E1420, called
//      by GetAnimsWithNameStartingWith 0x005DCB00 and push_back 0x005E1ECB). Retail has no ICF: one address, one instance.
// Callees: malloc 0x00BFEA0E (allocator::allocate), memmove 0x00BFEAE6 (__copy_trivial, head and tail), free 0x00BFEA14
//   (_M_clear's deallocate behind its null test).
// Shape: STLport 4.6 stl/_vector.c `_M_insert_overflow` (the POD __true_type overload) -- the shared header's out-of-line body
//   (FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE), the same body row 0x005DCC00 emits as its pick-any copy (retcmp EQ, relocs EQ).
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#include "stlport/fable_containers.h"
class CAnimationEntry;   // the element is a pointer to the pair: the complete type (engine/CAnimationEntry.h) is not needed

template void std::vector<const std::pair<unsigned long, CAnimationEntry>*>::_M_insert_overflow(const std::pair<unsigned long, CAnimationEntry>**,
    const std::pair<unsigned long, CAnimationEntry>* const&, const std::__true_type&, unsigned int, bool);
