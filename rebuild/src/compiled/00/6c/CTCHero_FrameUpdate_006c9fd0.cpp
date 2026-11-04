// CTCHero::FrameUpdate() -- per-frame hero component update: hand any pending tutorials to the user profile manager
// Leaf: ?FrameUpdate@CTCHero@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006C9FD0, 12 bytes: `call 0x0040D2A0 ; mov ecx,eax ; jmp 0x0040BB30` (this unused; tail call).
// Identity (two sources): CTCHero vtable 0x0125EB54 slot 9 = CTCBase slot 9 FrameUpdate (engine/CTCBase.h); Ego_r.pdb /
//   FableWin.pdb ?FrameUpdate@CTCHero@@UAEXXZ (Ego_r 0x00700990), right after ?ProcessPendingTutorials@CTCHero@@QAEXXZ 0x00700980,
//   as retail 0x006C9FD0 follows 0x006C9FC0, which has the same body (CTCHero::ProcessPendingTutorials, inlined here).
//   The catalog's "CTCHero::CTCHero vfunc_9" was a placeholder.
// Callees: ?Get@CUserProfileManager@@SIAAV1@XZ 0x0040D2A0 (both PDBs), ?ProcessPendingTutorials@CUserProfileManager@@QAEXXZ 0x0040BB30.
// Header: engine/CTCHero.h (new), engine/CUserProfileManager.h (new).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCHero.h"
#include "engine/CUserProfileManager.h"
void CTCHero::FrameUpdate()
{
    CUserProfileManager::Get().ProcessPendingTutorials();
}