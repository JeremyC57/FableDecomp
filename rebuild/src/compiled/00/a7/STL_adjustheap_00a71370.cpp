// std::__adjust_heap<std::pair<CCharString,C3DMeshVolume>*, CKeyPairCompareLess<CCharString,C3DMeshVolume>> (STLport 4.6, element std::pair<CCharString,C3DMeshVolume>) -- STLport heap sift-down of the (name, named convex mesh volume) table by name, then __push_heap val back up
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@VCCharString@@VC3DMeshVolume@@@std@@HU12@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DMeshVolume@@@0@HHU10@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A71370, 377 bytes (+ int3 padding), `ret 18h` (__fastcall: first in ecx,
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __adjust_heap body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,C3DMeshVolume>, comparator CKeyPairCompareLess<CCharString,C3DMeshVolume>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template void std::__adjust_heap(std::pair<CCharString,C3DMeshVolume>*,int,int,std::pair<CCharString,C3DMeshVolume>,CKeyPairCompareLess<CCharString,C3DMeshVolume>);
