// CWorld::AutoLoadRespawn() -- reload after the hero's death: the checkpoint save when one exists between a quest's start and finish
//   screens, else the most recent manual save, else the quest-start autosave (between the screens), else the plain autosave; each
//   through AutoLoadInternal -> LoadGameState(name, false), which queues the load; a queued load marks the status 3
// Grade: DIFFER(1722v1722) -- one instruction pair: in the manual-save copy VC7.1 hoists `lea ecx,[esp+10h]` above the `je` to the
//   shared path destructor (retail 0x004A6115 keeps it at the target 0x004A6242)
// Leaf: ?AutoLoadRespawn@CWorld@@QAE_NXZ (name fix: the catalog's "AutoLoadRegionLeaving" belongs to 0x004A34F0)
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A5E90, 1722 bytes, thiscall, plain `ret`, /GS cookie, frame 0x234.
// Identity: Ego_r.pdb ?AutoLoadRespawn@CWorld@@QAE_NXZ is 1643 bytes / 464 instructions (retail 1722 / 484) and
//   ?AutoLoadRegionLeaving 539 / 148 (retail 0x004A34F0: 564 / 154); FableWin.pdb sizes keep the same order (948 v 236 bytes).
// Callees: CQuestManager::IsBetweenQuestStartAndFinishScreens 0x004AF290 (singleton 0x013B89FC), CUserProfileManager::Get 0x0040D2A0
//   + GetAutoSaveCheckPointPathName 0x00408040 / GetAutoSaveCheckPointFileName 0x004066F0 / GetAutoSaveQuestStartFileName 0x004066B0 /
//   GetAutoSaveFileName 0x00406690 / GetPathNameFromFileName 0x00408120, CAFile::PathExists 0x00999230 / IsPathFile 0x00999A30 /
//   StartFileScan 0x00999760, NResourceDirectoryNames::GetDebugSaveDir 0x0041A1A0, CWorld::GetPathNameForFileName 0x0049EF50,
//   CWideString ctor 0x0099AED0 / dtor 0x0099B510 / operator= 0x0099B7D0, 0x0099B800 / operator!= 0x0099B150 / operator+ 0x0099BE70.
// Shape: LoadGameState (landed 0x004A3200 body) and AutoLoadInternal (retail 0x004A3730 is `jmp LoadGameState`) are defined in the
//   TU; VC7.1 expands both into each of the four calls without __forceinline.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_NRESOURCEDIRECTORYNAMES
#include "engine/CWorld.h"
#include "engine/CQuestManager.h"
#include "engine/CAFile.h"
#include "fable_user_profile.h"
#include "fable_resource_directories.h"
#include "fable_string.h"

extern bool g_bLoadFromAnyDirectory;                         // 0x013B8646 (console variable; descriptive, as the landed LoadGameState 0x004A3200)

// CWorld::LoadGameState (retail 0x004A3200, landed) and AutoLoadInternal (retail 0x004A3730: `return LoadGameState(name, loadNow);`)
// are defined in this TU; AutoLoadRespawn expands both four times with loadNow == false (0x004A5EEF..0x004A605A).
bool CWorld::LoadGameState(const CWideString& name, bool loadNow)
{
    if (SaveGameLoadStatus == 0 || loadNow) {
        bool found = false;
        CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(name, false);
        if (path != L"")
            found = true;
        if (g_bLoadFromAnyDirectory) {
            if (!found)
                return false;
            if (loadNow) {
                SerialisingAboutToLoadHeroState = IsHeroStateFileValid(name);
                bool loaded = LoadGameStateInternal(path, false);
                SerialisingAboutToLoadHeroState = false;
                if (!loaded)
                    return false;
                return LoadHeroStateInternal(name);
            }
            SaveGameLoadStatus = (ESaveGameLoadStatus)1;
            SaveGamePathName = path;
            return true;
        }
        if (found && CAFile::PathExists(path) && CAFile::IsPathFile(path)) {
            if (loadNow) {
                SerialisingAboutToLoadHeroState = IsHeroStateFileValid(name);
                bool loaded = LoadGameStateInternal(path, false);
                SerialisingAboutToLoadHeroState = false;
                if (!loaded)
                    return false;
                return LoadHeroStateInternal(name);
            }
            SaveGameLoadStatus = (ESaveGameLoadStatus)1;
            SaveGamePathName = path;
            return true;
        }
        CWideString fullPath;
        CWideString pattern = NResourceDirectoryNames::GetDebugSaveDir() + name;
        CAFile::CFileScanInfo info;
        wchar_t fileName[262];
        if (CAFile::StartFileScan(pattern, info, fileName))
            fullPath = fileName;
        else
            fullPath = GetPathNameForFileName(name);
        if (fullPath != L"") {
            if (loadNow) {
                SerialisingAboutToLoadHeroState = IsHeroStateFileValid(name);
                bool loaded = LoadGameStateInternal(path, false);
                SerialisingAboutToLoadHeroState = false;
                if (!loaded)
                    return false;
                return LoadHeroStateInternal(name);
            }
            SaveGameLoadStatus = (ESaveGameLoadStatus)1;
            SaveGamePathName = fullPath;
            return true;
        }
    }
    return false;
}

bool CWorld::AutoLoadInternal(const CWideString& name, bool loadNow)
{
    return LoadGameState(name, loadNow);
}

bool CWorld::AutoLoadRespawn()
{
    if (CQuestManager::Get().IsBetweenQuestStartAndFinishScreens()) {
        CWideString checkPointPath = CUserProfileManager::Get().GetAutoSaveCheckPointPathName();
        if (CAFile::PathExists(checkPointPath)) {
            if (AutoLoadInternal(CUserProfileManager::Get().GetAutoSaveCheckPointFileName(), false)) {
                SaveGameLoadStatus = (ESaveGameLoadStatus)3;
                return true;
            }
            return false;
        }
    }
    if (MostRecentSaveType == 1) {
        if (AutoLoadInternal(MostRecentManualSaveName, false)) {
            SaveGameLoadStatus = (ESaveGameLoadStatus)3;
            return true;
        }
        return false;
    }
    if (MostRecentSaveType == 3 && CQuestManager::Get().IsBetweenQuestStartAndFinishScreens()) {
        if (AutoLoadInternal(CUserProfileManager::Get().GetAutoSaveQuestStartFileName(), false)) {
            SaveGameLoadStatus = (ESaveGameLoadStatus)3;
            return true;
        }
        return false;
    }
    if (AutoLoadInternal(CUserProfileManager::Get().GetAutoSaveFileName(), false)) {
        SaveGameLoadStatus = (ESaveGameLoadStatus)3;
        return true;
    }
    return false;
}
