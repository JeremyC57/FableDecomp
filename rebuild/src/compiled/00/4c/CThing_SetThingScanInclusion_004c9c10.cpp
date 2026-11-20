// CThing::SetThingScanInclusion(bool include) -- store the IncludeInThingScans flag (bit 2 of +0x91), then re-evaluate the thing's scan inclusion
// Leaf: ?SetThingScanInclusion@CThing@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C9C10, 33 bytes (`ret 4`): the bit merge (`shl al,2 ; xor ; and 4 ; xor`) into +0x91, then
//   `call 0x004C8C00` (?UpdateThingScanInclusion@CThing@@IAEXXZ, Ego_r.pdb 0x004CAB10 + FableWin.pdb 0x01CF80C0, protected).
// Naming (NF198, 2026-11-20, q11973): the address is CThing::SetThingScanInclusion, as the catalog already says: Ego_r.pdb 0x004CB6B0 +
//   FableWin.pdb 0x01CF8070 ?SetThingScanInclusion@CThing@@QAEX_N@Z; Ego_r.pdb CThing layout byte 161 (retail +0x91) bit 2 is
//   IncludeInThingScans (bits 0/1 Dead / CreatedByOwnerThingCreation), and the callee is UpdateThingScanInclusion. The queue's
//   SetAsGameStatePersisted claim is wrong: GameStatePersisted is bit 5 of +0x92, set by 0x004C7880 (CThing.h STATEACCESS).
//   Was the free __fastcall stand-in SetAsGameStatePersisted(CThing*, int, bool) on a local CThingBits view; now on engine/CThing.h.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_SETTHINGSCANINCLUSION
#include "engine/CThing.h"

void CThing::SetThingScanInclusion(bool include)
{
    IncludeInThingScans = include;
    UpdateThingScanInclusion();
}
