#ifndef FABLE_ENGINE_CENGINEMESHEFFECT_H
#define FABLE_ENGINE_CENGINEMESHEFFECT_H
// New (chunk 892, row 0x00BF01E0 CEnginePrimitiveRenderUtil::SetupOutlineSmoothAlphaStencilMaskStates). Opt-in only: without
// FABLE_RETAIL_CENGINEMESHEFFECT this header declares nothing.
// CEngineMeshEffect (Ego_r.pdb, 0xC bytes: two leading dwords, MeshEffect +0x8) is the pointee of CIVCountedPointer<CEngineMeshEffect>
// (CEngineMeshEffect members of the mesh primitives), so its first 8 bytes are the CIVCountedPointeeBase (vptr + IVRefCount);
// RTTI .?AVCEngineMeshEffect@@, .?AVCEngineMeshEffectOutlineBase@@, .?AVCEngineMeshEffectOutlineSmooth@@. CEngineMeshEffectOutlineBase
// (Ego_r.pdb, 0x1C bytes): WidthWorldSpace +0xC, WidthScreenSpace +0x10, Colour +0x14, StencilBit +0x18, TextureAlphaRef +0x19,
// TextureAlphaBias +0x1A (0x00BF01F1 `movzx edi,byte ptr [ebp+19h]`, 0x00BF1030 reads StencilBit +0x18). CEngineMeshEffectOutline is
// the parameter type of ?SetupOutlineSmoothAlphaStencilMaskStates@CEnginePrimitiveRenderUtil@@QAEXABVCEngineMeshEffectOutline@@@Z
// (no Ego_r layout of its own; modelled as the outline base with no members of its own). EEngineMeshEffect values are not recovered.
#ifdef FABLE_RETAIL_CENGINEMESHEFFECT
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CRGBColour.h"
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
enum EEngineMeshEffect { MESH_EFFECT_OUTLINE_SMOOTH=0,MESH_EFFECT_OUTLINE=1,MESH_EFFECT_DISTANCE_FADE=2 };
#else
enum EEngineMeshEffect { };
#endif
class CEngineMeshEffect : public CIVCountedPointeeBase {
public:
 #ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
 EEngineMeshEffect GetEffect()const{return MeshEffect;}
protected:
 #endif
    EEngineMeshEffect MeshEffect;                        // +0x08
};

#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Original Ego_r/FableWin TPI and module916 call sites; private evidence in00ba115c.
class CEngineMeshEffectDistanceFade:public CEngineMeshEffect {
public:
 float GetAlphaAtDistance(float)const;
 float NearClipDistance,NearFadeDistance,FarFadeDistance,FarClipDistance;
};
#endif
class CEngineMeshEffectOutlineBase : public CEngineMeshEffect {
public:
    float         WidthWorldSpace;                       // +0x0c
    float         WidthScreenSpace;                      // +0x10
    CRGBColour    Colour;                                // +0x14
    unsigned char StencilBit;                            // +0x18
    unsigned char TextureAlphaRef;                       // +0x19
    unsigned char TextureAlphaBias;                      // +0x1a
};
class CEngineMeshEffectOutline : public CEngineMeshEffectOutlineBase {
};
#ifdef FABLE_RETAIL_CENGINEMESHEFFECT_OUTLINESMOOTH
// Nested opt-in (agentNF11, row 0x004C04C0 CTCGraphicAppearance::SetAsHighlighted): the smooth outline effect (RTTI
// .?AVCEngineMeshEffectOutlineSmooth@@; `new` 0x1C bytes = the outline base). Ego_r.pdb / FableWin.pdb constructor
// ??0CEngineMeshEffectOutlineSmooth@@QAE@MMABVCRGBColour@@EJJ@Z (out of line at 0x004C3000, landed: the two widths, the colour, the
// stencil byte and the two texture alpha values as long). Declaration only.
class CEngineMeshEffectOutlineSmooth : public CEngineMeshEffectOutlineBase {
public:
    CEngineMeshEffectOutlineSmooth(float widthWorldSpace, float widthScreenSpace, const CRGBColour& colour, unsigned char stencilBit,
                                   long textureAlphaRef, long textureAlphaBias);   // 0x004C3000
};
#endif
#ifdef FABLE_RETAIL_CENGINEMESHEFFECT_PASS22_DISTANCEFADE
class CEngineMeshEffectDistanceFade:public CEngineMeshEffect {
public:
    CEngineMeshEffectDistanceFade(float,float,float,float);
    float NearClipDistance,NearFadeDistance,FarFadeDistance,FarClipDistance;
};
#endif
FABLE_STATIC_ASSERT_AT(47, sizeof(CEngineMeshEffect) == 0xc);
FABLE_STATIC_ASSERT_AT(48, sizeof(CEngineMeshEffectOutlineBase) == 0x1c);
FABLE_STATIC_ASSERT_AT(49, offsetof(CEngineMeshEffectOutlineBase, StencilBit) == 0x18);
FABLE_STATIC_ASSERT_AT(50, offsetof(CEngineMeshEffectOutlineBase, TextureAlphaRef) == 0x19);
#endif // FABLE_RETAIL_CENGINEMESHEFFECT
#endif // FABLE_ENGINE_CENGINEMESHEFFECT_H
