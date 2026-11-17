// std::__unguarded_partition<std::pair<CCharString,unsigned long>*, CKeyPairCompareLess<CCharString,unsigned long>> (STLport 4.6, element std::pair<CCharString,unsigned long>) -- quicksort partition over (name, value) entries: advance first past smaller names, retreat last past larger ones, swap the pair, repeat until they cross
// Flags: /Gr
// Leaf: ??$__unguarded_partition@PAU?$pair@VCCharString@@K@std@@U12@V?$CKeyPairCompareLess@VCCharString@@K@@@std@@YIPAU?$pair@VCCharString@@K@0@PAU10@0U10@V?$CKeyPairCompareLess@VCCharString@@K@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B5800, 170 bytes, `ret 0Ch`: `__fastcall` (/Gr algorithm TU: first in ecx, last in
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __unguarded_partition body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,unsigned long>, comparator CKeyPairCompareLess<CCharString,unsigned long>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CVectorMap.h"
template std::pair<CCharString,unsigned long>* FABLE_STLP_FASTCALL std::__unguarded_partition(std::pair<CCharString,unsigned long>*,std::pair<CCharString,unsigned long>*,std::pair<CCharString,unsigned long>,CKeyPairCompareLess<CCharString,unsigned long>);
