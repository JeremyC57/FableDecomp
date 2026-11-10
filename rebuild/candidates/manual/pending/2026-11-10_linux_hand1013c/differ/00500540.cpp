// CWorldMap::LoadRegion(long, ELoadType, bool) -- apply region effects, open level files and queue the region loader.
// Grade: DIFFER(2396v2412) -- register / frame allocation: region_maps in ebp and c1 in ebx (retail ebx / ebp), the level_files end pointer kept in esi across the back() stores (retail reloads it after every store), frame 0x94 v 0x98 (the IntelligentPointer temp and the GetTC key 4 / 0x10 bytes lower), and the wait loop rotated (retail jumps back to an aligned top-test) (461 diff lines)
// Chunk 1013c: not graded -- SRC_FAIL under the current headers (CEnvironment has no SetRegionTheme / SetTheme any more: the prior's FABLE_RETAIL_CENVIRONMENT_THEMEWEIGHT opt-in is gone). Retail calls ONE function, 0x006B3600, for both environment calls (0x005005AA and 0x005005D5), so the prior's two names for it are wrong; that name must be proven before the overlay is rebuilt. Header debt before any grade: a shared engine/CRegionDef.h (retail offsets = Ego_r less 4 before SoundMap, less 12 after), CGameTimeManager, CTCDParticleEmitter, CAsyncLoadingMemoryPool.
// Next idea: the Codex pass-13 shape families (scoped loader phase, explicit break / goto / while wait loop, back() reference, helper bodies) are all spent; next try a by-pointer CLevelFiles* or an iterator into level_files after push_back so the end pointer escapes, and the STLport vector::back() spelled as *(end() - 1) through a by-value iterator
// Chunk 1013b: not worked (no grade); prior unchanged.
// Leaf: ?LoadRegion@CWorldMap@@QAEXJW4ELoadType@1@_N@Z
#define FABLE_RETAIL_CVECTORMAP_ORDINARY_LOWERBOUND
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CREGION_PREGIONDEF
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CLEVELLOADER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
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
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCAMERA
#define FABLE_RETAIL_CTHING_CUTSCENEPAUSE
#define FABLE_RETAIL_CENVIRONMENT_THEMEWEIGHT
#define FABLE_RETAIL_CAFILE_MODIFIEDTIME
#define FABLE_RETAIL_CQUESTMANAGER_REGIONLOADED
#define FABLE_RETAIL_CWORLDMAP_THINGFILENAME
#define FABLE_RETAIL_CWADFILE
#define FABLE_RETAIL_CLEVELFILES_IMPLICIT
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CMapInfo.h"
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
#include "engine/CAIGameCameraBase.h"
#include "engine/CLevelLoader.h"
#include "engine/CQuestManager.h"
#include "engine/CCamera.h"
#include "engine/CTCBase.h"
#include "fable_game.h"
// Ego_r.pdb CRegionDef at the retail offsets (0x00500540 reads +0x3c, +0x40, +0x44, +0x88, +0x8c, +0x90, +0x94, +0xa4: Ego_r's
// layout less 4 before SoundMap and less 12 after it); no shared header.
class CRegionDef : public CDefPointeeBase {
public:
    unsigned char _pad1c[0x3c - 0x1c];
    long EnvironmentTheme;                                       // +0x3c
    long InternalEnvironmentTheme;                               // +0x40
    long InternalLightingChannelCount;                           // +0x44
    unsigned char _pad48[0x88 - 0x48];
    bool UseVillageTime;                                         // +0x88
    bool AllowHenchmen;                                          // +0x89
    long AttachToCameraFX;                                       // +0x8c
    bool EnableSpotlightShadowMode;                              // +0x90
    bool CheckAllThingsOnMapForCombatCollision;                  // +0x91
    long HeroBreathParticle;                                     // +0x94
    unsigned char _pad98[0xa4 - 0x98];
    bool HasSea;                                                 // +0xa4
};
class CGameTimeManager {
public:
    void SetAsUsingVillageTime(bool value);                      // 0x006BB2F0 ?SetAsUsingVillageTime@CGameTimeManager@@QAEX_N@Z
};
class CTCDParticleEmitter : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0x4b; }
    static CThing* __fastcall Create(long def_index, const C3DVector& pos, bool flag);   // 0x006E0880
    void AttachToCamera(long priority, float time);              // 0x006E0DB0
    void AttachToThing(CThing& thing, long priority, const CCharString& dummy_name, long flags, float time);   // 0x006E0BE0
};
class CAsyncLoadingMemoryPool {
public:
    static void __fastcall ProcessLoading();                     // 0x00A14900 ?ProcessLoading@CAsyncLoadingMemoryPool@@SIXXZ
};
extern const CCharString MOUTH_DUMMY_OBJECT_NAME;
// The original Reset specialization is out of line in retail; its complete body
// exposes pointer lifetime information to the caller without changing its public name.
template <> __declspec(noinline) void CCountedPointer<CAFile>::Reset(CAFile* data)
{
    ReleaseData();
    Data = data;
    if (data) Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
}
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
