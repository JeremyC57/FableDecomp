// CAnimFilterOfGroup::SetCurrentGroupName(const CCharString& name) -- take the new group name and recompute its CRC
// Leaf: ?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073DFF0, 77 bytes (+3 int3), __fastcall `ret 4`.
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, Calc @ 0x004014A0 (`__fastcall(seed, data, length)`, the
//   engine's CRC-32 kernel; run over CCharString::NullChar 0x0129AAF4 when the name has no data block).
// Label: the manifest's "CRCHashedString_SetAndHash" is descriptive; FableWin.pdb names
//   `?SetCurrentGroupName@CAnimFilterOfGroup@@QAEXABVCCharString@@@Z` (0x01DD3450). Layout = Ego_r.pdb CAnimFilterOfGroup (12 bytes):
//   Group +4, GroupCRC +8 (as in the landed constructor 0x0073E8F0).
// Shape: `Group = name; GroupCRC = name.GetCRC();` with CCharString::GetCRC inline (FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE:
//   `CCRC::Calc(0, GetAsCharArray(), GetLength())`) -- the two inline accessors each test the data block (length into ecx first,
//   right to left), and VC7.1 duplicates the call + store + return tail into both arms of the GetAsCharArray test.
// NF130 (2026-11-20, one definition; GOTCHAS nf80 / nf86 / nf88): the body is the header's one inline definition in
//   engine/CAnimFilterOfGroup.h (FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE, NF128), which retail also expands in place
//   in NCreatureMode::CModeStrafe::OnActivate 0x008E9A30. The row used to define the member strongly over local CCharString /
//   CAnimFilterBase / CAnimFilterOfGroup / CCRC models, which clashed (LNK2005) with every includer's COMDAT copy; it now includes the
//   shared headers and makes VC7.1 emit the inline body as a pick-any COMDAT through the anonymous-namespace reference below (the
//   0x00686F10 / 0x00860150 pattern). The local models are gone.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE
#include "fable_string.h"
#include "engine/CAnimFilterOfGroup.h"

// The body is the one inline definition in engine/CAnimFilterOfGroup.h (FABLE_RETAIL_CANIMFILTEROFGROUP_SETCURRENTGROUPNAME_INLINE).
namespace {
// Fidelity: scaffolding, not retail: takes the address of the header's inline SetCurrentGroupName so this object emits it out of line
// as a pick-any COMDAT (retail's 0x0073DFF0 is that out-of-line copy; 0x008E9A30 expands the same body). Internal to this TU; never called.
void (CAnimFilterOfGroup::* ReferenceSetCurrentGroupName())(const CCharString&)
{
    return &CAnimFilterOfGroup::SetCurrentGroupName;
}
}
