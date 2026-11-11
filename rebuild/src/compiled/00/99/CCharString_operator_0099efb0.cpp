// CCharString::operator=(const CCharString& other) -- share the other string's data block (its reference count bumped), releasing ours first; self-assignment is a no-op
// Leaf: ??4CCharString@@QAEABV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099EFB0, 42 bytes (`ret 4`).
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ??4CCharString@@QAEABV0@ABV0@@Z (fable_string.h declares it so). Was the
//   __fastcall stand-in CCharString_operator_assign(CCharString*, void*, CCharString*) over a local CCharString model.
// Callees: CCharString::UnassignString() 0x0099E9B0 (protected, out of line). Layout: the retail fable_string.h branch
//   (FABLE_RETAIL_CCHARSTRING): CCharStringData { CBasicString<char> Data; long NoRefs; } packed, NoRefs +0x0D; NoRefs is protected
//   there, so the header opt-in FABLE_RETAIL_CCHARSTRINGDATA_FRIEND makes CCharString a friend.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRINGDATA_FRIEND
#include "fable_string.h"

const CCharString& CCharString::operator=(const CCharString& other)
{
    if (&other != this) {
        if (PStringData)
            UnassignString();
        CCharStringData* data = other.PStringData;
        if (data) {
            PStringData = data;
            ++data->NoRefs;
        }
    }
    return *this;
}
