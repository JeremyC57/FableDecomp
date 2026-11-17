// std::__unguarded_linear_insert<std::pair<CCharString,unsigned long>*, CKeyPairCompareLess<CCharString,unsigned long>> (STLport 4.6, element std::pair<CCharString,unsigned long>) -- insertion-sort inner step over (name, value) entries: shift entries with larger names up one slot until val's name fits, then assign val there
// Flags: /Gr
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@VCCharString@@K@std@@U12@V?$CKeyPairCompareLess@VCCharString@@K@@@std@@YIXPAU?$pair@VCCharString@@K@0@U10@V?$CKeyPairCompareLess@VCCharString@@K@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B47F0, 89 bytes, `ret 0Ch`: `__fastcall` (/Gr algorithm TU: last in ecx, the 8-byte
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __unguarded_linear_insert body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,unsigned long>, comparator CKeyPairCompareLess<CCharString,unsigned long>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#define FABLE_RETAIL_STLP_UNGUARDED_LINEAR_INSERT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__unguarded_linear_insert(std::pair<CCharString,unsigned long>*,std::pair<CCharString,unsigned long>,CKeyPairCompareLess<CCharString,unsigned long>);
