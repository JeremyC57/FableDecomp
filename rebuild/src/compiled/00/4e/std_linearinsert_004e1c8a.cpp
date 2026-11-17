// std::__linear_insert(pair<CCharString, CTCBase::CRegisteredTCInfo>* first, .. last, pair<..> val, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> comp) -- STLport insertion-sort step over the thing-component registry (CTCBase::RegisteredTCs, ordered by key)
// Flags: /Gr
// Leaf: ??$__linear_insert@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@0U10@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1C8A, 114 bytes, `ret 14h` (size-optimised TU: unaligned entry,
//   frame pointer).
// Callees: CBasicString<char>::operator< 0x00429950 (the inlined CCharString::operator<), __copy_backward(first, last,
//   result, const random_access_iterator_tag&, ptrdiff_t*) 0x004DA738 (`__fastcall`; catalogued as the propagated
//   `_Copy_backward<CArray<unsigned_short>*,...>`), the pair operator= 0x004DA56A and copy constructor 0x004D788D
//   (thiscall, out of line in this /Os TU), __unguarded_linear_insert(last, val, comp) 0x004DD80B (`__fastcall`),
//   ~CCharString 0x0099EAE0 (x2: the value's second.Name at +0xc, then its key).
// Label correction: the catalog's `Set_Insert_Element` is descriptive; the element is the 16-byte
//   pair<CCharString, CRegisteredTCInfo> of the registry CVectorMap (see 0x004DFD60 in this TU).
// The algorithm TUs are /Gr (link-time __fastcall): first in ecx, last in edx, the 16-byte value and the empty
//   comparator on the stack (16 + 4 = 0x14).
// Shape: `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` under `optimize("s")` with the comparator and CCharString::operator<
//   `__forceinline` (the landed 0x00AF2F10 inline: same block -> false, null this -> true, null other -> false, else
//   the payload compare); copy_backward's fresh `random_access_iterator_tag()` lands in the high byte of the
//   comparator's argument slot (`lea eax,[ebp+1Bh]`), the distance pointer is a null `push 0`.
// NF80 (q10923): moved onto the shared headers -- stlport/fable_containers.h (FABLE_RETAIL_STLP_SORT_INTERNALS: STLport 4.6 _algo.h
//   __linear_insert with the out-of-line __copy_backward and __unguarded_linear_insert, the 0x00808AA0 recipe), engine/CTCBase.h
//   (FABLE_RETAIL_CTCBASE_REGISTRY: CRegisteredTCInfo), engine/CVectorMap.h (CKeyPairCompareLess) and fable_string.h (CCharString::
//   operator< over the one CBasicString<char>::operator< body); the comparator and CCharString::operator< are forced inline per TU
//   (FABLE_RETAIL_CKEYPAIRCOMPARELESS_FORCEINLINE / FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS) as the old local models were.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS
#define FABLE_RETAIL_CKEYPAIRCOMPARELESS_FORCEINLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__linear_insert(std::pair<CCharString, CTCBase::CRegisteredTCInfo>*, std::pair<CCharString, CTCBase::CRegisteredTCInfo>*,
                                                       std::pair<CCharString, CTCBase::CRegisteredTCInfo>, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>);
