// Flags: 
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_LOADFOREGROUND_PASS30
// CEngineLandscapeRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams&) -- update loaded maps and foreground textures; return false if any update failed
// Leaf: ?LoadNewScene@CEngineLandscapeRenderer@@UAE_NABVCEngineComponentLoadNewSceneParams@@@Z
// Retail 0x00B684F0, 273 bytes. Vtable 0x012A2B54 slot 3 confirms the owner.
// Actual CEngineLandscapeRenderer, CEngineComponent and CEngineLandscapeMap PDB headers are included.
// Native scene-state projections: global 0x01436E8C +0x2c is the sentinel list of map-counted-pointer records;
// global 0x01436EA8 +0x6b0 is CEngineLandscapeMemoryManager. The latter's actual header names its
// three disposal lists; TexturePool_Clear(0x00BD9CE0) drains them. A byte at renderer +0x63c gates
// background-LOD requests; in the retail layout (engine/CEngineLandscapeRenderer.h, TLC MeshBuilder 0x484) it is byte 0x34
// of the opaque Settings (CLandscapeSettings, +0x608), not part of MeshBuilder as the Ego_r layout suggested. Map update helpers and statuses are native
// addresses 0x00BDC180 (LOD), 0x00BDC510 (foreground), 0x00BDC020 (background tessellation), and
// 0x00B68070 (foreground textures). Status values 3=failed, 2=pending.
// NF46 (round 7): the 0x01436E8C global is NEngineInternal::GPEngineWorldMap (Ego_r.pdb / FableWin.pdb
// ?GPEngineWorldMap@NEngineInternal@@3PAVCEngineWorldMap@@A); its +0x2C list is the Ego_r.pdb member VisibleMapList (std::list<CEngineMap*>,
// engine/CEngineWorldMap.h overlay opt-in _VISIBLEMAPLIST) and each node's map record is a CEngineMap whose +0 LandscapeMap is read through
// the FableWin.pdb inline GetLandscapeMap (engine/CEngineMap.h FABLE_RETAIL_CENGINEMAP). Were the CEngineSceneState_01436E8C /
// g_pEngineSceneState_01436E8C, CEngineMapList / CEngineMapListNode / CEngineMapReference stand-ins. The component is the retail
// engine/CEngineComponent.h (its public FableWin.pdb inline PeekRenderEnabled for the +0x08 flag).
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEWORLDMAP
#define FABLE_RETAIL_CENGINEWORLDMAP_VISIBLEMAPLIST
#define FABLE_RETAIL_CENGINEMAP
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER   // NEngineInternal::GPLandscapeRenderer 0x01436EA8 (landed opt-in; was the CEngineGlobal_01436EA8 stand-in, NF271)
#include "engine/CEngineComponent.h"
#include "engine/CEngineWorldMap.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"
#include "engine/CEngineMap.h"

enum LS_LOADING_STATUS
{
    LS_LOADING_READY = 0,
    LS_LOADING_PENDING = 2,
    LS_LOADING_FAILED = 3
};

class CLandscapeGPUUpdateQueue;
class CEngineComponentLoadNewSceneParams;

struct CEngineLandscapeMapCallView : CEngineLandscapeMap
{
    LS_LOADING_STATUS UpdateBackgroundLOD(
        CLandscapeGPUUpdateQueue& queue, double distanceToCamera, bool forceUpdate); // 0x00BDC180
    LS_LOADING_STATUS UpdateForeground(); // 0x00BDC510
    void UpdateBackgroundTesselation(); // 0x00BDC020
};

struct CEngineLandscapeMemoryManagerCallView : CEngineLandscapeMemoryManager
{
    void ClearTexturePool(); // 0x00BD9CE0
};

class CEngineComponentParams
{ public:
    float FrameInterpolation;
    unsigned char InterpolationInfo[0x70];
    unsigned int FrameID;
    unsigned int SceneFilterFlags;
    bool OffWorldTickProcessFlag;
    unsigned char _pad_0x7d[3];
};

class CEngineComponentLoadNewSceneParams
{ public:
    CEngineComponentParams Params;
    void* Camera;
    float Progress;
    unsigned int Flags;
};

namespace NEngineInternal { extern CEngineWorldMap* GPEngineWorldMap; }   // 0x01436E8C

bool CEngineLandscapeRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams& params)
{
    if (!PeekRenderEnabled())
        return false;

    bool updatePending = false;
    CEngineLandscapeRenderer* global = NEngineInternal::GPLandscapeRenderer;
    reinterpret_cast<CEngineLandscapeMemoryManagerCallView*>(
        global->MemoryManager)->ClearTexturePool();

    CEngineWorldMap* const sceneState = NEngineInternal::GPEngineWorldMap;
    const bool updateBackgroundLOD = Settings.EnableUpdate;   // +0x63c: CLandscapeSettings at +0x608 in the retail header (not MeshBuilder)
    std::list<CEngineMap*>* const maps = &sceneState->VisibleMapList;

    if (updateBackgroundLOD && (params.Flags & 2u) == 0)
    {
        for (std::list<CEngineMap*>::iterator it = maps->begin(); it != maps->end(); ++it)
        {
            CEngineLandscapeMapCallView* const map =
                reinterpret_cast<CEngineLandscapeMapCallView*>((*it)->GetLandscapeMap());
            const LS_LOADING_STATUS status =
                map->UpdateBackgroundLOD(
                    GPUUpdateQueue,
                    1.0e30f,
                    true);
            if (status == LS_LOADING_FAILED)
                return false;
            if (status == LS_LOADING_PENDING)
                updatePending = true;
        }
    }

    for (std::list<CEngineMap*>::iterator it = maps->begin(); it != maps->end(); ++it)
    {
        CEngineLandscapeMapCallView* const map =
            reinterpret_cast<CEngineLandscapeMapCallView*>((*it)->GetLandscapeMap());
        const LS_LOADING_STATUS status =
            map->UpdateForeground();
        if (status == LS_LOADING_FAILED)
            return false;
        if (status == LS_LOADING_PENDING)
            updatePending = true;
    }

    if (!updatePending && (params.Flags & 2u) == 0)
    {
        for (std::list<CEngineMap*>::iterator it = maps->begin(); it != maps->end(); ++it)
        {
            CEngineLandscapeMapCallView* const map =
                reinterpret_cast<CEngineLandscapeMapCallView*>((*it)->GetLandscapeMap());
            map->UpdateBackgroundTesselation();
        }
    }

    bool foregroundLoaded =
        LoadForegroundTextures(true);
    foregroundLoaded |= updatePending;
    return foregroundLoaded;
}