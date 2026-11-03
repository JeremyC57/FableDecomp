#ifndef FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_H
#define FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_H
#ifdef FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
// Retail branch (one model, merged 2026-11-03: chunk 723b CShaderRenderManager::PrepareForRender + Codex pass 13 0x00989CB0 /
// 0x0098ACF0 / 0x009DA9F0; Codex's FABLE_RETAIL_SHADER_LIGHTING / _RENDER2D_TYPES variants folded in): the polymorphic class (RTTI : CBaseClass; ??1CVertexShaderConstantLayout@@UAE@XZ,
// virtual PeekName) with typed members: Ego_r.pdb names and offsets (the 28 CShaderConstantRange members hold no container, so
// retail = Ego_r up to ReservedPresets +0x140, a 12-byte STLport CArray<CVertexShaderConstant>: retail reads its start +0x140 and
// finish +0x144, 0x00B709BB). sizeof 0x14C.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/CBaseClass.h"
#include "engine/CShaderConstantRange.h"
#include "engine/CVertexShaderConstant.h"
class CVertexShaderConstantLayout : public CBaseClass {
    friend class CShaderRenderManager;
public:
    CVertexShaderConstantLayout();                                       // ??0CVertexShaderConstantLayout@@QAE@XZ
    virtual ~CVertexShaderConstantLayout();                              // ??1CVertexShaderConstantLayout@@UAE@XZ
    virtual CCharString PeekName() const;                                // ?PeekName@CVertexShaderConstantLayout@@UBE?AVCCharString@@XZ
    // Accessors (Ego_r.pdb / FableWin.pdb declarations, as Codex pass 13 cited them for 0x009DA9F0).
    CShaderConstantRange GetReserved() const;
    CShaderConstantRange GetUser() const;
    CShaderConstantRange GetCompression() const;
    CShaderConstantRange GetAmbient() const;
    CShaderConstantRange GetFogColour() const;
    CShaderConstantRange GetCameraPos() const;
    unsigned long GetLightSize() const;                                  // ?GetLightSize@CVertexShaderConstantLayout@@QBEKXZ
    CShaderConstantRange GetLightArray() const;
    CShaderConstantRange GetLightGlobals() const;
    CShaderConstantRange GetCombinedProjectionMatrix() const;
    CShaderConstantRange GetWorldMatrix() const;
    CShaderConstantRange GetSphereMapMatrix() const;
    CShaderConstantRange GetFogTransform() const;
    CShaderConstantRange GetTextureTransform(unsigned long) const;
    CShaderConstantRange GetBoneMatrices() const;
    CShaderConstantRange GetShadowTextureTransform();
    CShaderConstantRange GetShadowFadeTransform();
    CShaderConstantRange GetShadowFadeFactors();
    CShaderConstantRange GetShadowedSpotlightFalloffTransform();
    CShaderConstantRange GetShadowedSpotlightAttenuation();
    CShaderConstantRange GetShadowedSpotlightColour();
    CShaderConstantRange GetScreenSpaceTransformScale() const;
    CShaderConstantRange GetScreenSpaceTransformOffset() const;
    void SetReserved(const CShaderConstantRange&);
    void SetUser(const CShaderConstantRange&);
    void SetCompression(const CShaderConstantRange&);
    void SetAmbient(const CShaderConstantRange&);
    void SetFogColour(const CShaderConstantRange&);
    void SetCameraPos(const CShaderConstantRange&);
    void SetLightSize(unsigned long);
    void SetLightArray(const CShaderConstantRange&);
    void SetLightAttenuations(const CShaderConstantRange&);
    void SetLightGlobals(const CShaderConstantRange&);
    void SetCombinedProjectionMatrix(const CShaderConstantRange&);
    void SetWorldMatrix(const CShaderConstantRange&);
    void SetFogTransform(const CShaderConstantRange&);
    void SetSphereMapMatrix(const CShaderConstantRange&);
    void SetTextureTransform(unsigned long,const CShaderConstantRange&);
    void SetBoneMatrices(const CShaderConstantRange&);
    void SetShadowTextureTransform(const CShaderConstantRange&);
    void SetShadowFadeTransform(const CShaderConstantRange&);
    void SetShadowFadeFactors(const CShaderConstantRange&);
    void SetShadowedSpotlightFalloffTransform(const CShaderConstantRange&);
    void SetShadowedSpotlightAttenuation(const CShaderConstantRange&);
    void SetShadowedSpotlightColour(const CShaderConstantRange&);
    void SetScreenSpaceTransformScale(const CShaderConstantRange&);
    void SetScreenSpaceTransformOffset(const CShaderConstantRange&);
    void SetReservedPresets(const CArray<CVertexShaderConstant>&);
    bool Validate(const CShaderConstantRange&) const;
private:
    bool RangeValidate(const CShaderConstantRange&,const CShaderConstantRange&,CArray<bool>&) const;
    // Data: private in the PDB declarations; CShaderRenderManager (a friend) reads them in place (PrepareForRender,
    // UpdateObjectSpaceLighting 0x00989CB0).
    CShaderConstantRange Reserved;                          // +0x04
    CShaderConstantRange User;                              // +0x10
    CShaderConstantRange Compression;                       // +0x1c
    CShaderConstantRange Ambient;                           // +0x28
    CShaderConstantRange FogColour;                         // +0x34
    CShaderConstantRange CameraPos;                         // +0x40
    unsigned long        LightSize;                         // +0x4c
    CShaderConstantRange LightArray;                        // +0x50
    CShaderConstantRange LightGlobals;                      // +0x5c
    CShaderConstantRange LightAttenuations;                 // +0x68
    CShaderConstantRange CombinedProjectionMatrix;          // +0x74
    CShaderConstantRange FogTransform;                      // +0x80
    CShaderConstantRange WorldMatrix;                       // +0x8c
    CShaderConstantRange SphereMapMatrix;                   // +0x98
    CShaderConstantRange TextureTransform[4];               // +0xa4
    CShaderConstantRange BoneMatrices;                      // +0xd4
    CShaderConstantRange ShadowTextureTransform;            // +0xe0
    CShaderConstantRange ShadowFadeTransform;               // +0xec
    CShaderConstantRange ShadowFadeFactors;                 // +0xf8
    CShaderConstantRange ShadowedSpotlightFalloffTransform; // +0x104
    CShaderConstantRange ShadowedSpotlightAttenuation;      // +0x110
    CShaderConstantRange ShadowedSpotlightColour;           // +0x11c
    CShaderConstantRange ScreenSpaceTransformScale;         // +0x128
    CShaderConstantRange ScreenSpaceTransformOffset;        // +0x134
    CArray<CVertexShaderConstant> ReservedPresets;          // +0x140
};
FABLE_STATIC_ASSERT(sizeof(CVertexShaderConstantLayout) == 0x14c);
#else // flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CVertexShaderConstantLayout = 0x150.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CVertexShaderConstantLayout = 0x14c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CVertexShaderConstantLayout {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char Reserved[0xc];                             // +0x04 CShaderConstantRange
    unsigned char User[0xc];                                 // +0x10 CShaderConstantRange
    unsigned char Compression[0xc];                          // +0x1c CShaderConstantRange
    unsigned char Ambient[0xc];                              // +0x28 CShaderConstantRange
    unsigned char FogColour[0xc];                            // +0x34 CShaderConstantRange
    unsigned char CameraPos[0xc];                            // +0x40 CShaderConstantRange
    unsigned long LightSize;                                 // +0x4c
    unsigned char LightArray[0xc];                           // +0x50 CShaderConstantRange
    unsigned char LightGlobals[0xc];                         // +0x5c CShaderConstantRange
    unsigned char LightAttenuations[0xc];                    // +0x68 CShaderConstantRange
    unsigned char CombinedProjectionMatrix[0xc];             // +0x74 CShaderConstantRange
    unsigned char FogTransform[0xc];                         // +0x80 CShaderConstantRange
    unsigned char WorldMatrix[0xc];                          // +0x8c CShaderConstantRange
    unsigned char SphereMapMatrix[0xc];                      // +0x98 CShaderConstantRange
    unsigned char TextureTransform[0x30];                    // +0xa4 CShaderConstantRange[4]
    unsigned char BoneMatrices[0xc];                         // +0xd4 CShaderConstantRange
    unsigned char ShadowTextureTransform[0xc];               // +0xe0 CShaderConstantRange
    unsigned char ShadowFadeTransform[0xc];                  // +0xec CShaderConstantRange
    unsigned char ShadowFadeFactors[0xc];                    // +0xf8 CShaderConstantRange
    unsigned char ShadowedSpotlightFalloffTransform[0xc];    // +0x104 CShaderConstantRange
    unsigned char ShadowedSpotlightAttenuation[0xc];         // +0x110 CShaderConstantRange
    unsigned char ShadowedSpotlightColour[0xc];              // +0x11c CShaderConstantRange
    unsigned char ScreenSpaceTransformScale[0xc];            // +0x128 CShaderConstantRange
    unsigned char ScreenSpaceTransformOffset[0xc];           // +0x134 CShaderConstantRange
    unsigned char ReservedPresets[0xc];                      // +0x140 CArray<CVertexShaderConstant>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CVertexShaderConstantLayout) == 0x14c);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, Reserved) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, User) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, Compression) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, Ambient) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, FogColour) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, CameraPos) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, LightSize) == 0x4c);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, LightArray) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, LightGlobals) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, LightAttenuations) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, CombinedProjectionMatrix) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, FogTransform) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, WorldMatrix) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, SphereMapMatrix) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, TextureTransform) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, BoneMatrices) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, ShadowTextureTransform) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, ShadowFadeTransform) == 0xec);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, ShadowFadeFactors) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, ShadowedSpotlightFalloffTransform) == 0x104);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, ShadowedSpotlightAttenuation) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, ShadowedSpotlightColour) == 0x11c);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, ScreenSpaceTransformScale) == 0x128);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, ScreenSpaceTransformOffset) == 0x134);
FABLE_STATIC_ASSERT(offsetof(CVertexShaderConstantLayout, ReservedPresets) == 0x140);

#endif // FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT

#endif // FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_H
