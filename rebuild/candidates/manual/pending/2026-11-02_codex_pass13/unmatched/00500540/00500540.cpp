// CWorldMap::LoadRegion(long, ELoadType, bool) -- apply region effects, open level files and queue the region loader.
// Leaf: ?LoadRegion@CWorldMap@@QAEXJW4ELoadType@1@_N@Z
#define FABLE_RETAIL_CVECTORMAP_ORDINARY_LOWERBOUND
#define FABLE_RETAIL_CTHINGFILTER_HASTC_EXTERNAL
#define FABLE_RETAIL_STLPORT_TREE_INSERT_BODY
#define FABLE_RETAIL_CVECTORMAP_SIZE_LOWERBOUND
#define FABLE_RETAIL_REGION_CONNECTIVITY_TYPES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYER_TYPED
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHING_NAVIGATION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_REGION_LOADING_TYPES
#define FABLE_RETAIL_REGION_SERIALIZATION
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_EXTERNAL
#define FABLE_PDCB_TYPED_MEMBERS
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CMapInfo.h"
#include "engine/CRegionDef.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CWADFile.h"
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CEnvironment.h"
#include "engine/CGameTimeManager.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CLevelLoader.h"
#include "engine/CAsyncLoadingMemoryPool.h"
#include "engine/CQuestManager.h"
#include "fable_game.h"
extern const CCharString MOUTH_DUMMY_OBJECT_NAME;
// The original Reset specialization is out of line in retail; its complete body
// exposes pointer lifetime information to the caller without changing its public name.
template <> __declspec(noinline) void CCountedPointer<CAFile>::Reset(CAFile* data)
{
    ReleaseData();
    Data = data;
    if (data) Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
}
__declspec(noinline) void CLevelLoader::CObject::SetAsLoading(const std::vector<CLevelFiles>& level_files,const std::set<CCharString>& loadable_sections,long region_index)
{
    RegionLevelFiles=level_files;
    LoadableSections=loadable_sections;
    RegionIndex=region_index;
}
__declspec(noinline) bool CLevelLoader::IsBusy() const { return !ObjectList.empty(); }
void CWorldMap::LoadRegion(long index,ELoadType load_type,bool load_navigation)
{
    CIEngine& engine=CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine();
    engine.SetMapVisibilityAllMaps(false);
    {
        CDefPointer<const CRegionDef> pregion_def=Regions[index].GetPRegionDef();
        if(pregion_def.Get()) {
            if(pregion_def->EnvironmentTheme>0) World.GetEnvironment().SetRegionTheme(pregion_def->EnvironmentTheme);
            if(pregion_def->InternalEnvironmentTheme>0) {
                for(long n=0;n<pregion_def->InternalLightingChannelCount;++n)
                    World.GetEnvironment().SetTheme(n+1,0,pregion_def->InternalEnvironmentTheme);
            }
            engine.SetSpotlightShadowMode(pregion_def->EnableSpotlightShadowMode);
            if(pregion_def->HasSea) engine.SetSeaBodyToRender(Regions[index].GetName());
            else engine.SetSeaBodyToRender("");
            if(PCameraRegionParticle.Get() && !PCameraRegionParticle->IsDead()) PCameraRegionParticle->Kill(false);
            if(pregion_def->AttachToCameraFX>0) {
                C3DVector camera_pos=World.GetGameCamera()->GetRenderCamera(1.0f).PeekWorldPos();
                PCameraRegionParticle=CTCDParticleEmitter::Create(pregion_def->AttachToCameraFX,camera_pos,false);
                if(PCameraRegionParticle.Get() && !PCameraRegionParticle->IsDead()) {
                    CTCDParticleEmitter* pparticle_driver;
                    if(PCameraRegionParticle->GetTC(&pparticle_driver)) {
                        pparticle_driver->AttachToCamera(8,0.0f);
                        PCameraRegionParticle->SetCutscenePauseBehaviour(CUTSCENE_BEHAVIOUR_NOT_PAUSED);
                    }
                }
            }
            if(PHeroBreathParticle.Get() && !PHeroBreathParticle->IsDead()) PHeroBreathParticle->Kill(false);
            if(pregion_def->HeroBreathParticle>0) {
                CThingPlayerCreature* phero=World.PeekMyPlayer()->GetPControlledCreature();
                if(phero && !phero->IsDead()) {
                    PHeroBreathParticle=CTCDParticleEmitter::Create(pregion_def->HeroBreathParticle,phero->GetPos(),false);
                    if(PHeroBreathParticle.Get() && !PHeroBreathParticle->IsDead()) {
                        CTCDParticleEmitter* pparticle_driver;
                        if(PHeroBreathParticle->GetTC(&pparticle_driver))
                            pparticle_driver->AttachToThing(*phero,10,MOUTH_DUMMY_OBJECT_NAME,0,0.0f);
                    }
                }
            }
            World.GetGameTimeManager().SetAsUsingVillageTime(pregion_def->UseVillageTime);
        } else World.GetGameTimeManager().SetAsUsingVillageTime(false);
    }
    std::vector<CLevelLoader::CObject::CLevelFiles> level_files;
    const std::vector<long>& region_maps=Regions[index].GetMaps();
    for(long c1=0;c1<region_maps.size();++c1) {
        engine.SetMapVisibility(region_maps[c1],true);
        CWideString thing_filename=GetThingFilenameFromLevelFilename(Maps[region_maps[c1]].LevelName.ToWideString());
        CCountedPointer<CAFile> pmap_file;
        CCountedPointer<CAFile> pthing_file;
        Maps[index].LevStamp=CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(Maps[region_maps[c1]].LevelName.ToWideString());
        Maps[index].TngStamp=CAFile::GetFileModifiedTimeAsSomeWackyWin32BigNumber(thing_filename);
        if(UseLevelWADBank) {
            pmap_file.Reset(new CWADFile(*LevelWADBank,Maps[region_maps[c1]].LevelName.ToWideString(),FILE_ACCESS_READ,2));
            pthing_file.Reset(new CWADFile(*LevelWADBank,thing_filename,FILE_ACCESS_READ,2));
        } else {
            pmap_file.Reset(new CDiskFileWin32(Maps[region_maps[c1]].LevelName.ToWideString(),FILE_ACCESS_READ,2));
            pthing_file.Reset(new CDiskFileWin32(thing_filename,FILE_ACCESS_READ,2));
        }
        level_files.push_back(CLevelLoader::CObject::CLevelFiles());
        level_files.back().LevelID=region_maps[c1];
        level_files.back().PThingFile=pthing_file;
        level_files.back().PTopologyFile=pmap_file;
        if(load_navigation) level_files.back().PNavigationFile=pmap_file;
    }
    const std::vector<long>& visible_maps=Regions[index].GetVisMaps();
    for(long c1=0;c1<visible_maps.size();++c1) engine.SetMapVisibility(visible_maps[c1],true);
    CCountedPointer<CLevelLoader::CObject> pload_object=new CLevelLoader::CObject;
    std::set<CCharString> section_names=GetLoadableSections();
    pload_object->SetAsLoading(level_files,section_names,index);
    PLevelLoader->Add(pload_object);
    if(load_type==WAIT_FOR_LOAD) {
        for(;;) {
            if(!PLevelLoader->IsBusy()) break;
            CAsyncLoadingMemoryPool::ProcessLoading();
            PLevelLoader->RunSuspendableProcess();
        }
    }
    CQuestManager::Get().OnRegionLoaded(Regions[index].GetName());
}
