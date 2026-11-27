// 006c2170 CLevelLoader::CObject::Load
// Leaf: ?Load@CObject@CLevelLoader@@AAEXAAVCASuspendableProcess@@@Z
// Flags:
#define FABLE_RETAIL_LEVEL_LOADER_OBJECT_LOAD_PASS29
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_THINGMAPMANAGER
#define FABLE_RETAIL_CWORLDMAP_CREATENAVMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CWORLD_TYPED_SAVE
#define FABLE_RETAIL_CTHINGMANAGER_WORLDLOAD
#define FABLE_RETAIL_CTHINGMAPMANAGER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_STLP_SET_COPY
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CLevelLoader.h"
#include "engine/CWorldMap.h"
#include "engine/CThingMapManager.h"
#include "engine/CWorld.h"
#include "engine/CThingManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMessageEventManager.h"
#include "engine/NProgressDisplay.h"
#include "engine/GFGetTime.h"

void CLevelLoader::CObject::Load(CASuspendableProcess& suspendable_process)
{
    const double start_time = GFGetTime();
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        const CCountedPointer<CAFile>& ptopology_file = RegionLevelFiles[c1].PTopologyFile;
        long level_id = RegionLevelFiles[c1].LevelID;
        if (!ptopology_file.IsNull()) {
            NProgressDisplay::DisplayProgress("Loading topology", -1.0f, false, false);
            PLevelLoader->WorldMap->LoadTopologyFromFile(level_id, ptopology_file, suspendable_process);
            PLevelLoader->WorldMap->GetThingMapManager().AddGameMap(level_id);
            NProgressDisplay::DisplayProgress("Post load topology", -1.0f, false, false);
            PLevelLoader->WorldMap->PostLoadTopology(level_id);
            NProgressDisplay::DisplayProgress("Post load topology end", -1.0f, false, false);
        }
    }
    std::vector<CThing*> loaded_thing_list;
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        const CCountedPointer<CAFile>& pthing_file = RegionLevelFiles[c1].PThingFile;
        if (!pthing_file.IsNull()) {
            NProgressDisplay::DisplayProgress("Loading objects", -1.0f, false, false);
            long level_id = RegionLevelFiles[c1].LevelID;
            PLevelLoader->ThingManager->RuntimeLoadEntities(level_id, LoadableSections, loaded_thing_list);
            PLevelLoader->ThingManager->LoadFromFile(pthing_file, level_id, LoadableSections, loaded_thing_list, &suspendable_process, 3);
        }
    }
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        const CCountedPointer<CAFile>& pnavigation_file = RegionLevelFiles[c1].PNavigationFile;
        long level_id = RegionLevelFiles[c1].LevelID;
        if (!pnavigation_file.IsNull())
            PLevelLoader->WorldMap->CreateNavMap(level_id, LoadableSections, *pnavigation_file, suspendable_process);
    }
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        if (!RegionLevelFiles[c1].PNavigationFile.IsNull())
            PLevelLoader->WorldMap->ActivateNavMap(RegionLevelFiles[c1].LevelID);
    }
    PLevelLoader->ThingManager->ActivateThings(loaded_thing_list);
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        NProgressDisplay::DisplayProgress("Region Level Files: Post Load Initialise", -1.0f, false, false);
        long level_id = RegionLevelFiles[c1].LevelID;
        PLevelLoader->WorldMap->PostLoadInitialise(level_id);
    }
    for (long c1 = 0; c1 < RegionLevelFiles.size(); ++c1) {
        NProgressDisplay::DisplayProgress("Region Level Files: Activate Topology", -1.0f, false, false);
        long level_id = RegionLevelFiles[c1].LevelID;
        if (!RegionLevelFiles[c1].PTopologyFile.IsNull())
            PLevelLoader->WorldMap->ActivateTopology(level_id);
        PLevelLoader->WorldMap->SetMapAsLoading(level_id, false);
        if (!CMainGameComponent::Get()->IsEditorActive())
            PLevelLoader->World->GetMessageEventManager().AddMessage(MESSAGE_EVENT_LEVEL_LOADED, 50, C3DVector(0, 0, 0), 0, 0, 0, 0, level_id);
    }
    if (RegionIndex > 0) {
        if (!CMainGameComponent::Get()->IsEditorActive())
            PLevelLoader->World->GetMessageEventManager().AddMessage(MESSAGE_EVENT_REGION_LOADED, 50, C3DVector(0, 0, 0), 0, 0, 0, 0, RegionIndex);
        PLevelLoader->WorldMap->PostRegionLoad(RegionIndex);
        CCharString world_name = PLevelLoader->World->GetCurrentWorldName();
        PLevelLoader->WorldMap->SetRegionAsLoaded(RegionIndex, world_name);
    }
    const double end_time = GFGetTime();
}