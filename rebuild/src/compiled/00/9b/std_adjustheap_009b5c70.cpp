// std::__adjust_heap<std::pair<CCharString,unsigned long>*, CKeyPairCompareLess<CCharString,unsigned long>> (STLport 4.6, element std::pair<CCharString,unsigned long>) -- heap sift-down of string-keyed pairs, then _Push_heap @ 0x009B4940
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@VCCharString@@K@std@@HU12@V?$CKeyPairCompareLess@VCCharString@@K@@@std@@YIXPAU?$pair@VCCharString@@K@0@HHU10@V?$CKeyPairCompareLess@VCCharString@@K@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B5C70, 184 bytes (+ int3 padding).
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __adjust_heap body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,unsigned long>, comparator CKeyPairCompareLess<CCharString,unsigned long>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CVectorMap.h"
template void std::__adjust_heap(std::pair<CCharString,unsigned long>*,int,int,std::pair<CCharString,unsigned long>,CKeyPairCompareLess<CCharString,unsigned long>);
