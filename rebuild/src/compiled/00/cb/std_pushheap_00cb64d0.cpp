// std::__push_heap<std::pair<CCharString,CScriptInfo>*, CKeyPairCompareLess<CCharString,CScriptInfo>> (STLport 4.6, element std::pair<CCharString,CScriptInfo>) -- heap sift-up of the script registry's (name, script info) pairs
// Flags: /Gr
// Leaf: ??$__push_heap@PAU?$pair@VCCharString@@VCScriptInfo@@@std@@HU12@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@std@@YIXPAU?$pair@VCCharString@@VCScriptInfo@@@0@HHU10@V?$CKeyPairCompareLess@VCCharString@@VCScriptInfo@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB64D0, 223 bytes, `ret 0x20`.
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __push_heap body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,CScriptInfo>, comparator CKeyPairCompareLess<CCharString,CScriptInfo>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CSCRIPTINFO_ACCESSORS
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CScriptInfo.h"
#include "engine/CVectorMap.h"
template void std::__push_heap(std::pair<CCharString,CScriptInfo>*,int,int,std::pair<CCharString,CScriptInfo>,CKeyPairCompareLess<CCharString,CScriptInfo>);
