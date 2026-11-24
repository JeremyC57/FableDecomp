// CUserProfileManager::GetAutoSaveFileName() const -- the file name of the auto save, L"AutoSave"
// Leaf: ?GetAutoSaveFileName@CUserProfileManager@@QBE?AVCWideString@@XZ
// Retail: Fable.exe @ 0x00406690, 23 bytes, __fastcall `ret 4` (hidden CWideString result, this unused).
// Identity: Ego_r.pdb 0x004052E0 / FableWin.pdb 0x0187B9B0 ?GetAutoSaveFileName@CUserProfileManager@@QBE?AVCWideString@@XZ (public,
//   const); the body pushes L"AutoSave" (0x0122DE9C) through 0x0099B6B0; every landed caller binds this spelling (the table line,
//   witness 0x004A0AE0: CWorld::AutoSave, AutoLoadRespawn, the in-game menu rows, CFrontEndManager::GetSavedGameNames).
// NF236 (2026-11-22, q12667 follow-up of q12664): leaf-only self respell (was the non-const QAE of a local model, which also
//   collided with the mislabelled 0x007D7C10's QAE leaf) and ported onto the shared headers (fable_string.h CWideString,
//   engine/CUserProfileManager.h FABLE_RETAIL_CUSERPROFILEMANAGER_SAVESLOTS) in place of the local models and the cast address.
// Callee: CWideString::CWideString(const unsigned short*) @ 0x0099B6B0 (bound ??0CWideString@@QAE@PBG@Z).
#define FABLE_RETAIL_CWIDESTRING
#include "fable_string.h"
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SAVESLOTS
#include "engine/CUserProfileManager.h"

CWideString CUserProfileManager::GetAutoSaveFileName() const
{
    return L"AutoSave";
}
