// CTCInGameMenu::LoadGame(long index) -- remember which save to load: LoadGameIndex (+0x64) = index
// Leaf: ?LoadGame@CTCInGameMenu@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047D008, 10 bytes, thiscall, `ret 4`.
// Name (NF348, 2026-11-24): Ego_r.pdb + FableWin.pdb ?LoadGame@CTCInGameMenu@@QAEXJ@Z (Ego_r body ~10 bytes, PCSetToLoad ~489 = landed
//   0x0047DD9D); Ego_r NUISystem::CManager::Action calls LoadGame, and retail's only caller is 0x0041F831 in the landed
//   NUISystem::CManager::Action 0x0041E6D3 (respelled onto this name at nf345 b2). Was the stand-in ?Set@T@@QAEXH@Z (catalog
//   _global::SetMember_dword0x64_0047d008); the PDB long parameter gives the same bytes as the stand-in's int.
// Layout: the shared engine/CTCInGameMenu.h (generated flat branch; LoadGameIndex +0x64 asserted there) with the add-only
//   declaration opt-in FABLE_RETAIL_CTCINGAMEMENU_LOADGAME.
#define FABLE_RETAIL_CTCINGAMEMENU_LOADGAME
#include "engine/CTCInGameMenu.h"

void CTCInGameMenu::LoadGame(long index)
{
    LoadGameIndex = index;
}
