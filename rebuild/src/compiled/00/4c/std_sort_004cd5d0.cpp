// std::sort<pair<ETCInterfaceType, CTCBase*>*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >(first, last, comp) -- STLport 4.6 _algo.c:
//   for a non-empty range the introsort loop (0x004CD500) to depth 2*lg(n), then the final insertion sort (inlined: threshold 16 ->
//   __insertion_sort 0x004CD330 + __unguarded_insertion_sort_aux 0x004CBE80, else one __insertion_sort)
// Leaf: ??$sort@PAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@std@@YIXPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@0@0V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CD5D0, 115 bytes; __fastcall (first in ecx, last in edx, the 4-byte comparator on the stack).
// Identity (NF92, q11220): the __fastcall std::sort of CThing::Components (Ego_r.pdb CVectorMap<ETCInterfaceType, CTCBase*,
//   CKeyPairCompareLess<..> >): FableWin.pdb 0x01CFFAB0 ??$sort@..pair<ETCInterfaceType,CTCBase*>..CKeyPairCompareLess<ETCInterfaceType,CTCBase*>..
//   (Dinkumware iterator spelling in the debug build) + FableWin.pdb ?Sort@?$CVectorMap@W4ETCInterfaceType@@..@@QAEXXZ (0x004CD650 here);
//   the ported CThing::AddTC 0x004C9D60 binds this address under this spelling. Explicit instantiation of stlport/fable_containers.h (the
//   NF87 0x0077F440 recipe). Was the SGI placeholder _global::Sort_SElement8_004cd5d0 (SElement8 / SCompare models). Descriptive-by-library.
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_SORT_FASTCALL_BODY
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
class CTCBase;
template void __fastcall std::sort<std::pair<ETCInterfaceType, CTCBase*>*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >(
    std::pair<ETCInterfaceType, CTCBase*>*, std::pair<ETCInterfaceType, CTCBase*>*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>);
