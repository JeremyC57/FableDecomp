// CWorld::AutoLoadRegionLeaving() -- save the region-leaving state, delete the quest-start autosave (and its .hs hero state), then load the region-leaving autosave
// Grade: DIFFER(564v564) (QLEN=564, SPLITS.tsv) -- frame slot order only: retail fullPath+heroState at F+0 and path at F+4, ours the reverse; both slots weigh 11 (path 11; fullPath 7 + heroState 4), a density tie the packer's qsort breaks the other way
// Retail: 0x004A34F0 (564 bytes, `ret`, /GS; the catalog extent runs 17 bytes into AutoLoadInternal 0x004A3730, see SPLITS.tsv), FableWin.pdb ?AutoLoadRegionLeaving@CWorld@@QAE_NXZ
// Flags: /GS
// Leaf: ?AutoLoadRegionLeaving@CWorld@@QAE_NXZ
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_SAVEGAMESTATE
#define FABLE_RETAIL_CUSERPROFILEMANAGER_AUTOSAVEPATHS
#define FABLE_RETAIL_CDRIVEMANAGER
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_NRESOURCEDIRECTORYNAMES
#include "engine/CWorld.h"
#include "engine/CQuestManager.h"
#include "engine/CAFile.h"
#include "engine/CDriveManager.h"
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

bool CWorld::AutoLoadRegionLeaving()
{
    AutoSaveRegionLeaving();
    CWideString questStart = CUserProfileManager::Get().GetAutoSaveQuestStartPathName();
    if (CAFile::PathExists(questStart)) {
        CTBaseSingletonNoDefaultAlloc<CDriveManager>::Get().DeleteFileW(questStart);
        CWideString heroState = questStart + L".hs";
        if (CAFile::PathExists(heroState))
            CTBaseSingletonNoDefaultAlloc<CDriveManager>::Get().DeleteFileW(heroState);
    }
    CWideString name = CUserProfileManager::Get().GetAutoSaveRegionLeavingFileName();
    return LoadGameState(name, false);
}
