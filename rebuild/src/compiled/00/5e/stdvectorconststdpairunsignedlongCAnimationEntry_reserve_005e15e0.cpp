// std::vector<const std::pair<unsigned long, CAnimationEntry>*>::reserve(unsigned int n) -- grow the capacity to n, moving the
//   elements into a fresh block (the vector of CVectorMap<unsigned long, CAnimationEntry>::const_iterator that
//   CAnimationSet::GetRandomAnim / GetRandomLoadedAnim collect their candidate animations in)
// Leaf: ?reserve@?$vector@PBU?$pair@KVCAnimationEntry@@@std@@V?$allocator@PBU?$pair@KVCAnimationEntry@@@std@@@2@@std@@QAEXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E15E0, 179 bytes, thiscall, `ret 4`. Callers: CAnimationSet::GetRandomAnim 0x005DCC00
//   (0x005DCC24) and CAnimationSet::GetRandomLoadedAnim 0x005DCD10 (0x005DCD2D, 0x005DCD44), each `found_anims.reserve(5)`.
// Identity (AUTH26, 2026-11-25; was spelled vector<const CAnimationEntry*>::reserve, a wrong element type):
//   1. Ego_d.pdb: both callers declare found_anims (and GetRandomLoadedAnim found_loaded_anims) as type 0x3AEEC =
//      std::vector<std::_Vector_const_iterator<_Vector_val<pair<unsigned long,CAnimationEntry> > > >, CVectorMap's const_iterator,
//      which is `const std::pair<unsigned long, CAnimationEntry>*` under STLport;
//   2. Ego_r.exe: GetRandomAnim (0001:2340368) and GetRandomLoadedAnim (0001:2340704) call only the _Vector_const_iterator<pair<
//      unsigned long,CAnimationEntry>> instance of reserve (0001:8337216), never vector<const CAnimationEntry*>::reserve
//      (0001:7493920). The const CAnimationEntry* vector is GetAnimsWithNameStartingWith's (its overflow is 0x005E1420).
// Callees: malloc 0x00BFEA0E (allocator::allocate, both arms), memmove 0x00BFEAE6 (__copy_trivial), free 0x00BFEA14 (_M_clear).
// Shape: STLport 4.6 stl/_vector.c `reserve` verbatim (capacity() < n, the old size taken inside, the copy through
//   _M_allocate_and_copy inlined: the new block spilled to the `push ecx` word) -- the shared header's
//   FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY body (owner set of landed 0x0077E580), the same body row 0x005DCC00 emits
//   as its pick-any copy (retcmp EQ, relocs EQ).
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_STLP_VECTOR_RESERVE_ALLOCATE_AND_COPY
#define FABLE_RETAIL_STLP_VECTOR_C
#include "stlport/fable_containers.h"
class CAnimationEntry;   // the element is a pointer to the pair: the complete type (engine/CAnimationEntry.h) is not needed

template void std::vector<const std::pair<unsigned long, CAnimationEntry>*>::reserve(unsigned int);
