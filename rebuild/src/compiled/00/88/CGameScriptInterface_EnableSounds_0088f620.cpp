// CGameScriptInterface::EnableSounds(bool enable) const -- script switch for the sounds: when the sound manager exists, enabling
//   re-enables its atmos and raises the global sounds-enabled flag; disabling stops every sound, disables the atmos and clears the flag
// Leaf: ?EnableSounds@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088F620, thiscall `ret 4`.
// Identity: Ego_r.pdb ?EnableSounds@CGameScriptInterface@@UBEX_N@Z (virtual, slot 694 of engine/CGameScriptInterface.h).
// Callees: PSoundManager 0x013B8394 slot 16 StopAllSounds and CASoundManager::EnableAtmos 0x00991620 (the Anniversary name agentSND
//   proved; FABLE_RETAIL_CASOUNDMANAGER_ENABLE_NAMES); global g_soundsEnabledFlag 0x0137E974 (landed spelling).
// Sound manager (agentSND2, 2026-11-10): engine/CASoundManager.h; was a raw-vtable CSoundMgr / CFlaggedObject_Retail stand-in on a
//   free __fastcall CGameScriptInterface_EnableSounds (respelled to the PDB member: NAME_FIXES).
// Shape: the manager read once into a local for the test and the enable path; the disable path's second call re-reads the global.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLE_NAMES
#include "engine/CASoundManager.h"
extern unsigned char g_soundsEnabledFlag;                // 0x0137E974

void CGameScriptInterface::EnableSounds(bool enable) const
{
    CASoundManager* mgr = PSoundManager;
    if (mgr == 0)
        return;
    if (enable) {
        mgr->EnableAtmos(true);
        g_soundsEnabledFlag = 1;
        return;
    }
    mgr->StopAllSounds();
    PSoundManager->EnableAtmos(false);
    g_soundsEnabledFlag = 0;
}
