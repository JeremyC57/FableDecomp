// CThing::AddAndCopyTC(const CTCBase& other, bool flag) -- add a component named like the given one, then copy the given component into this thing's component of its type (an absent type leaves the out pointer unset, as retail)
// Leaf: ?AddAndCopyTC@CThing@@QAEXABVCTCBase@@_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9F60, 166 bytes, `ret 8`; Ego_r.pdb ?AddAndCopyTC@CThing@@QAEXABVCTCBase@@_N@Z.
// Callees: CTCBase virtual GetClassTypeName (slot 22, +0x58, by hidden pointer) / GetInterfaceType (slot 21, +0x54) / Copy (slot 12, +0x30),
//   CThing::AddTC(const CCharString&, bool, const CTCParamsBase*) 0x004C9D60, ~CCharString 0x0099EAE0, the STLport bitset range check
//   (puts 0x00BFEAF2 "bitset" / abort 0x00BFEAEC), LowerBound 0x0040F020 (Components.Find inside the FableWin.pdb GetTC<CTCBase>(id, ptc)).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"

void CThing::AddAndCopyTC(const CTCBase& other, bool flag)
{
    AddTC(other.GetClassTypeName(), flag, 0);
    CTCBase* tc;
    GetTC(other.GetInterfaceType(), &tc);
    tc->Copy(other);
}