// CWorld::SaveGameStateManual(const CWideString& name) -- manual save: between a quest's start and finish screens copy the plain (or quest-start) autosave to the named slot and save only the hero state next to it, otherwise save the whole game; then record the manual save and its name
// Leaf: ?SaveGameStateManual@CWorld@@QAEXABVCWideString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A0780, 200 bytes, thiscall, `ret 4`.
// Identity: Ego_r.pdb ?SaveGameStateManual@CWorld@@QAEXABVCWideString@@@Z (Ego_r donor order SaveGameState 0x004A9690 /
//   SaveGameStateManual 0x004A98C0 / AutoSaveInternal 0x004A9990 = retail 0x004A05C0 / 0x004A0780 / 0x004A0850, the neighbours landed).
// Callees: CQuestManager::Get() (inline singleton 0x013B89FC) / IsBetweenQuestStartAndFinishScreens 0x004AF290,
//   CUserProfileManager::Get 0x0040D2A0 / GetPathNameFromFileName 0x00408120 / GetAutoSavePathName 0x00406F70 /
//   GetAutoSaveQuestStartPathName 0x00407FD0, CAFile::PathExists 0x00999230, CWorld::CopyFileInternal 0x0049DD50 (protected const),
//   SaveHeroStateInternal 0x004A00E0, SaveGameState 0x004A05C0, ~CWideString 0x0099B510, CWideString::operator= 0x0099B7D0.
// Shape: the source path, its existence test and the copy are written in both arms of the save-type test: c2 hoists the shared
//   hidden-result `lea ecx,[esp+10h] ; push ecx` above the `jne` (0x004A07B7) and cross-jumps the identical tails, which also gives
//   the copy's `lea edx ; lea eax ; push ; push` order (0x004A07E5). The prior's inline if/return helper picked ecx / edx per arm
//   (205v200), a conditional expression adds conditional-destruction temporaries (273v200).
// Layout: engine/CWorld.h (FABLE_RETAIL_CWORLD): SerialisingNonPersistentQuestItems +0x103, MostRecentSaveType +0x134,
//   MostRecentSaveTypeBeforeManualSave +0x138, MostRecentManualSaveName +0x13C. ESaveType values (enumerators not in the PDB name
//   sets): 1 manual, 2 plain autosave.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_SAVEGAMESTATE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVEPATHS
#define FABLE_RETAIL_CAFILE
#include "fable_user_profile.h"
#include "engine/CWorld.h"
#include "engine/CQuestManager.h"
#include "engine/CAFile.h"
#include "fable_string.h"

void CWorld::SaveGameStateManual(const CWideString& name)
{
    if (CQuestManager::Get().IsBetweenQuestStartAndFinishScreens()) {
        CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(name, false);
        if (MostRecentSaveTypeBeforeManualSave == 2) {
            CWideString source = CUserProfileManager::Get().GetAutoSavePathName();
            if (CAFile::PathExists(source))
                CopyFileInternal(source, path);
        } else {
            CWideString source = CUserProfileManager::Get().GetAutoSaveQuestStartPathName();
            if (CAFile::PathExists(source))
                CopyFileInternal(source, path);
        }
        SerialisingNonPersistentQuestItems = false;
        SaveHeroStateInternal(name, false);
        SerialisingNonPersistentQuestItems = true;
    } else
        SaveGameState(name, false, false);
    MostRecentSaveType = (ESaveType)1;
    MostRecentManualSaveName = name;
}