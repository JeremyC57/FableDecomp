// std::__partial_sort<std::pair<CCharString,C3DVector>*, CKeyPairCompareLess<CCharString,C3DVector>> (STLport 4.6, element std::pair<CCharString,C3DVector>) -- STLport partial_sort core over 16-byte (CCharString, C3DVector) pairs ordered by name: heapify [first, middle), pop every smaller later entry into the heap, then sort the heap
// Flags: /Gr
// Leaf: ??$__partial_sort@PAU?$pair@VCCharString@@VC3DVector@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DVector@@@0@000V?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F2C0, 135 bytes, `ret 0Ch` (__fastcall /Gr algorithm TU: first in
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __partial_sort body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,C3DVector>, comparator CKeyPairCompareLess<CCharString,C3DVector>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_INLINE_MAKE_HEAP
#define FABLE_RETAIL_STLP_PARTIAL_SORT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
template void std::__partial_sort(std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,CKeyPairCompareLess<CCharString,C3DVector>);
