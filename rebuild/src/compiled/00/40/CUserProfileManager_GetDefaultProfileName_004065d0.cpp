// CUserProfileManager::GetDefaultProfileName() const -- the name of the default profile, L"Default"
// Leaf: ?GetDefaultProfileName@CUserProfileManager@@QBE?AVCWideString@@XZ
// Retail: Fable.exe @ 0x004065D0, 23 bytes, __fastcall `ret 4` (hidden CWideString result, this unused).
// Identity: Ego_r.pdb 0x00405230 / FableWin.pdb 0x0187B2C0 ?GetDefaultProfileName@CUserProfileManager@@QBE?AVCWideString@@XZ (public,
//   const); Ego_r lays it right before GetManualSaveBaseFileName (0x00405250, +0x20), as retail 0x004065D0 sits right before the landed
//   0x004065F0 (+0x20). The body pushes L"Default" (0x0122DE80) through 0x0099B6B0; both binders bind this spelling: the
//   CFrontendGameComponent ctor 0x00496070 (the default-profile load it expands from LoadProfile) and ChooseProfileNumber 0x004962C0.
// NF236 (2026-11-22, q12664): renamed from the catalog's GetAutoSaveFileName (a different PDB member: Ego_r 0x004052E0, L"AutoSave",
//   retail 0x00406690, the landed row and table line of that name) and ported onto the shared headers (fable_string.h CWideString,
//   engine/CUserProfileManager.h FABLE_RETAIL_CUSERPROFILEMANAGER_DEFAULTPROFILENAME) in place of the local models and the cast address.
// Callee: CWideString::CWideString(const unsigned short*) @ 0x0099B6B0 (bound ??0CWideString@@QAE@PBG@Z).
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#define FABLE_RETAIL_CUSERPROFILEMANAGER_DEFAULTPROFILENAME
#include "engine/CUserProfileManager.h"

CWideString CUserProfileManager::GetDefaultProfileName() const
{
    return L"Default";
}
