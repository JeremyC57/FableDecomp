// std::__adjust_heap<std::pair<CCharString,CSoundBankLH*>*, CKeyPairCompareLess<CCharString,CSoundBankLH*>> (STLport 4.6, element std::pair<CCharString,CSoundBankLH*>) -- heap sift-down of string-keyed pairs, then _Push_heap @ 0x00A403F0
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@VCCharString@@PAVCSoundBankLH@@@std@@HU12@V?$CKeyPairCompareLess@VCCharString@@PAVCSoundBankLH@@@@@std@@YIXPAU?$pair@VCCharString@@PAVCSoundBankLH@@@0@HHU10@V?$CKeyPairCompareLess@VCCharString@@PAVCSoundBankLH@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A408E0, 184 bytes (+ int3 padding).
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __adjust_heap body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,CSoundBankLH*>, comparator CKeyPairCompareLess<CCharString,CSoundBankLH*>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
class CSoundBankLH;   // RTTI .?AVCSoundBankLH@@ (no header: declared only)
#include "engine/CVectorMap.h"
template void std::__adjust_heap(std::pair<CCharString,CSoundBankLH*>*,int,int,std::pair<CCharString,CSoundBankLH*>,CKeyPairCompareLess<CCharString,CSoundBankLH*>);
