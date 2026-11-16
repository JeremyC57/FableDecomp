// CEngineScene::RenderPrimitiveListToShadowBuffer(CEngineInternalPrimitiveBase* first, EEngineRenderLayer layer, const CShadowClipVolume&, CEngineSceneRenderStats&) -- walk
// Leaf: ?RenderPrimitiveListToShadowBuffer@CEngineScene@@AAEXPAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@ABVCShadowClipVolume@@AAVCEngineSceneRenderStats@@@Z
// Identity (naming wave 11): Ego_r.pdb's only (list, layer, CShadowClipVolume&, stats&) CEngineScene member; RenderSceneShadowBuffer 0x00B94060 calls it on both loose lists. the primitive chain: a primitive whose render-layer mask (+0x30) meets the mask is handed to its type's manager (slot 8) and the walk continues through its +0x38 link, otherwise through its +0x40 link
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B917E0, length 55 bytes.
// Callees: virtual slot 8 (+0x20) of the primitive's manager (`g_pEngine->m_managers[type]` @ 0x01436E84 + 0x10). Shape: `test esi,esi ; je` skips an empty chain, the
// mask stays in edi, the loop head is aligned with `mov edi,edi`; the two trailing arguments and `this` are unused (`ret 10h`).
class CEngineInternalPrimitiveBase;

// NF46 (round 7): the 0x01436E84 global is NEngineInternal::GPPrimitiveTypeInfo (Ego_r.pdb / FableWin.pdb
// ?GPPrimitiveTypeInfo@NEngineInternal@@3PAVCEnginePrimitiveTypeInfo@@A); the manager is its FableWin.pdb inline GetPrimitiveManager(type)
// (PrimitiveManagers +0x10, engine/CEnginePrimitiveTypeInfo.h, overlay opt-in _GETPRIMITIVEMANAGER) and slot 8 is
// CEnginePrimitiveManager::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase&) (engine/CEnginePrimitiveManager.h, Ego_r.pdb order).
// Were the address-named CEngine_01436e84 / g_pEngine_01436e84 and CEnginePrimitiveManagerBase_00b917e0 stand-ins.
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_GETPRIMITIVEMANAGER
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitiveManager.h"
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }   // 0x01436E84

class CEngineInternalPrimitiveBase { public:
    unsigned char                 _pad0[8];
    long                          m_type;          // +0x08
    unsigned char                 _pad0c[0x24];
    unsigned long                 RenderLayerMask; // +0x30
    unsigned char                 _pad34[4];
    CEngineInternalPrimitiveBase* m_pNextRendered; // +0x38
    unsigned char                 _pad3c[4];
    CEngineInternalPrimitiveBase* m_pNextSkipped;  // +0x40
};

enum EEngineRenderLayer { };
class CShadowClipVolume;
class CEngineSceneRenderStats;
class CEngineScene {
private:                                        // Ego_r.pdb: private (AAE)
    void RenderPrimitiveListToShadowBuffer(CEngineInternalPrimitiveBase* first, EEngineRenderLayer layerMask,
                                           const CShadowClipVolume& volume, CEngineSceneRenderStats& stats);
};

void CEngineScene::RenderPrimitiveListToShadowBuffer(CEngineInternalPrimitiveBase* first, EEngineRenderLayer layerMask,
                                                     const CShadowClipVolume& volume, CEngineSceneRenderStats& stats)
{
    CEngineInternalPrimitiveBase* primitive = first;
    while (primitive) {
        if (primitive->RenderLayerMask & layerMask) {
            NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(primitive->m_type)->RenderPrimitiveToShadowBuffer(*primitive);
            primitive = primitive->m_pNextRendered;
        } else {
            primitive = primitive->m_pNextSkipped;
        }
    }
}