// NHeroInformationScreens::CManager::GetCurrentScreen() -- the screen at the current index (falls back to index 0 when that slot is empty)
// Leaf: ?GetCurrentScreen@CManager@NHeroInformationScreens@@QAEPBVCBase@2@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066E7BD, 23 bytes: `mov eax,[ecx+4] ; mov edx,[ecx+28h] ; cmp [eax+edx*4],0 ; jne ;
//   and [ecx+28h],0 ; mov ecx,[ecx+28h] ; mov eax,[eax+ecx*4] ; ret` (the `and` store: a size-optimised TU, as the HEAD row's pragma).
// Identity (two sources): Ego_r.pdb 0x006A0CAB + FableWin.pdb 0x0211514F ?GetCurrentScreen@CManager@NHeroInformationScreens@@QAEPBVCBase@2@XZ
//   (non-const method returning const CBase*), which engine/NHeroInformationScreens_CManager.h declares for 0x0066E7BD; every landed
//   caller binds it (0x00574576 0x00577216 0x00577AC0 0x00622A00, and 0x006116E0 after NF217's respell of its QAEPAV stand-in).
// Layout: the header's LAYOUT opt-in (RegisteredScreens vector +0x04, CurrentScreenIndex +0x28; descriptive member names, neither PDB
//   gives the layout), as the landed sibling 0x0066E799 SelectScreenOfType.
// NF217 (2026-11-21, q12373): re-authored on the shared header; was the July free __fastcall stand-in
//   ?CManager_GetCurrentScreen@@YIPAUCBase@@PAUCManager@@@Z over local CBase / CManager structs.
#pragma optimize("s", on)
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#include "engine/NHeroInformationScreens_CManager.h"

namespace NHeroInformationScreens {
const CBase* CManager::GetCurrentScreen()
{
    if (RegisteredScreens[CurrentScreenIndex] == 0)
        CurrentScreenIndex = 0;
    return RegisteredScreens[CurrentScreenIndex];
}
}
