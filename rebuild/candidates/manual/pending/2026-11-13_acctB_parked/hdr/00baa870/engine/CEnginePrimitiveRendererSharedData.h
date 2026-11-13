#ifndef FABLE_ENGINE_CENGINEPRIMITIVERENDERERSHAREDDATA_H
#define FABLE_ENGINE_CENGINEPRIMITIVERENDERERSHAREDDATA_H
// New header (chunk 731, 2026-11-03; one model, no class macro). RTTI: CEnginePrimitiveRenderer (vtable 0x012A1348) :
// CEnginePrimitiveRendererSharedData (+0) : CBaseClassNonCopyable : CBaseClass, with CEngineComponent at +0x268, so the shared
// data is 0x268 bytes in retail (CEngine::Initialise 0x00B26360 converts the renderer to CEngineComponent* as `p ? p + 0x268 : 0`).
// Own members are not modelled yet (one opaque block).
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
// Opt-in (chunk 851, row 0x00BA1B00 DisplayMirroredTextures): the members that row reads, Ego_r.pdb names at the Ego_r offsets (the
// record has no container before +0x268, so retail = Ego_r: DynamicIB* +0xD0..+0xD8 feed CIndexBuffer::Initialise / Lock,
// PixelShaderDiffuse +0xF4 goes to ApplyPixelShader, HighlightMirroredTexturesEnabled +0x262 gates the pass); the rest stays opaque.
#include "engine/CPixelShader.h"
enum EIndexBufferPool;                                                   // MS forward enum (engine/CIndexBuffer.h)
class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
public:
    unsigned char    _pad_0x4[0xd0 - 0x4];                               // +0x04 members not modelled yet
    unsigned long    DynamicIBUsage;                                     // +0xd0
    EIndexBufferPool DynamicIBPool;                                      // +0xd4
    unsigned long    DynamicIBLockFlags;                                 // +0xd8
    CPixelShader     PixelShaderDiffuseOnly;                             // +0xdc
    CPixelShader     PixelShaderDiffuseColourTextureDiffuseAlpha;        // +0xe4
    CPixelShader     PixelShaderTextureOnly;                             // +0xec
    CPixelShader     PixelShaderDiffuse;                                 // +0xf4
    unsigned char    _pad_0xfc[0x262 - 0xfc];                            // +0xfc members not modelled yet
    bool             HighlightMirroredTexturesEnabled;                   // +0x262
    unsigned char    _pad_0x263[0x268 - 0x263];                          // +0x263 (LODForced +0x264)
};
#else
class CEnginePrimitiveRendererSharedData : public CBaseClassNonCopyable {
private:
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
protected:
    bool BlockOnLoad;
#ifdef FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_ZSPRITE
    // Overlay opt-in (row 0x00BAA870 RenderFromSortList): Ego_r.pdb ZSpriteDrawDistanceFactor +0x8C (read in place 0x00BAB394) and the
    // FableWin.pdb inline ?GetZSpriteDrawDistanceFactor@CEnginePrimitiveRendererSharedData@@QAEMXZ. No layout change.
    unsigned char _pad_0x5[0x8c - 0x5];
    float ZSpriteDrawDistanceFactor;                                     // +0x8c
    float MinimumZSpriteAsMeshDistance;                                  // +0x90
    float MaximumZSpriteAsMeshDistance;                                  // +0x94
    unsigned char _pad_0x98[0x268 - 0x98];
public:
    // FableWin.pdb inlines ?GetZSpriteDrawDistanceFactor / ?GetMinimumZSpriteAsMeshDistance / ?GetMaximumZSpriteAsMeshDistance
    // @CEnginePrimitiveRendererSharedData@@QAEMXZ (the landed GetTransitionFade 0x00BAA460 reads +0x90 / +0x94 in place).
    float GetZSpriteDrawDistanceFactor() { return ZSpriteDrawDistanceFactor; }
    float GetMinimumZSpriteAsMeshDistance() { return MinimumZSpriteAsMeshDistance; }
    float GetMaximumZSpriteAsMeshDistance() { return MaximumZSpriteAsMeshDistance; }
#else
    unsigned char _pad_0x5[0x263];
#endif
#else
    unsigned char _pad_0x4[0x264];                                       // +0x04 members not modelled yet
#endif
};
#endif
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveRendererSharedData) == 0x268);
#endif // FABLE_ENGINE_CENGINEPRIMITIVERENDERERSHAREDDATA_H
