// std::__partial_sort(pair<CCharString,CTCBase::CRegisteredTCInfo>* first, .. middle, .. last, pair<..>*, CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo> comp) -- STLport 4.6 partial sort over the thing-component registry (the introsort depth-limit fallback)
// Flags: /Gr
// Leaf: ??$__partial_sort@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@000V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6E9E, 104 bytes, `ret 0Ch` (size-optimised TU: unaligned entry,
//   frame pointer).
// Callees: make_heap(first, last, comp) 0x004E36C3 and sort_heap(first, last, comp) 0x004E41DD (`__fastcall`, the
//   inline STLport front ends out of line in this /Os TU; 0x004E41DD is catalogued as `_Sort_heap<...>`),
//   CBasicString<char>::operator< 0x00429950 (the inlined CCharString::operator<), the pair copy constructor
//   0x004D788D, __pop_heap(first, last, result, val, comp, ptrdiff_t*) 0x004E1D20 (`__fastcall`).
// Label correction: the catalog's `_Find<CCharString*,CCharString>` is propagated; the body is STLport's
//   `__partial_sort` (it is the introsort loop's depth-limit fallback for the 16-byte registry pair, see 0x004DFD60).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, middle in edx; last, the null `_Tp*` tag and the
//   empty comparator on the stack (`ret 0Ch`).
// Shape: the STLport text `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, _Tp(*i), comp, __DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` under
//   `optimize("s")` with the comparator and CCharString::operator< `__forceinline` (the landed 0x00AF2F10 inline);
//   the by-value copy is built in the argument area by the out-of-line pair copy constructor, the distance tag is a
//   null `push 0`; `*first`'s block pointer is re-read every pass (the heap top changes in __pop_heap).
// NF82 (q11007): moved off the row-local CBasicStringChar (the stand-in ??MCBasicStringChar@std@@QBE_NABU01@@Z compare) / CCharString /
//   CRegisteredTCInfo / pair / CKeyPairCompareLess models onto the shared headers, as NF80's 0x004DFDBB / 0x004E1C8A (same instantiation):
//   stlport/fable_containers.h (the new overlay opt-in FABLE_RETAIL_STLP_PARTIAL_SORT_BODY: the STLport 4.6 _algo.c body, make_heap /
//   sort_heap / __pop_heap declared only), engine/CTCBase.h, engine/CVectorMap.h (CKeyPairCompareLess, FORCEINLINE) and fable_string.h
//   (LESS_INLINE + FORCEINLINE_LESS + CBASICSTRING_LESS_BODY); `// Flags: /Gr`.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS
#define FABLE_RETAIL_CKEYPAIRCOMPARELESS_FORCEINLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_PARTIAL_SORT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void std::__partial_sort(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,
 std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>);
