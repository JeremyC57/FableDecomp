// std::__pop_heap_aux<std::pair<CCharString,unsigned long>*, CKeyPairCompareLess<CCharString,unsigned long>> (STLport 4.6, element std::pair<CCharString,unsigned long>) -- SGI pop_heap front end for a heap of (name, unsigned long) pairs: move the back element out, put the root there and sift the old back element down
// Flags: /Gr
// Leaf: ??$__pop_heap_aux@PAU?$pair@VCCharString@@K@std@@U12@V?$CKeyPairCompareLess@VCCharString@@K@@@std@@YIXPAU?$pair@VCCharString@@K@0@00V?$CKeyPairCompareLess@VCCharString@@K@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B6D80, length 109 bytes, `ret 8` (the unused T* and the empty comparator on the
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __pop_heap_aux body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,unsigned long>, comparator CKeyPairCompareLess<CCharString,unsigned long>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_POP_HEAP_AUX_FASTCALL_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CVectorMap.h"
template void __fastcall std::__pop_heap_aux(std::pair<CCharString,unsigned long>*,std::pair<CCharString,unsigned long>*,std::pair<CCharString,unsigned long>*,CKeyPairCompareLess<CCharString,unsigned long>);
