// std::__median<std::pair<CCharString,C3DMeshVolume>, CKeyPairCompareLess<CCharString,C3DMeshVolume> > (STLport 4.6) -- median-of-three for the introsort pivot of the name-keyed mesh-volume table (inline CCharString::operator< on .first); returns a reference, never swaps
// Leaf: ??$__median@U?$pair@VCCharString@@VC3DMeshVolume@@@std@@V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@std@@YIABU?$pair@VCCharString@@VC3DMeshVolume@@@0@ABU10@00V?$CKeyPairCompareLess@VCCharString@@VC3DMeshVolume@@@@@Z
// Flags: /Gr /Oa
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70020, 193 bytes, `ret 8` (__fastcall: a in ecx, b in edx, c and the empty
//   comparator's slot on the stack); the chosen element's address in eax. Callee: CBasicString<char>::operator< @ 0x00429950.
//   /Oa (assume no aliasing): the a/b/c string-data pointers stay in edi/esi/ebx across the data-compare calls.
// Identity: the single retail caller is __introsort_loop 0x00A73A00 (call at 0x00A73A52) over pair<CCharString, C3DMeshVolume> (NF87's
//   __partial_sort 0x00A733B0 / __pop_heap 0x00A71BE0 / __make_heap 0x00A71D40 / sort_heap 0x00A72B10 spell that element), so this out-of-line
//   COMDAT is that instance's __median (retail folds no identical code: EGOD_FINDINGS). The old row's pair<CCharString, CCharString> element
//   (catalogued as Dinkumware _Med3) was a stand-in: the comparator reads only .first, so the bytes are the same.
// NF89 (2026-11-17, q11176): STLport 4.6 _algo.h __median of stlport/fable_containers.h FABLE_RETAIL_STLP_INTROSORT_LOOP by explicit
//   instantiation on the shared headers. STLport instance name: descriptive-by-library.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_INTROSORT_LOOP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CVectorMap.h"
template const std::pair<CCharString,C3DMeshVolume>& std::__median(const std::pair<CCharString,C3DMeshVolume>&,const std::pair<CCharString,C3DMeshVolume>&,const std::pair<CCharString,C3DMeshVolume>&,CKeyPairCompareLess<CCharString,C3DMeshVolume>);
