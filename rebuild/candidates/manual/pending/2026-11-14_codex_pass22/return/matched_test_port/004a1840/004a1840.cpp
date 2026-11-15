// 004a1840 CWorld::Load(const CWideString&) -- loads world/quests, startup WAD and static map state.
// Retail:0x004A1840..004A1CA9,1129bytes including its catch handler and normal-return cleanup.
// Flags: none
// Leaf: ?Load@CWorld@@QAEXABVCWideString@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_SPRINTF
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_TYPED_MEMBERS
#define FABLE_RETAIL_CWORLD_TYPED_SAVE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CWORLD_LOAD_ORIGINAL
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CAFILE_SPLITPATH_WIDE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_WORLDLOAD
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER_CRITICALDISPLAY
#include "fable_string.h"
#include "fable_profile.h"
#include "fable_world_options.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CBBBFileException.h"
#include "engine/CLevelLoader.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CSystemManager.h"
#include "engine/NProgressDisplay.h"

void CWorld::GenerateOfflineDataForWorld(const CWideString& world_name)
{
    if(GEnableStaticMapCreation){
        CWideString static_map_path=ConvertWorldToStaticMapPath(world_name);
        GetWorldMap().CreateStaticMaps(static_map_path,Component.GetDisplayEngine().Get3DEngine());
    }
}

void CWorld::Load(const CWideString& level)
{
    CWideString current_world_name;
    CAFile::SplitPath(level,0,0,&current_world_name,0);
    CurrentWorldName=current_world_name.ToCharString();
    CDiskFileWin32 file;
    try{
        file.Open(level,FILE_ACCESS_READ,2);
    }catch(CBBBFileException& exc){
        CCharString error;
        error.Sprintf("Unable to load world %s",level.GetAsCharArray());
        GFGetSystemManager()->DisplayCriticalMessage(error);
        return;
    }
    NProfileTimer::StartProfile("Load Quests",0);
    CWideString static_map_path=ConvertWorldToStaticMapPath(level);
    CWideString quest_path=ConvertWorldToQuestPath(level);
    if(CAFile::PathExists(quest_path)){
        CDiskFileWin32 quest_file(quest_path,FILE_ACCESS_READ,2);
        LoadQuestsFromFile(quest_file,true);
    }
    CWideString global_quests_path(L"Data\\Levels\\GlobalQuests.qst");
    if(CAFile::PathExists(global_quests_path)){
        CDiskFileWin32 global_quest_file(global_quests_path,FILE_ACCESS_READ,2);
        LoadQuestsFromFile(global_quest_file,false);
    }
    PWorldMap->SetAsUsingWAD(L"");
    NProfileTimer::StartProfile("Startup Wad",0);
    NProgressDisplay::DisplayProgress("Startup WAD",-1.0f,false,false);
    if(GUseLevelWAD){
        CWideString wad_path=ConvertWorldToLevelWADPath(level);
        PWorldMap->CreateWorldLevelWad(level,wad_path);
        PWorldMap->SetAsUsingWAD(ConvertWorldToLevelWADPath(level));
    }
    NProfileTimer::EndProfile(0);
    LoadWorldFromFile(file,-1);
    while(PWorldMap->GetLevelLoader()->IsBusy())
        PWorldMap->GetLevelLoader()->RunSuspendableProcess();
    NProfileTimer::StartProfile("Generate Offline Data",0);
    NProgressDisplay::DisplayProgress("Generate Offline Data",-1.0f,false,false);
    GenerateOfflineDataForWorld(level);
    NProfileTimer::StartProfile("Set Static Map for Engine",0);
    NProgressDisplay::DisplayProgress("Set Static Map for Engine",-1.0f,false,false);
    Component.GetDisplayEngine().Get3DEngine().SetStaticMapFileForUse(static_map_path);
    NProgressDisplay::DisplayProgress("Set Static Map for Engine: End",-1.0f,false,false);
    NProfileTimer::EndProfile(0);
}
