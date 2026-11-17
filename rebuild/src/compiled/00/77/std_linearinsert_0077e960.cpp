// std::__linear_insert<std::pair<CCharString,C3DVector>*, CKeyPairCompareLess<CCharString,C3DVector>> (STLport 4.6, element std::pair<CCharString,C3DVector>) -- STLport insertion-sort step over 16-byte (CCharString, C3DVector) pairs ordered by name: a value that sorts before the first element shifts [first, last) up one slot and is stored at *first, anything else goes through the unguarded insert
// Flags: /Gr
// Leaf: ??$__linear_insert@PAU?$pair@VCCharString@@VC3DVector@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DVector@@@0@0U10@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E960, 166 bytes (+ int3 padding to 176), `ret 14h` (__fastcall /Gr
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __linear_insert body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,C3DVector>, comparator CKeyPairCompareLess<CCharString,C3DVector>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__linear_insert(std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>,CKeyPairCompareLess<CCharString,C3DVector>);
