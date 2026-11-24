// CGameScriptInterface::EnableDangerMusic(bool) const -- switch the world music manager's danger music on or off
// Leaf: ?EnableDangerMusic@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00891320, 11 bytes: World (+0x04), its music manager (+0x6C), `jmp 0x006BDE50`.
// Body port (NF321, 2026-11-23): defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT; was
//   ?EnableDangerMusic@CGameScriptInterface_Methods@@QBEXW4EMusicSetType@@@Z); the retail RTTI vftable placement names this PDB member here.
//   The local CWorld / CMusicTarget models are gone: engine/CWorld.h's PMusicManager_Data (+0x6C, the CCountedPointer<CMusicManager>
//   Data word) and engine/CMusicManager.h's ?EnableDangerMusic@CMusicManager@@QAEX_N@Z 0x006BDE50 (landed self row; was the made-up
//   CMusicTarget::CacheMusicSet_006bde50). The member is read directly, not through FableWin.pdb's inline
//   ?GetMusicManager@CWorld@@QAEAAVCMusicManager@@XZ (FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI): VC7.1 then also emits that inline as an
//   unreferenced pick-any COMDAT, which the move may not add (NF320 Q2); same bytes either way.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWORLD
#include "engine/CMusicManager.h"
#include "engine/CWorld.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::EnableDangerMusic(bool enable) const
{
    World->PMusicManager_Data->EnableDangerMusic(enable);
}
