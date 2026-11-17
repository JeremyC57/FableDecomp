// CTCBase::GetPTCInfo(const CCharString& name) -- look a TC class name up in the registered-TC map: the registration info, or null when the name is not registered
// Leaf: ?GetPTCInfo@CTCBase@@SIPBVCRegisteredTCInfo@1@ABVCCharString@@@Z
//
// Retail 0x00686F60, 62 bytes (+ int3 padding to 64), plain `ret` (static __fastcall: the name in ecx).
// Callees: CVectorMap<CCharString, CTCBase::CRegisteredTCInfo>::LowerBound @ 0x006870C0 on the global map @ 0x013BA89C
//   (end word @ 0x013BA8A0; the landed CTCBase::Clone 0x00686FA0 uses the same pair), CBasicString<char>::operator< @
//   0x00429950 (the inline CCharString compare's last step).
// Identity: Ego_r.pdb ?GetPTCInfo@CTCBase@@SIPBVCRegisteredTCInfo@1@ABVCCharString@@@Z (static, __fastcall); nine
//   callers in CThingComponentSet (0x0087033B..0x0087075A; landed CThingComponentSet::Add 0x00870750 names it).  The
//   catalog's "CVectorMap_FindMeshVolume" is propagated.
// Layout: Ego_r.pdb CTCBase::CRegisteredTCInfo (12 bytes) = { AllocFunc; ulong DriverType; CCharString Name; }, so the
//   map's pair<CCharString, CRegisteredTCInfo> is 16 bytes and the info sits at +4.
// Shape: `it = LowerBound(name); if (it == end() || name < it->first) return 0; return &it->second;` with the inline CCharString operator< (same pointer: not less; null name: less; null key: not less;
//   else the string compare).
// NF80 (q10923): moved onto engine/CTCBase.h (FABLE_RETAIL_CTCBASE_REGISTRY: CRegisteredTCInfo and the static RegisteredTCs
//   CVectorMap at 0x013BA89C; the overlay's FABLE_RETAIL_CTCBASE_REGISTRY_LOOKUP declares AllocateTC / GetPTCInfo / Clone) and fable_string.h (the inline
//   CCharString::operator< over the one CBasicString<char>::operator< body, FABLE_RETAIL_CBASICSTRING_LESS_BODY); the local
//   CBasicString / CCharString / CTCBase / pair / CRegisteredTCMap models and the made-up g_RegisteredTCs are gone.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CTCBASE_REGISTRY_LOOKUP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#include "fable_string.h"
#include "engine/CTCBase.h"

const CTCBase::CRegisteredTCInfo* __fastcall CTCBase::GetPTCInfo(const CCharString& name)
{
    CVectorMap<CCharString, CRegisteredTCInfo>::iterator it = RegisteredTCs.LowerBound(name);
    if (it == RegisteredTCs.end() || name < it->first)
        return 0;
    return &it->second;
}
