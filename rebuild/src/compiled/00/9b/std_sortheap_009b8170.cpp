// std::sort_heap<std::pair<CCharString,unsigned long>*, CKeyPairCompareLess<CCharString,unsigned long>> (STLport 4.6, element std::pair<CCharString,unsigned long>) -- Dinkumware _Sort_heap over string-keyed pairs: pop the root to the back until one element remains
// Flags: /Gr
// Leaf: ??$sort_heap@PAU?$pair@VCCharString@@K@std@@V?$CKeyPairCompareLess@VCCharString@@K@@@std@@YIXPAU?$pair@VCCharString@@K@0@0V?$CKeyPairCompareLess@VCCharString@@K@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B8170, length 169 bytes (+ int3 padding), `ret 4`.
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 sort_heap body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,unsigned long>, comparator CKeyPairCompareLess<CCharString,unsigned long>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CVectorMap.h"
template void __fastcall std::sort_heap(std::pair<CCharString,unsigned long>*,std::pair<CCharString,unsigned long>*,CKeyPairCompareLess<CCharString,unsigned long>);
