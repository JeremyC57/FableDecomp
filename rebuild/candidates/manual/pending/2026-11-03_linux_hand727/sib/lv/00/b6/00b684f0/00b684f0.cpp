// CEngineLandscapeRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams&) -- update loaded maps and foreground textures; return false if any update failed
// Retail 0x00B684F0, 273 bytes. Vtable 0x012A2B54 slot 3 confirms the owner.
// Actual CEngineLandscapeRenderer, CEngineComponent and CEngineLandscapeMap PDB headers are included.
// Native scene-state projections: global 0x01436E8C +0x2c is the sentinel list of map-counted-pointer records;
// global 0x01436EA8 +0x6b0 is CEngineLandscapeMemoryManager. The latter's actual header names its
// three disposal lists; TexturePool_Clear(0x00BD9CE0) drains them. A byte at renderer +0x63c gates
// background-LOD requests; in the retail layout (engine/CEngineLandscapeRenderer.h, TLC MeshBuilder 0x484) it is byte 0x34
// of the opaque Settings (CLandscapeSettings, +0x608), not part of MeshBuilder as the Ego_r layout suggested. Map update helpers and statuses are native
// addresses 0x00BDC180 (LOD), 0x00BDC510 (foreground), 0x00BDC020 (background tessellation), and
// 0x00B68070 (foreground textures). Status values 3=failed, 2=pending.
#include "engine/CEngineComponent.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/_quarantine/CEngineLandscapeMap.h"

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

struct CEngineLandscapeRendererCallView : CEngineLandscapeRenderer
{
    bool LoadNewScene(const CEngineComponentLoadNewSceneParams& params);
    bool LoadForegroundTextures(bool forceLoad); // 0x00B68070
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

struct CEngineMapReference
{
    CEngineLandscapeMap* Data;
    void* ReferenceInfo;
};

struct CEngineMapListNode
{
    CEngineMapListNode* Next;
    CEngineMapListNode* Previous;
    CEngineMapReference* Value;
};

struct CEngineMapList
{
    CEngineMapListNode* Head;
    unsigned int Size;
};

struct CEngineSceneState_01436E8C
{
    unsigned char _pad_0x00[0x2c];
    CEngineMapList LoadedLandscapeMaps;
};

struct CEngineGlobal_01436EA8
{
    unsigned char _pad_0x00[0x6b0];
    CEngineLandscapeMemoryManager* LandscapeMemoryManager;
};

extern CEngineSceneState_01436E8C* g_pEngineSceneState_01436E8C; // 0x01436E8C
extern CEngineGlobal_01436EA8* g_pEngineGlobal_01436EA8; // 0x01436EA8

bool CEngineLandscapeRendererCallView::LoadNewScene(const CEngineComponentLoadNewSceneParams& params)
{
    if (!reinterpret_cast<CEngineComponent*>(this)->RenderEnabled)
        return false;

    bool updatePending = false;
    CEngineGlobal_01436EA8* global = g_pEngineGlobal_01436EA8;
    reinterpret_cast<CEngineLandscapeMemoryManagerCallView*>(
        global->LandscapeMemoryManager)->ClearTexturePool();

    CEngineSceneState_01436E8C* const sceneState = g_pEngineSceneState_01436E8C;
    const bool updateBackgroundLOD = Settings[0x34] != 0;   // +0x63c: CLandscapeSettings at +0x608 in the retail header (not MeshBuilder)
    CEngineMapList* const maps = &sceneState->LoadedLandscapeMaps;

    if (updateBackgroundLOD && (params.Flags & 2u) == 0)
    {
        for (CEngineMapListNode* node = maps->Head->Next; node != maps->Head; node = node->Next)
        {
            CEngineMapReference* const reference = node->Value;
            CEngineLandscapeMapCallView* const map =
                reinterpret_cast<CEngineLandscapeMapCallView*>(reference->Data);
            const LS_LOADING_STATUS status =
                map->UpdateBackgroundLOD(
                    *reinterpret_cast<CLandscapeGPUUpdateQueue*>(GPUUpdateQueue),
                    1.0e30f,
                    true);
            if (status == LS_LOADING_FAILED)
                return false;
            if (status == LS_LOADING_PENDING)
                updatePending = true;
        }
    }

    for (CEngineMapListNode* node = maps->Head->Next; node != maps->Head; node = node->Next)
    {
        CEngineMapReference* const reference = node->Value;
        CEngineLandscapeMapCallView* const map =
            reinterpret_cast<CEngineLandscapeMapCallView*>(reference->Data);
        const LS_LOADING_STATUS status =
            map->UpdateForeground();
        if (status == LS_LOADING_FAILED)
            return false;
        if (status == LS_LOADING_PENDING)
            updatePending = true;
    }

    if (!updatePending && (params.Flags & 2u) == 0)
    {
        for (CEngineMapListNode* node = maps->Head->Next; node != maps->Head; node = node->Next)
        {
            CEngineMapReference* const reference = node->Value;
            CEngineLandscapeMapCallView* const map =
                reinterpret_cast<CEngineLandscapeMapCallView*>(reference->Data);
            map->UpdateBackgroundTesselation();
        }
    }

    bool foregroundLoaded =
        reinterpret_cast<CEngineLandscapeRendererCallView*>(this)->LoadForegroundTextures(true);
    foregroundLoaded |= updatePending;
    return foregroundLoaded;
}