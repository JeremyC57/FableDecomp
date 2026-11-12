// CUserProfileManager::Get() -- the profile manager singleton: create it on first use (Reset with a new CUserProfileManager), then return it
// Leaf: ?Get@CUserProfileManager@@SIAAV1@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040D2A0, 69 bytes, static __fastcall (no arguments), plain `ret`. Callees: operator new 0x00BFEA1A
//   (0x140 bytes), CUserProfileManager::CUserProfileManager 0x0040CEC0, CCountedPointer<CUserProfileManager>::Reset 0x0040F1B0.
// Names (agentNF27, 2026-11-12, FAM10 NAME_FIXES follow-up; was CUserProfileManagerSingleton::Get over the stand-in global
//   g_CUserProfileManagerSingleton_013B7D4C whose operator= was the Reset): Ego_r.pdb + FableWin.pdb ?Get@CUserProfileManager@@SIAAV1@XZ
//   (engine/CUserProfileManager.h and fable_user_profile.h already declare it for 0x0040D2A0); the pointer at 0x013B7D4C is
//   CUserProfileManager::PClass (Ego_r / FableWin `dynamic initializer for 'CUserProfileManager::PClass'', private static per the Anniv
//   Test PDB); the call at 0x0040D2C7 is FableWin.pdb ?Reset@?$CCountedPointer@VCUserProfileManager@@@@QAEXPAVCUserProfileManager@@@Z
//   (it stores DeleteData<CUserProfileManager> 0x0040D830).
// Shape: `if (PClass.IsNull()) PClass.Reset(new CUserProfileManager); return *PClass;` -- the Data word is read again after the Reset;
//   the null-checked new keeps two Reset call sites (VC7.1 does not merge the arms).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_PCLASS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "fable_user_profile.h"

CUserProfileManager& FABLE_FASTCALL CUserProfileManager::Get()
{
    if (PClass.IsNull())
        PClass.Reset(new CUserProfileManager);
    return *PClass;
}
