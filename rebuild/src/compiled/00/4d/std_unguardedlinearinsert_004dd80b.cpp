// std::__unguarded_linear_insert(pair<CCharString, CTCBase::CRegisteredTCInfo>* last, pair<CCharString, CTCBase::CRegisteredTCInfo> val, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> comp) -- STLport 4.6 insertion-sort inner loop of the registered-TC table sort
// Flags: /Gr
// Leaf: ??$__unguarded_linear_insert@PAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredTCInfo@CTCBase@@@0@U10@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredTCInfo@CTCBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD80B (unaligned: a size-optimised TU), 85 bytes, `ret 14h` (/Gr algorithm TU,
//   explicit __fastcall here: last in ecx, the 16-byte by-value pair and the empty comparator slot on the stack; the
//   callee destroys both strings of the by-value pair).
// Callers: call sites 0x004DFD79 and 0x004E1CE1 (the same size-optimised TU; by shape the insertion-sort callers).
// Callees: CBasicString<char>::operator< 0x00429950 (landed; the real body sits in this TU under auto_inline(off)),
//   pair<CCharString, CTCBase::CRegisteredTCInfo>::operator= 0x004DA56A (implicit, out of line: it calls
//   CCharString::operator= and CRegisteredTCInfo::operator= 0x004D78C5), CCharString::~CCharString 0x0099EAE0 (twice).
// Identity: CTCBase's CVectorMap<CCharString, CTCBase::CRegisteredTCInfo, CKeyPairCompareLess<...> > (Ego_r.pdb names the
//   map; CRegisteredTCInfo is {AllocFunc, DriverType, Name}, 12 bytes). The catalog's "std::set_Insert_Sorted" is
//   descriptive; the manifest labels on 0x004DA56A / 0x004D78C5 are propagated from other instantiations.
// Shape: `#pragma optimize("s", on)`; retail's compare is the inline CCharString::operator< with no frame, which under /Os needs the
//   FABLE_FORCEINLINE opt-ins on CCharString::operator< and CKeyPairCompareLess::operator() (NF80's 0x004E1C8A / 0x004DFDBB set).
// NF82 (q11000 / q11007): moved off the row-local CBasicString (with its own copy of the operator< body) / CCharString / CTCBase /
//   pair / CKeyPairCompareLess models onto the shared headers: stlport/fable_containers.h (FABLE_RETAIL_STLP_SORT_INTERNALS + the new
//   overlay opt-in FABLE_RETAIL_STLP_UNGUARDED_LINEAR_INSERT_BODY, explicit instantiation), engine/CTCBase.h, engine/CVectorMap.h and
//   fable_string.h (CBASICSTRING_LESS_BODY); `// Flags: /Gr` (the old row spelled __fastcall by hand). The own symbol is now the full
//   template instance NF80's 0x004E1C8A binds (the old Leaf line was partial).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_LESS
#define FABLE_RETAIL_CKEYPAIRCOMPARELESS_FORCEINLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_SORT_INTERNALS
#define FABLE_RETAIL_STLP_UNGUARDED_LINEAR_INSERT_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__unguarded_linear_insert(std::pair<CCharString, CTCBase::CRegisteredTCInfo>*,
                                                                 std::pair<CCharString, CTCBase::CRegisteredTCInfo>,
                                                                 CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo>);
