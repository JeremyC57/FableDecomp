// std::__introsort_loop<std::pair<CCharString,C3DVector>*, CKeyPairCompareLess<CCharString,C3DVector>> (STLport 4.6, element std::pair<CCharString,C3DVector>) -- STLport introsort recursion over 16-byte (CCharString, C3DVector) pairs ordered by name: median-of-three quicksort partitions down to 16 elements, heap sort once the depth budget is spent
// Flags: /Gr
// Leaf: ??$__introsort_loop@PAU?$pair@VCCharString@@VC3DVector@@@std@@U12@HV?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@std@@YIXPAU?$pair@VCCharString@@VC3DVector@@@0@00HV?$CKeyPairCompareLess@VCCharString@@VC3DVector@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F370, 200 bytes (+ int3 padding to 208), `ret 0Ch` (__fastcall /Gr
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __introsort_loop body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,C3DVector>, comparator CKeyPairCompareLess<CCharString,C3DVector>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_INTROSORT_LOOP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__introsort_loop(std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,std::pair<CCharString,C3DVector>*,int,CKeyPairCompareLess<CCharString,C3DVector>);
