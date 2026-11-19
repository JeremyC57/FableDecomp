// std::__adjust_heap(pair<unsigned long, CAnimationEntry>* first, int holeIndex, int len, pair<unsigned long, CAnimationEntry> val, CKeyPairCompareLess<unsigned long, CAnimationEntry> comp) -- STLport heap sift-down of the animation table (unsigned key) to a leaf, then __push_heap of val back up
// Leaf: ??$__adjust_heap@PAU?$pair@KVCAnimationEntry@@@std@@HU12@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@std@@YIXPAU?$pair@KVCAnimationEntry@@@0@HHU10@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E1CB0, 256 bytes, __fastcall (/Gr algorithm TU: first in ecx, holeIndex in edx; len, the
//   0x14-byte pair by value and the empty comparator on the stack), `ret 1Ch`.
// Identity: STLport 4.6 _heap.c __adjust_heap over the CVectorMap<unsigned long, CAnimationEntry> entries (Ego_r.pdb holds the
//   Dinkumware twin std::_Adjust_heap<pair<unsigned long,CAnimationEntry>*,..,CKeyPairCompareLess<..> >); same TU as the landed
//   0x005E1210 (pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >) instance.
// Source: the shared STLport body (fable_containers.h FABLE_RETAIL_STLP_DECAL_HEAP), explicit instantiation, with the landed 0x005E1210
//   counted-pointer opt-ins (CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN / RELEASE_MEMBER / CONVERT, CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22):
//   CAnimationEntry::Components is a CIVCountedPointer<CAnimComponentMap>, so pair's implicit operator= takes the same path. Those
//   opt-ins close the parked secondChild / source-pointer ebx<->edi swap as they did 0x005E1210's.
// Callees: __push_heap (same instantiation) 0x005E1310 (__fastcall), pair<unsigned long, CAnimationEntry>::operator= 0x005E0E20 (the
//   implicit assignment, out of line at the bottom case), the counted map's Release.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN_EARLY_RETURN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CAnimationEntry.h"
#include "engine/CVectorMap.h"
template void std::__adjust_heap(std::pair<unsigned long, CAnimationEntry>*, int, int, std::pair<unsigned long, CAnimationEntry>, CKeyPairCompareLess<unsigned long, CAnimationEntry>);