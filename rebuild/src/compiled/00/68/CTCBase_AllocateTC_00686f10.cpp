// CTCBase::AllocateTC(const CCharString& name, CThing& owner) -- create a thing component by its registered class name: the registered allocator's new component for the owner, or null when the name is not registered
// Leaf: ?AllocateTC@CTCBase@@SIPAV1@ABVCCharString@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00686F10, 69 bytes, static __fastcall (name in ecx, owner in edx), plain `ret`.
// Identity (proven): Ego_r.pdb donor order AllocateTC 0x006B2200 -> GetPTCInfo 0x006B2260 matches retail this row -> GetPTCInfo
//   0x00686F60 (landed); the signature ?AllocateTC@CTCBase@@SIPAV1@ABVCCharString@@AAVCThing@@@Z fits (ecx name, edx owner, the
//   allocator called with the owner); its only caller is CThing::AddTC 0x004C9D60 (0x004C9D7F). The catalog label
//   "CMeshVolumeMap::Find" is propagated.
// Callees and globals: exactly the landed GetPTCInfo 0x00686F60 model: LowerBound 0x006870C0 on the registered-TC map 0x013BA89C
//   (end word 0x013BA8A0), CBasicString<char>::operator< 0x00429950, then the info's AllocFunc (__fastcall, the owner in ecx).
//   The old landed spellings for 0x013BA89C/0x006870C0 were invented (`g_RegisteredTCs` of type CRegisteredTCMap); Ego_r.pdb has the
//   static member CTCBase::RegisteredTCs, a CVectorMap<CCharString, CRegisteredTCInfo, CKeyPairCompareLess<...> >, which this row now
//   uses through engine/CTCBase.h (see the NF80 note below).
// Shape: `it = LowerBound(name); if (it == end() || name < it->first) return 0; return it->second.AllocFunc(owner);`.
// NF80 (q10923): moved onto engine/CTCBase.h (FABLE_RETAIL_CTCBASE_REGISTRY: CRegisteredTCInfo and the static RegisteredTCs
//   CVectorMap at 0x013BA89C; the overlay's FABLE_RETAIL_CTCBASE_REGISTRY_LOOKUP declares AllocateTC / GetPTCInfo / Clone, and its FABLE_RETAIL_CTCBASE_ALLOCATETC_BODY
//   (NF80 added _DEFINE here; NF88 dropped it, below) holds this function's one definition, which Clone 0x00686FA0 expands) and fable_string.h (the inline
//   CCharString::operator< over the one CBasicString<char>::operator< body, FABLE_RETAIL_CBASICSTRING_LESS_BODY); the local
//   CBasicString / CCharString / CTCBase / pair / CRegisteredTCMap models and the made-up g_RegisteredTCs are gone.
// NF88 (round 7, q11162, coordinator ruling: one definition): the row no longer defines FABLE_RETAIL_CTCBASE_ALLOCATETC_DEFINE, which turned the
//   header's inline body into a strong (nodup) copy that clashed (LNK2005) with every includer's COMDAT (Clone 0x00686FA0). It includes the inline
//   body like every includer and makes VC7.1 emit it as a pick-any COMDAT (selection 2) through the anonymous-namespace reference below (the
//   0x00860150 pattern). The _DEFINE branch stays in the header, unused.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CTCBASE_REGISTRY_LOOKUP
#define FABLE_RETAIL_CTCBASE_ALLOCATETC_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#include "fable_string.h"
#include "engine/CTCBase.h"

// The body is the one inline definition in engine/CTCBase.h (FABLE_RETAIL_CTCBASE_ALLOCATETC_BODY).
namespace {
// Fidelity: scaffolding, not retail: takes the address of the header's inline AllocateTC so this object emits it out of line as a pick-any
// COMDAT (retail's 0x00686F10 is that COMDAT, called by CThing::AddTC 0x004C9D60). Internal to this TU; never called.
CTCBase* (__fastcall* ReferenceAllocateTC())(const CCharString&, CThing&)
{
    return &CTCBase::AllocateTC;
}
}
