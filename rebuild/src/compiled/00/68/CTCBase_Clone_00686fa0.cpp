// CTCBase::Clone() -- look this component's class name up in the registered-TC map, allocate a fresh component of that class for the same owner thing, copy this one into it and return it
// Leaf: ?Clone@CTCBase@@QAEPAV1@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686FA0, length 107 bytes.
// Callees: GetClassTypeName (virtual slot 22, +0x58, CCharString by hidden pointer into the frame word), the registered-TC
// CVectorMap<CCharString, CRegisteredTCInfo>::LowerBound @ 0x006870C0 on the global map @ 0x013BA89C (end word @ 0x013BA8A0;
// catalog names the byte-twin <CCharString,C3DMeshVolume> instantiation), CBasicString<char>::operator< @ 0x00429950 (the inline
// CCharString compare's last step), the info's `__fastcall` Alloc(CThing&) pointer (+4 of the pair), CCharString::~CCharString
// @ 0x0099EAE0, the clone's virtual slot 12 (+0x30, copy from this).
// Shape: the owner is read into a local (ebp) before the name call; the name is a temporary passed by reference into an inline
// static AllocTC(name, owner) (VC keeps the returned pointer in esi) and dies at the end of that statement, before the copy call
// (made without a null test). The found test is `it != end && !(name < it->first)` with the inline CCharString operator<
// written `o.PString == PString` first (key loaded before the name), then the two null tests.
// NF80 (q10923): moved onto engine/CTCBase.h (FABLE_RETAIL_CTCBASE_REGISTRY: CRegisteredTCInfo and the static RegisteredTCs
//   CVectorMap at 0x013BA89C; the overlay's FABLE_RETAIL_CTCBASE_REGISTRY_LOOKUP declares AllocateTC / GetPTCInfo / Clone, and its FABLE_RETAIL_CTCBASE_ALLOCATETC_BODY
//   gives the inline AllocateTC 0x00686F10 that retail expands here) and fable_string.h (the inline
//   CCharString::operator< over the one CBasicString<char>::operator< body, FABLE_RETAIL_CBASICSTRING_LESS_BODY); the local
//   CBasicString / CCharString / CTCBase / pair / CRegisteredTCMap models and the made-up g_RegisteredTCs are gone.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CTCBASE_REGISTRY_LOOKUP
#define FABLE_RETAIL_CTCBASE_ALLOCATETC_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#include "fable_string.h"
#include "engine/CTCBase.h"

CTCBase* CTCBase::Clone()
{
    CThing* owner = ParentThing;
    CTCBase* clone = AllocateTC(GetClassTypeName(), *owner);
    clone->Copy(*this);
    return clone;
}
