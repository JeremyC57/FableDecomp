// std::__insertion_sort(pair<ETCInterfaceType, CTCBase*>* first, .. last, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> comp) -- STLport 4.6 guarded insertion sort of a thing's component map: __linear_insert(first, i, T(*i), comp) for every element after the first
// Leaf: ??$__insertion_sort@PAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@std@@YIXPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@0@0V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CD330, 62 bytes; __fastcall (first in ecx, last in edx, the 4-byte comparator on the stack,
//   `ret 4`). Retail bytes: push ebx ; push ebp ; push esi ; push edi ; mov edi,edx ; mov ebx,ecx ; cmp ebx,edi ; je E ; lea esi,[ebx+8] ;
//   cmp esi,edi ; je E ; L: (push comp ; the 8-byte copy of *i in the argument area) ; mov edx,esi ; mov ecx,ebx ; call __linear_insert ;
//   add esi,8 ; cmp esi,edi ; jne L ; E: ... ret 4.
// Callee: std::__linear_insert<pair<ETCInterfaceType, CTCBase*>*, pair<..>, CKeyPairCompareLess<..> > @ 0x004CD0C0 (landed, __fastcall).
// Name fix (agentNF94, q11243; descriptive-by-library: STLport instance, in neither PDB): the re-authored std::sort 0x004CD5D0 (NF92) calls this
//   address twice as ??$__insertion_sort@PAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@V?$CKeyPairCompareLess@...@@@std@@YIX...@Z, and the body
//   is STLport's __insertion_sort (above). The old row was the generated placeholder `InsertionSort_SElement8_004cd330` on a local SElement8 /
//   SCompare model calling a stand-in `LinearInsert_004cd0c0`; it is now the explicit instantiation over stlport/fable_containers.h
//   (FABLE_RETAIL_STLP_INSERTION_SORT_BODY, the NF82 0x004E3686 recipe) and binds 0x004CD0C0 under its own landed spelling.
#define FABLE_RETAIL_STLP_INSERTION_SORT_BODY
#include "engine/ETCInterfaceType.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorMap.h"
class CTCBase;
template void __fastcall std::__insertion_sort<std::pair<ETCInterfaceType, CTCBase*>*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >(
    std::pair<ETCInterfaceType, CTCBase*>*, std::pair<ETCInterfaceType, CTCBase*>*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*>);
