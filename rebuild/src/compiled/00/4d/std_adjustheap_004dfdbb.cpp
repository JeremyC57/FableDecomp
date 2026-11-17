// _global::__adjust_heap(pair<CCharString,CTCBase::CRegisteredTCInfo>*, long, long, pair<CCharString,CTCBase::CRegisteredTCInfo>, CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>) -- SGI heap sift-down of the registered-TC table entries by name, then __push_heap @ 0x004DD875
// Flags: /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@HU12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@HHU10@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DFDBB, 167 bytes (a /Os module: unaligned entry,
// ebp frame, loop test at the bottom; the SGI algorithm TUs are compiled /Gr, so first/hole ride in
// ecx/edx).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x004DA56A pair::operator= and 0x004D788D pair
// copy ctor (out of line in /Os; manifest labels them pair<CCharString,CCharString>), 0x0099EAE0
// ~CCharString, 0x004DD875 __push_heap (same instantiation).
// Element (16 bytes): CCharString key +0, CTCBase::CRegisteredTCInfo +4 (PDB: AllocFunc +0,
// DriverType +4, Name +8 -> element +0xc).  The by-value element is copy-built in the __push_heap
// slot (`sub esp,0x10 ; mov ecx,esp ; push &value`) and destroyed here by the implicit pair
// destructor (Name at value+0xc, then the key).  `ret 0x18` = bottom, the element, the comparator.
// Shape: SGI `__adjust_heap` (`2 * (secondChild + 1)`); the key compare is CCharString::operator<
// inline (same payload -> false, null this -> true, null other -> false, else payload compare).
// NF80 (q10923): moved onto the shared headers -- stlport/fable_containers.h (FABLE_RETAIL_STLP_DECAL_HEAP: STLport 4.6 _heap.c
//   __adjust_heap / __push_heap; the /Gr algorithm TU makes them __fastcall, the 0x00BEEA00 recipe), engine/CTCBase.h
//   (FABLE_RETAIL_CTCBASE_REGISTRY: CRegisteredTCInfo), engine/CVectorMap.h (CKeyPairCompareLess) and fable_string.h (the inline
//   CCharString::operator< over the one CBasicString<char>::operator< body). The local string / pair / comparator models are gone.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS
#define FABLE_RETAIL_CKEYPAIRCOMPARELESS_FORCEINLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void std::__adjust_heap(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,int,int,
 std::pair<CCharString,CTCBase::CRegisteredTCInfo>,CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>);
