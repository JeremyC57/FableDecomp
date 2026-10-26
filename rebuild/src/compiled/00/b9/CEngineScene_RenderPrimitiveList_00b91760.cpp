// CEngineScene::RenderPrimitiveList(CEngineInternalPrimitiveBase*, EEngineRenderLayer, const CEngineCamera&, CEngineSceneRenderStats&) -- walks a primitive list: each enabled primitive of the layer that passes its visibility test is handed to its type's manager, and a primitive the manager rendered records the frame number
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B91760, length 126 bytes (+ int3 padding).
// Signature from Ego_r.pdb: ?RenderPrimitiveList@CEngineScene@@AAEXPAVCEngineInternalPrimitiveBase@@W4EEngineRenderLayer@@ABVCEngineCamera@@AAVCEngineSceneRenderStats@@@Z
// (`ret 10h`; `this` and the stats are unused).
// Callees: the primitive's virtual slot 20 (+0x50, visibility test (camera, 0) -> bool), the manager's virtual slot 7
// (+0x1c, RenderPrimitive(primitive, layer, chain) -> ERendered: 2 = rendered). Globals: 0x01436E84 the primitive
// type info (engine/CEnginePrimitiveTypeInfo.h: PrimitiveManagers[] at +0x10), 0x0143B8D8 the engine frame counter.
// Layout: CEngineInternalPrimitiveBase (engine header / PDB): Type +8, RenderLayerMask +0x30, the Enable bit (bit 0
// of +0x34), LastFrameRendered +0x36, NextPrimitive +0x38, NextLayerMask +0x40; modelled locally because the
// virtual calls need a polymorphic class (the generated header is a flat pack(1) record).
// Shape: a rotated while loop (aligned head) advancing through NextPrimitive for a primitive on the layer and
// through NextLayerMask otherwise; the type's manager is read into a local BEFORE the 8-byte sub-primitive chain is
// declared, which lets VC sink the chain's zero stores below the first argument pushes (a chain declared first, or a
// chain temporary in the call, is zeroed before the manager load); one zero register (ebp) serves the null tests,
// the 0 argument and the chain.
#include "engine/CEnginePrimitiveTypeInfo.h"

class CEngineCamera;
class CEngineSceneRenderStats;
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT = 0 };
enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };

class CEngineInternalPrimitiveBase;
class CEngineSubPrimitiveChain {                         // first / last sub-primitive
public:
    void* m_pFirst;
    void* m_pLast;
    CEngineSubPrimitiveChain() : m_pFirst(0), m_pLast(0) { }
};
class CEnginePrimitiveManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                      CEngineSubPrimitiveChain& chain);   // slot 7 (+0x1c)
};

class CEngineInternalPrimitiveBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual bool IsVisible(const CEngineCamera& camera, long flags);   // slot 20 (+0x50)
    long                          m_refCount;            // +0x04
    long                          Type;                  // +0x08 EEnginePrimitiveType
    unsigned char                 _pad0c[0x24];
    unsigned long                 RenderLayerMask;       // +0x30
    unsigned char                 Enable : 1;            // +0x34 bit 0
    unsigned char                 NonPersistent : 1;
    unsigned char                 OverrideFadeDistance : 1;
    unsigned char                 PostRenderEnable : 1;
    unsigned short                LastFrameRendered;     // +0x36
    CEngineInternalPrimitiveBase* NextPrimitive;         // +0x38
    void*                         RefPrimitive;          // +0x3c
    CEngineInternalPrimitiveBase* NextLayerMask;         // +0x40
    void*                         RefLayerMask;          // +0x44
};

extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84
extern unsigned short            g_engineFrameNumber;          // 0x0143B8D8

class CEngineScene {
public:
    void RenderPrimitiveList(CEngineInternalPrimitiveBase* primitive, EEngineRenderLayer layer,
                             const CEngineCamera& camera, CEngineSceneRenderStats& stats);
};

void CEngineScene::RenderPrimitiveList(CEngineInternalPrimitiveBase* primitive, EEngineRenderLayer layer,
                                       const CEngineCamera& camera, CEngineSceneRenderStats& stats)
{
    while (primitive) {
        if (primitive->RenderLayerMask & layer) {
            if (primitive->Enable && primitive->IsVisible(camera, 0)) {
                CEnginePrimitiveManager* manager = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive->Type];
                CEngineSubPrimitiveChain chain;
                if (manager->RenderPrimitive(*primitive, layer, chain) == RENDERED_SUB_PRIMITIVES)
                    primitive->LastFrameRendered = g_engineFrameNumber;
            }
            primitive = primitive->NextPrimitive;
        }
        else
            primitive = primitive->NextLayerMask;
    }
}