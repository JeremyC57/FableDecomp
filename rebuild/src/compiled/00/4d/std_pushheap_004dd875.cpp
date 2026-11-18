// std::__push_heap(pair<CCharString,CTCBase::CRegisteredTCInfo>*, int, int, pair<CCharString,CTCBase::CRegisteredTCInfo>, CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>) -- STLport 4.6 _heap.c heap sift-up of the registered-TC table entries by name
// Flags: /Gr
// Leaf: ??$__push_heap@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@HU12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@HHU10@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DD875, 138 bytes, `ret 0x18` (a /Os module: unaligned
// entry, ebp frame with `first` spilled to [ebp-4], `leave`; the SGI algorithm TUs are compiled /Gr,
// so first/hole ride in ecx/edx).
// Called by the landed __adjust_heap @ 0x004DFDBB (same instantiation; its model is reused here).
// Callees: 0x00429950 CBasicString<char>::operator<, 0x004DA56A pair::operator= (out of line in /Os;
// manifest labels it pair<CCharString,CCharString>), 0x0099EAE0 ~CCharString.
// Element (16 bytes): CCharString key +0, CTCBase::CRegisteredTCInfo +4 (PDB: AllocFunc +0,
// DriverType +4, Name +8 -> element +0xc).  The by-value element is destroyed at the end by the
// implicit pair destructor (Name at value+0xc, then the key).  `ret 0x18` = top, the 16-byte
// element and the comparator.  The key compare is CCharString::operator< inline (same payload ->
// false, null this -> true, null other -> false, else the payload compare); under /Os the value's
// key word is compared in memory (`cmp [ebp+0Ch],ecx`) rather than loaded.
// Shape: SGI `__push_heap` (`parent = (hole - 1) / 2; while (hole > top && comp(*(first + parent), value))`).
#include "fable_compiler.h"
// NF82 (q10997 / q11007): moved off the row-local CBasicStringChar / CCharString / CRegisteredTCInfo / pair / CKeyPairCompareLess models
//   onto the shared headers, as NF80's 0x004DFDBB (same instantiation): stlport/fable_containers.h (FABLE_RETAIL_STLP_DECAL_HEAP: the
//   STLport 4.6 _heap.c __push_heap body, explicit instantiation), engine/CTCBase.h (CTCBase::CRegisteredTCInfo), engine/CVectorMap.h
//   (CKeyPairCompareLess, FORCEINLINE) and fable_string.h (CBASICSTRING_LESS_BODY, FORCEINLINE_LESS). `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). The own symbol is now the template instance NF80's 0x004DFDBB binds, and the string
//   compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z (was the stand-in ??MCBasicStringChar@@QBE_NABU0@@Z).
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
template void std::__push_heap(std::pair<CCharString,CTCBase::CRegisteredTCInfo>*,int,int,
 std::pair<CCharString,CTCBase::CRegisteredTCInfo>,CKeyPairCompareLess<CCharString,CTCBase::CRegisteredTCInfo>);
