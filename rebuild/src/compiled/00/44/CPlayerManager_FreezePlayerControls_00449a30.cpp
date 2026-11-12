// CPlayerManager::FreezePlayerControls(long number) -- the numbered player's FreezeControls()
// Leaf: ?FreezePlayerControls@CPlayerManager@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00449A30, 20 bytes, thiscall, `ret 4`: `push number ; call GetPlayer 0x004498C0 ; mov ecx,eax ; call
//   0x00487140`.
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES follow-up; was CSpellContainerList::AddChild with the extern "C" stand-ins
//   FableSpellWrap_6a4dd0 / FableSpellConsume_6a4dd0..., catalog CEngine::GetEditorAnimation): Ego_r.pdb (0x00445230) + FableWin.pdb
//   ?FreezePlayerControls@CPlayerManager@@QAEXJ@Z, the Ego_r neighbour order IsPlayerAssociatedWithJoystickDeviceNumber / FreezePlayerControls /
//   UnfreezePlayerControls = retail 0x004499E0 / 0x00449A30 / 0x00449A50; callees ?GetPlayer@CPlayerManager@@QAEAAVCPlayer@@J@Z
//   0x004498C0 and ?FreezeControls@CPlayer@@QAEXXZ 0x00487140 (engine/CPlayer.h FABLE_RETAIL_PLAYER_FREEZECONTROLS).
#define FABLE_RETAIL_CPLAYERMANAGER_FREEZEPLAYERCONTROLS
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_FREEZECONTROLS
#include "engine/CPlayerManager.h"
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"
void CPlayerManager::FreezePlayerControls(long number)
{
    GetPlayer(number).FreezeControls();
}
