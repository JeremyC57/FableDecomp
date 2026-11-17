// NHeroInformationScreens::CManager::SelectScreenOfType(EScreenType type) -- remember the screen type to show (stored at +0x28)
// Leaf: ?SelectScreenOfType@CManager@NHeroInformationScreens@@QAEXW4EScreenType@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066E799, 10 bytes: `mov eax,[esp+4] ; mov [ecx+28h],eax ; ret 4`.
// Identity (two sources): Ego_r.pdb + FableWin.pdb ?SelectScreenOfType@CManager@NHeroInformationScreens@@QAEXW4EScreenType@2@@Z, which
//   engine/NHeroInformationScreens_CManager.h declares for 0x0066E799; the landed callers 0x00577216 (OpenInventoryInternal) and NF76's
//   0x005778CA (CloseInventoryInternalPC) bind it, passing INVENTORY_EXPERIENCE_SCREEN on the manager CPlayerGui::GetPHeroInfoScreensManager
//   0x00437CD0 returns. The catalog's "_global::SetMember_dword0x28_0066e799" and the old leaf "AddExclusiveObserver" were propagated from a
//   byte-identical setter (retail has no identical-code folding: one address, one name).
// Layout: the header's LAYOUT opt-in (CurrentScreenIndex +0x28, descriptive member name; neither PDB gives the layout).
// NF79 (round 7, q10959): re-authored on the shared header; was a free __fastcall stand-in over a local struct.
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#include "engine/NHeroInformationScreens_CManager.h"

namespace NHeroInformationScreens {
void CManager::SelectScreenOfType(EScreenType type)
{
    CurrentScreenIndex = type;
}
}
