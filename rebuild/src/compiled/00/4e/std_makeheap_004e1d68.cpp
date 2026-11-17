// std::__make_heap(pair<CCharString,CTCBase::CRegisteredTCInfo>* first, .. last, CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo> comp, pair<..>*, int*) -- STLport 4.6 _heap.c heap build over the thing-component registry
// Flags: /Gr
// Leaf: ??$__make_heap@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@U12@H@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@0PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1D68, 75 bytes, `ret 0Ch` (size-optimised TU: unaligned entry).
// Callees: the pair copy constructor 0x004D788D (thiscall, out of line in this /Os TU), __adjust_heap(first, parent,
//   len, val, comp) 0x004DFDBB (`__fastcall`; catalogued as `_Adjust_heap<pair<CCharString,CRegisteredTCInfo>*,...>`,
//   which names the element).
// Label correction: the catalog's `Heap_SiftDown` is descriptive; the body is STLport's `__make_heap` for the 16-byte
//   registry pair (see 0x004DFD60 in this TU).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx; the empty comparator, the null `_Tp*`
//   and `_Distance*` tags on the stack (`ret 0Ch`).
// Shape: the STLport text (`if (last - first < 2) return; len = last - first; parent = (len - 2) / 2;
//   while (true) { __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp); if (parent == 0) return;
//   parent--; }`) under `optimize("s")`: the comparator is passed through with `push dword ptr [esp+14h]`, the
//   by-value copy is built in the argument area by the out-of-line pair copy constructor, `first + parent` is
//   strength-reduced into edi.
// NF82 (q10998): moved off the row-local CCharString / CRegisteredTCInfo / pair / CKeyPairCompareLess models onto the shared headers,
//   as NF80's 0x004DFDBB (same instantiation): stlport/fable_containers.h (FABLE_RETAIL_STLP_HEAP + the new overlay opt-in
//   FABLE_RETAIL_STLP_MAKE_HEAP_BODY: the STLport 4.6 _heap.c __make_heap body, __adjust_heap declared only), engine/CTCBase.h,
//   engine/CVectorMap.h and fable_string.h; `// Flags: /Gr`. The __adjust_heap call now binds 0x004DFDBB's template spelling.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_MAKE_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void std::__make_heap(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,
 CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,int*);
