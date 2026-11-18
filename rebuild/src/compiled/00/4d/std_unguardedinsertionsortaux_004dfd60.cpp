// std::__unguarded_insertion_sort_aux(pair<CCharString, CTCBase::CRegisteredTCInfo>* first, .. last, pair<..>*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> comp) -- STLport 4.6 unguarded insertion sort over the thing-component registry
// Flags: /Gr
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@00V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DFD60, 42 bytes, `ret 8` (size-optimised TU: unaligned entry).
// Callees: the pair copy constructor 0x004D788D (thiscall, out of line in this /Os TU; catalogued as
//   `pair<CCharString,CCharString>`), __unguarded_linear_insert(last, val, comp) 0x004DD80B (`__fastcall`, the pair by
//   value and the comparator on the stack; catalogued as `std::set_Insert_Sorted`).
// Label correction: the catalog's `_Fill<CIntelligentPointer<CThing>*,...>` is propagated; the element is the 16-byte
//   pair<CCharString, CRegisteredTCInfo> of the registry CVectorMap (Ego_r.pdb CRegisteredTCInfo: AllocFunc +0,
//   DriverType +4, Name +8), the same element as 0x004DFCEA / 0x004E1C8A / 0x004E1D68 in this TU.
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx, the null `_Tp*` tag and the empty
//   comparator on the stack.
// Shape: `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` under `optimize("s")`: the
//   comparator is passed through with `push dword ptr [esp+10h]`, the by-value copy is built in the argument area by
//   the out-of-line pair copy constructor.
// NF82 (q11000): moved off the row-local CCharString / CRegisteredTCInfo / pair / CKeyPairCompareLess models onto the shared headers:
//   stlport/fable_containers.h (the new overlay opt-in FABLE_RETAIL_STLP_UNGUARDED_INSERTION_SORT_AUX_BODY: the STLport 4.6 _algo.c
//   body, __unguarded_linear_insert declared only), engine/CTCBase.h, engine/CVectorMap.h and fable_string.h; `// Flags: /Gr`. The
//   __unguarded_linear_insert call now binds 0x004DD80B's template spelling.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_UNGUARDED_INSERTION_SORT_AUX_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void std::__unguarded_insertion_sort_aux(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,
 std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>);
