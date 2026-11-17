// std::__insertion_sort(pair<CCharString,CTCBase::CRegisteredTCInfo>* first, .. last, CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo> comp) -- STLport 4.6 guarded insertion sort of the registered-TC table entries
// Flags: /Gr
// Leaf: ??$__insertion_sort@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3686, length 53 bytes (unaligned entry: a size-optimised TU).
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. Its caller 0x004E415D is
// STLport `__final_insertion_sort` (`and eax,-10h ; cmp eax,100h` = 16 elements of 16 bytes, then this function on the
// first 16 and the unguarded sort on the rest), so this is `__insertion_sort(first, last, comp)`: return on an empty
// range, then `__linear_insert(first, i, _Tp(*i), comp)` @ 0x004E1C8A for i = first+1 .. last (`ret 14h` = 16-byte
// value + comparator). The value type's copy constructor is out of line @ 0x004D788D (it copies a CCharString and then
// a 12-byte member through its own copy constructor 0x004D6C66), the element is pair<CCharString, CTCBase::CRegisteredTCInfo> (0x004DFDBB).
// Convention: LTCG register form -- first in ecx, last in edx, the 4-byte empty comparator on the stack (`ret 4`).
// Callees: SStringEntry16::SStringEntry16(const SStringEntry16&) @ 0x004D788D, std::__linear_insert @ 0x004E1C8A.
// Pragma: optimize("s") -- the size TU's `push dword ptr [esp+10h]` for the comparator and the loop entered by a jump
// to its bottom test (under /O2 the test is duplicated in front of the loop, 58 v 53 bytes).
// NF82 (q10999): moved off the row-local CCharString / SValue12 / SStringEntry16 / SStringEntry16Less models onto the shared headers,
//   as NF80's 0x004E1C8A (the same instantiation's __linear_insert): stlport/fable_containers.h (the new overlay opt-in
//   FABLE_RETAIL_STLP_INSERTION_SORT_BODY: the STLport 4.6 _algo.c body, __linear_insert declared only), engine/CTCBase.h,
//   engine/CVectorMap.h and fable_string.h; `// Flags: /Gr`. The __linear_insert call now binds 0x004E1C8A's template spelling.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_INSERTION_SORT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void std::__insertion_sort(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,
 CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>);
