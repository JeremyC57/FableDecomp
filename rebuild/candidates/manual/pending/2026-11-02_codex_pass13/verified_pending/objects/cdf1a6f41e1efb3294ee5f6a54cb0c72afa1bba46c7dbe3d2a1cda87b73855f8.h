#ifdef FABLE_RETAIL_RENDER2D_TYPES
#ifndef FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_RENDER2D_H
#define FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_RENDER2D_H
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#include "engine/CShaderConstantRange.h"
#include "engine/CArray.h"
#include "engine/CVertexShaderConstant.h"
class CCharString;
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CVertexShaderConstantLayout : public CBaseClass {
friend class CShaderRenderManager;
private: CShaderConstantRange Reserved;
private: CShaderConstantRange User;
private: CShaderConstantRange Compression;
private: CShaderConstantRange Ambient;
private: CShaderConstantRange FogColour;
private: CShaderConstantRange CameraPos;
private: unsigned long LightSize;
private: CShaderConstantRange LightArray;
private: CShaderConstantRange LightGlobals;
private: CShaderConstantRange LightAttenuations;
private: CShaderConstantRange CombinedProjectionMatrix;
private: CShaderConstantRange FogTransform;
private: CShaderConstantRange WorldMatrix;
private: CShaderConstantRange SphereMapMatrix;
private: CShaderConstantRange TextureTransform[4];
private: CShaderConstantRange BoneMatrices;
private: CShaderConstantRange ShadowTextureTransform;
private: CShaderConstantRange ShadowFadeTransform;
private: CShaderConstantRange ShadowFadeFactors;
private: CShaderConstantRange ShadowedSpotlightFalloffTransform;
private: CShaderConstantRange ShadowedSpotlightAttenuation;
private: CShaderConstantRange ShadowedSpotlightColour;
private: CShaderConstantRange ScreenSpaceTransformScale;
private: CShaderConstantRange ScreenSpaceTransformOffset;
private: CArray<CVertexShaderConstant> ReservedPresets;
public: CVertexShaderConstantLayout();
public: virtual CCharString PeekName() const;
public: CShaderConstantRange GetReserved() const;
public: CShaderConstantRange GetUser() const;
public: CShaderConstantRange GetCompression() const;
public: CShaderConstantRange GetAmbient() const;
public: CShaderConstantRange GetFogColour() const;
public: CShaderConstantRange GetCameraPos() const;
public: unsigned long GetLightSize() const;
public: CShaderConstantRange GetLightArray() const;
public: CShaderConstantRange GetLightGlobals() const;
public: CShaderConstantRange GetCombinedProjectionMatrix() const;
public: CShaderConstantRange GetWorldMatrix() const;
public: CShaderConstantRange GetSphereMapMatrix() const;
public: CShaderConstantRange GetFogTransform() const;
public: CShaderConstantRange GetTextureTransform(unsigned long) const;
public: CShaderConstantRange GetBoneMatrices() const;
public: CShaderConstantRange GetShadowTextureTransform();
public: CShaderConstantRange GetShadowFadeTransform();
public: CShaderConstantRange GetShadowFadeFactors();
public: CShaderConstantRange GetShadowedSpotlightFalloffTransform();
public: CShaderConstantRange GetShadowedSpotlightAttenuation();
public: CShaderConstantRange GetShadowedSpotlightColour();
public: CShaderConstantRange GetScreenSpaceTransformScale() const;
public: CShaderConstantRange GetScreenSpaceTransformOffset() const;
public: void SetReserved(const CShaderConstantRange &);
public: void SetUser(const CShaderConstantRange &);
public: void SetCompression(const CShaderConstantRange &);
public: void SetAmbient(const CShaderConstantRange &);
public: void SetFogColour(const CShaderConstantRange &);
public: void SetCameraPos(const CShaderConstantRange &);
public: void SetLightSize(unsigned long);
public: void SetLightArray(const CShaderConstantRange &);
public: void SetLightAttenuations(const CShaderConstantRange &);
public: void SetLightGlobals(const CShaderConstantRange &);
public: void SetCombinedProjectionMatrix(const CShaderConstantRange &);
public: void SetWorldMatrix(const CShaderConstantRange &);
public: void SetFogTransform(const CShaderConstantRange &);
public: void SetSphereMapMatrix(const CShaderConstantRange &);
public: void SetTextureTransform(unsigned long,const CShaderConstantRange &);
public: void SetBoneMatrices(const CShaderConstantRange &);
public: void SetShadowTextureTransform(const CShaderConstantRange &);
public: void SetShadowFadeTransform(const CShaderConstantRange &);
public: void SetShadowFadeFactors(const CShaderConstantRange &);
public: void SetShadowedSpotlightFalloffTransform(const CShaderConstantRange &);
public: void SetShadowedSpotlightAttenuation(const CShaderConstantRange &);
public: void SetShadowedSpotlightColour(const CShaderConstantRange &);
public: void SetScreenSpaceTransformScale(const CShaderConstantRange &);
public: void SetScreenSpaceTransformOffset(const CShaderConstantRange &);
public: void SetReservedPresets(const CArray<CVertexShaderConstant> &);
public: bool Validate(const CShaderConstantRange &) const;
private: bool RangeValidate(const CShaderConstantRange &,const CShaderConstantRange &,CArray<bool> &) const;
};
FABLE_STATIC_ASSERT(sizeof(CVertexShaderConstantLayout)==0x14c);
#endif
#else
#ifdef FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#ifndef FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_H
#define FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_H
// Retail opt-in (Codex pass13): Ego_r TPI0x40b9e derives from CBaseClass, native sizeof0x14c.
// All original data retained; native manager ctor0098ab00 places this object at18 and Lights at164.
#include "engine/CBaseClass.h"
#ifdef FABLE_RETAIL_SHADER_LIGHTING
#include "engine/CShaderConstantRange.h"
#endif
class CVertexShaderConstantLayout : public CBaseClass {
#ifdef FABLE_RETAIL_SHADER_LIGHTING
    friend class CShaderRenderManager;
#endif
public:
    CVertexShaderConstantLayout();
    virtual ~CVertexShaderConstantLayout();
private:
    unsigned char Reserved[0xc]; // +0x4 CShaderConstantRange
    unsigned char User[0xc]; // +0x10 CShaderConstantRange
    unsigned char Compression[0xc]; // +0x1c CShaderConstantRange
    unsigned char Ambient[0xc]; // +0x28 CShaderConstantRange
    unsigned char FogColour[0xc]; // +0x34 CShaderConstantRange
#ifdef FABLE_RETAIL_SHADER_LIGHTING
    CShaderConstantRange CameraPos;
#else
    unsigned char CameraPos[0xc];
#endif // +0x40 CShaderConstantRange
    unsigned long LightSize; // +0x4c
#ifdef FABLE_RETAIL_SHADER_LIGHTING
    CShaderConstantRange LightArray;
#else
    unsigned char LightArray[0xc];
#endif // +0x50 CShaderConstantRange
    unsigned char LightGlobals[0xc]; // +0x5c CShaderConstantRange
#ifdef FABLE_RETAIL_SHADER_LIGHTING
    CShaderConstantRange LightAttenuations;
#else
    unsigned char LightAttenuations[0xc];
#endif // +0x68 CShaderConstantRange
    unsigned char CombinedProjectionMatrix[0xc]; // +0x74 CShaderConstantRange
    unsigned char FogTransform[0xc]; // +0x80 CShaderConstantRange
    unsigned char WorldMatrix[0xc]; // +0x8c CShaderConstantRange
    unsigned char SphereMapMatrix[0xc]; // +0x98 CShaderConstantRange
    unsigned char TextureTransform[0x30]; // +0xa4 CShaderConstantRange
    unsigned char BoneMatrices[0xc]; // +0xd4 CShaderConstantRange
    unsigned char ShadowTextureTransform[0xc]; // +0xe0 CShaderConstantRange
    unsigned char ShadowFadeTransform[0xc]; // +0xec CShaderConstantRange
    unsigned char ShadowFadeFactors[0xc]; // +0xf8 CShaderConstantRange
    unsigned char ShadowedSpotlightFalloffTransform[0xc]; // +0x104 CShaderConstantRange
    unsigned char ShadowedSpotlightAttenuation[0xc]; // +0x110 CShaderConstantRange
    unsigned char ShadowedSpotlightColour[0xc]; // +0x11c CShaderConstantRange
    unsigned char ScreenSpaceTransformScale[0xc]; // +0x128 CShaderConstantRange
    unsigned char ScreenSpaceTransformOffset[0xc]; // +0x134 CShaderConstantRange
    unsigned char ReservedPresets[0xc]; // +0x140 CArray<CVertexShaderConstant>
};
FABLE_STATIC_ASSERT(sizeof(CVertexShaderConstantLayout)==0x14c);
#endif
#else
#ifndef FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_H
#define FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_H
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

#endif // FABLE_ENGINE_CVERTEXSHADERCONSTANTLAYOUT_H

#endif

#endif
