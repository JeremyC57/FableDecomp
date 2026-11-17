// CMainGameComponent::PostInit() -- hand the display engine to the world, then either load the save named in the init
//   structure (flagging a failed load) or load the world file (the init structure's world, else the remembered one, else
//   updatedscenic.wld from the levels directory); in static-map export mode copy the world's map files to their static
//   paths and stop, otherwise run the post-load step and activate the initial quest; finally register the component
// Leaf: ?PostInit@CMainGameComponent@@MAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x00416953, 738 bytes, frame pointer, no EH frame (the TU has no unwind tables).
// Layout: CMainGameComponent as in the landed ForceRender 0x004164E0 (retail offsets); InitStructure +0x161d0 holds
//   (retail loads) a world name +0x0 and a second wide string +0x4, the initial quest CCharString +0x8 and the save name +0xc.
// Names: the path helpers are Ego_r.pdb / FableWin.pdb CWorld::ConvertWorldToStaticMapPath (0x0049DDD0 / 0x0049D550) and
//   ConvertWorldToISMThingyPath (0x0049DEC0 / 0x0049D6B0); 0x0049F180 is ?ActivateWorld@CWorld@@QAEX_N@Z (was PostLoad(long));
//   the globals 0x013B89C0 / 0x013B8648 / 0x013B8616 are descriptive (no PDB name; the spellings of the landed majority); literals 0x0122EE48
//   "Loading save", 0x0122EE38 "Loading world", 0x0122EE14 L"updatedscenic.wld", 0x0122EDFC "Activate Initial Quests",
//   0x0122D70E "", -1.0f 0x0122DEE0.
// Name fix (agentNF26, 2026-11-12, FAM09 follow-up): the object binders below now use the PDB object names (Ego_r.pdb + FableWin.pdb
//   `dynamic initializer for '...'`, defining rows named by agentFAM09):
//   0x013B8668 GForceLevel (global; initializer 0x01219FC0), was g_WorldToLoad.
//   0x013B866C GForceStartingHolySite (global; initializer 0x01219FE0), was g_WorldToLoad2.
//   0x013B8674 GRunLUAScript (global CCharString; initializer 0x0121A020), was g_Registration (a CRegistration). It goes to the protected
//   CMainGameComponent::InitLUA(const CCharString&) 0x004BBC00 (Ego_r.pdb + FableWin.pdb; empty `ret 4` in both builds), was Register.
// NF73 (2026-11-17): ported onto the shared headers (CMainGameComponent.h EVENTPACKAGES / EDITOR / INITLUA + NF73 INITSTRUCTURE,
//   CMainGameComponentInit.h, CWorld.h + NF73 STATICMAPEXPORT, CDiskFileWin32.h, CAFile.h + NF73 COPYFILE, CQuestManager.h ACTIVATEQUEST,
//   NProgressDisplay.h, fable_profile.h, fable_resource_directories.h LEVELDIR); Ego_d.pdb local names. Respelled: the world's slot 7 is
//   ?PostInit@CWorld@@UAEXAAVCDisplayEngine@@@Z (was SetDisplayEngine), ?LoadGameState@CWorld@@QAE_NABVCWideString@@_N@Z (was LoadGame),
//   ?ActivateQuest@CQuestManager@@QAE_NABVCCharString@@_N1@Z (bool result), ?GetLevelDir@NResourceDirectoryNames@@YI?AVCWideString@@XZ.
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_COPYFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTPACKAGES
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT_INITLUA
#define FABLE_RETAIL_CMAINGAMECOMPONENT_INITSTRUCTURE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_LOADGAMESTATEINTERNAL
#define FABLE_RETAIL_CWORLD_STATICMAPEXPORT
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUEST
#define FABLE_RETAIL_NRESOURCEDIRECTORYNAMES_LEVELDIR
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMainGameComponentInit.h"
#include "engine/CWorld.h"
#include "engine/CQuestManager.h"
#include "engine/NProgressDisplay.h"
#include "fable_profile.h"
#include "fable_resource_directories.h"

extern bool        g_bSaveLoadFailed;                  // 0x013B89C0
extern CWideString GForceLevel;                        // 0x013B8668
extern CWideString GForceStartingHolySite;             // 0x013B866C
extern bool        g_bCompileFrontendDefinitions;      // 0x013B8648
extern bool        g_bRuntimeLevelFiles;               // 0x013B8616
extern CCharString GRunLUAScript;                      // 0x013B8674

void CMainGameComponent::PostInit()
{
    PWorld->PostInit(*PDisplayEngine);
    if (InitStructure.SaveGameName.GetLength() > 0) {
        NProgressDisplay::DisplayProgress("Loading save", -1.0f, false, false);
        if (!PWorld->LoadGameState(InitStructure.SaveGameName, true))
            g_bSaveLoadFailed = true;
    } else {
        NProgressDisplay::DisplayProgress("Loading world", -1.0f, false, false);
        CWideString filename;
        if (InitStructure.InitialWorldName.GetLength() > 0) {
            filename = NResourceDirectoryNames::GetLevelDir() + InitStructure.InitialWorldName;
            GForceLevel = InitStructure.InitialWorldName;
            if (InitStructure.InitialWorldHolySiteName.GetLength() > 0)
                GForceStartingHolySite = InitStructure.InitialWorldHolySiteName;
        } else if (GForceLevel.GetLength() > 0) {
            filename = NResourceDirectoryNames::GetLevelDir() + GForceLevel;
        } else {
            filename = NResourceDirectoryNames::GetLevelDir() + L"updatedscenic.wld";
        }
        PWorld->Load(filename);
        if (g_bCompileFrontendDefinitions) {
            CWideString static_map_filename = PWorld->ConvertWorldToStaticMapPath(filename);
            CDiskFileWin32 static_map_file(static_map_filename, FILE_ACCESS_READ, 2);
            CWideString xbox_static_map_filename = PWorld->ConvertWorldToStaticMapPath(filename, true, g_bRuntimeLevelFiles);
            CDiskFileWin32 xbox_static_map_file(xbox_static_map_filename, FILE_ACCESS_CREATE, 2);
            CAFile::CopyFileW(static_map_file, xbox_static_map_file);
            CWideString ism_thingy_filename = PWorld->ConvertWorldToISMThingyPath(filename);
            CDiskFileWin32 ism_thingy_file(ism_thingy_filename, FILE_ACCESS_READ, 2);
            CWideString xbox_ism_thingy_filename = PWorld->ConvertWorldToISMThingyPath(filename, true, g_bRuntimeLevelFiles);
            CDiskFileWin32 xbox_ism_thingy_file(xbox_ism_thingy_filename, FILE_ACCESS_CREATE, 2);
            CAFile::CopyFileW(ism_thingy_file, xbox_ism_thingy_file);
            return;
        }
        PWorld->ActivateWorld(false);
        NProfileTimer::StartProfile("Activate Initial Quests", 0);
        if (InitStructure.InitialQuestName != "")
            CQuestManager::Get().ActivateQuest(InitStructure.InitialQuestName, false, true);
        NProfileTimer::EndProfile(0);
    }
    InitLUA(GRunLUAScript);
}
