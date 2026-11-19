// CWorld::PeekMyPlayer() const -- the main player, through the world's player manager (PlayerManager +0x0C)
// Leaf: ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049D9D0, 8 bytes: `mov ecx,[ecx+0Ch] ; jmp 0x00449970` (tail call of the non-const
//   CPlayerManager::GetMainPlayer through the const world's pointer member).
// NF104 (q4493, 2026-11-19): moved off its `T::Run()` pad forwarder stand-in onto the shared engine/CWorld.h (FABLE_RETAIL_CWORLD declares
//   PeekMyPlayer, PlayerManager +0x0C asserted) and engine/CPlayerManager.h (GetMainPlayer 0x00449970, ?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ).
//   Name: Ego_r.pdb + FableWin.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ (Ego_r order GetSampleBank 0x0049D9B0, PeekSampleBank, PeekMyPlayer);
//   CalculateSecuritySweepTasks 0x00659800 and 61 other landed rows bind this symbol.
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"

const CPlayer* CWorld::PeekMyPlayer() const
{
    return &PlayerManager->GetMainPlayer();
}
