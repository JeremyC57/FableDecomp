// std::__push_heap<std::pair<unsigned long,C3DMeshVolume>*, int, std::pair<unsigned long,C3DMeshVolume>, CKeyPairCompareLess<unsigned long,C3DMeshVolume>> (STLport 4.6) -- SGI push_heap core: sift `value` up from holeIndex towards topIndex in a heap of (unsigned long key, C3DMeshVolume) pairs
// Flags: /Gr
// Leaf: ??$__push_heap@PAU?$pair@KVC3DMeshVolume@@@std@@HU12@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@std@@YIXPAU?$pair@KVC3DMeshVolume@@@0@HHU10@V?$CKeyPairCompareLess@KVC3DMeshVolume@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A333A0, 258 bytes (`__fastcall`: first in ecx, holeIndex in edx; topIndex, the 0x10-byte value
//   and the comparator on the stack).
// Identity (NF85, q11096): its only retail caller is std::__adjust_heap 0x00A33A70, landed as the std::pair<unsigned long,C3DMeshVolume> instance and binding this
//   exact spelling at +0x10B; the element is the same TU's (0x00A34420 __pop_heap and 0x00A33A70 __adjust_heap). The value's C3DMeshVolume::operator= is 0x00A32960
//   (q11096: ??4C3DMeshVolume@@QAEAAV0@ABV0@@Z). The old row's Pair_00a333a0 / Named_00a333a0 / CountedPointer_00a333a0 were stand-ins.
// Callees: CCharString::operator= 0x0099EFB0 and operator delete 0x00BFE9BC (C3DMeshVolume's assignment expanded in the sift loop),
//   C3DMeshVolume::operator= 0x00A32960 (the final store), ~CCharString 0x0099EAE0 (the by-value value's destructor).
// NF85: moved onto stlport/fable_containers.h (FABLE_RETAIL_STLP_DECAL_HEAP: STLport 4.6 _heap.c __push_heap, explicit instantiation;
//   `/Gr` as the algorithm TU), engine/C3DMeshVolume.h, engine/CVectorMap.h (CKeyPairCompareLess) and fable_string.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void std::__push_heap(std::pair<unsigned long,C3DMeshVolume>*,int,int,std::pair<unsigned long,C3DMeshVolume>,CKeyPairCompareLess<unsigned long,C3DMeshVolume>);
