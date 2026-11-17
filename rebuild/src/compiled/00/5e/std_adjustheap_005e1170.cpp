// std::__adjust_heap<CCharString*, std::less<CCharString>> (STLport 4.6, element CCharString) -- STLport heap sift-down of a CCharString hole to a leaf, then __push_heap of val back up
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAVCCharString@@HV1@U?$less@VCCharString@@@std@@@std@@YIXPAVCCharString@@HHV1@U?$less@VCCharString@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1170, 151 bytes (+ padding to 160), `ret 0Ch` (__fastcall /Gr algorithm TU: first in ecx ->
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __adjust_heap body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element CCharString, comparator std::less<CCharString>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CVectorMap.h"
template void std::__adjust_heap(CCharString*,int,int,CCharString,std::less<CCharString>);
