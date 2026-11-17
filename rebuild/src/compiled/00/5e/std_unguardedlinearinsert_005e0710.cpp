// std::__unguarded_linear_insert<CCharString*, std::less<CCharString>> (STLport 4.6, element CCharString) -- STLport insertion step: shift greater strings up one slot until val's place, then store it
// Flags: /Gr
// Leaf: ??$__unguarded_linear_insert@PAVCCharString@@V1@U?$less@VCCharString@@@std@@@std@@YIXPAVCCharString@@V1@U?$less@VCCharString@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0710, 76 bytes (+ padding to 80), `ret 8` (__fastcall /Gr algorithm TU: last in ecx ->
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __unguarded_linear_insert body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element CCharString, comparator std::less<CCharString>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#define FABLE_RETAIL_STLP_UNGUARDED_LINEAR_INSERT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__unguarded_linear_insert(CCharString*,CCharString,std::less<CCharString>);
