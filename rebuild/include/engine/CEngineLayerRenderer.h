#ifndef FABLE_ENGINE_CENGINELAYERRENDERER_H
#define FABLE_ENGINE_CENGINELAYERRENDERER_H
// New header (chunk 731, 2026-11-03; one model, no class macro). RTTI .?AVCEngineLayerRenderer@@ : CBaseClassNonCopyable :
// CBaseClass (vtable 0x012A0F04). Ego_r.pdb layout (0x20 with Dinkumware's 16-byte CArray; retail 0x1C: CEngine::Initialise
// 0x00B26360 allocates 0x1C): Layer +0x04, Flags +0x08, SceneFilterFlags +0x0C, Components +0x10 (CArray<CEngineComponent*>).
// Functions (Ego_r.pdb / FableWin.pdb): the constructor ??0CEngineLayerRenderer@@QAE@W4EEngineRenderLayer@@W4ERenderRenderLayerFlags@@K@Z
// is inline (0x00B26A51: CBaseClassNonCopyable() 0x0099A2F0, the vtable, the three fields, the empty vector); AddComponent
// 0x00B2AC80 is out of line; operator<< (??6CEngineLayerRenderer@@QAEAAV0@PAVCEngineComponent@@@Z) is the inline chaining form
// CEngine::Initialise expands (`call 0x00B2AC80` on the returned layer, once per component).
// EEngineRenderLayer (one bit per layer) and ERenderRenderLayerFlags: no enumerator names are recoverable from the repo's PDB
// extracts, so both are declared without enumerators (the ESystemReturn convention).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CArray.h"
enum EEngineRenderLayer;
enum ERenderRenderLayerFlags { };
class CEngineComponent;
class CEngineComponentParams;
class CEngineLayerRenderer : public CBaseClassNonCopyable {
public:
    CEngineLayerRenderer(EEngineRenderLayer layer, ERenderRenderLayerFlags flags, unsigned long scene_filter_flags)
        : Layer(layer), Flags(flags), SceneFilterFlags(scene_filter_flags) { }
    virtual ~CEngineLayerRenderer();                                     // ??1CEngineLayerRenderer@@UAE@XZ
    virtual void Render(const CEngineComponentParams& params);           // ?Render@CEngineLayerRenderer@@UAEXABVCEngineComponentParams@@@Z
    void AddComponent(CEngineComponent* component);                      // 0x00B2AC80 ?AddComponent@CEngineLayerRenderer@@QAEXPAVCEngineComponent@@@Z
    CEngineLayerRenderer& operator<<(CEngineComponent* component) { AddComponent(component); return *this; }
private:
    EEngineRenderLayer       Layer;                                      // +0x04
    ERenderRenderLayerFlags  Flags;                                      // +0x08
    unsigned long            SceneFilterFlags;                           // +0x0c
    CArray<CEngineComponent*> Components;                                // +0x10
};
FABLE_STATIC_ASSERT(sizeof(CEngineLayerRenderer) == 0x1c);
#endif // FABLE_ENGINE_CENGINELAYERRENDERER_H
