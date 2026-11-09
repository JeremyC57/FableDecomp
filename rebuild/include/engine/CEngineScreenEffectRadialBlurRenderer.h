#ifndef FABLE_ENGINE_CENGINESCREENEFFECTRADIALBLURRENDERER_H
#define FABLE_ENGINE_CENGINESCREENEFFECTRADIALBLURRENDERER_H
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
// Retail branch (chunk 731, 2026-11-03): RTTI .?AVCEngineScreenEffectRadialBlurRenderer@@ (vtable 0x012A2658) : CEngineComponent (+0x0); sizeof 0x22C from
// CEngine::Initialise 0x00B26360 (`new` of 0x22C bytes before the constructor call). Constructor ??0CEngineScreenEffectRadialBlurRenderer@@QAE@ABVCEngineInit@@@Z 0x00B5C460 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineScreenEffectRadialBlurRenderer@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CEngineComponent.h"
class CEngineInit;
#ifdef FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
// Retail opt-in (chunk 892, rows 0x00B5E0F0 ApplyRadialBlur / landed twin 0x00B5E700 ShowBlurTexture): the own members in Ego_r.pdb
// order and names (struct_layouts_egor.tsv; no STLport member, so the Ego_r offsets are retail's: 0x00B5E0F0 reads
// CurrentParameters.Fade +0x1B4, writes TextureToBackBufferVerts[i].DiffuseColour +0x11C.., applies ScreenCopyVertexShader +0x20 /
// TextureDiffusePixelShader +0x48 and draws BlurTexture256_2 +0x10; 0x00B5E700 reads ShowDebug +0x1C4 and BlurToBackBufferVerts
// +0x1CC). The nested CParameters (0x28 bytes, ten floats) is Ego_r's `CParameters`. Methods: Ego_r.pdb / FableWin.pdb spellings
// (?ApplyRadialBlur@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ, ?ShowBlurTexture@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ,
// both private).
#ifndef FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#endif
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CTVertexColTex1.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
class CTexture;
class CEngineScreenEffectRadialBlurRenderer : public CEngineComponent {
public:
    class CParameters {                                      // 0x28 bytes (Ego_r.pdb)
    public:
        float ThresholdR;                                    // +0x00
        float ThresholdG;                                    // +0x04
        float ThresholdB;                                    // +0x08
        float IntensityR;                                    // +0x0c
        float IntensityG;                                    // +0x10
        float IntensityB;                                    // +0x14
        float Length;                                        // +0x18
        float Fade;                                          // +0x1c
        float Range;                                         // +0x20
        float Twist;                                         // +0x24
    };
    CEngineScreenEffectRadialBlurRenderer(const CEngineInit&); // 0x00B5C460 ??0CEngineScreenEffectRadialBlurRenderer@@QAE@ABVCEngineInit@@@Z
    virtual unsigned long PeekSceneFilterFlags() const;
private:
    void ApplyRadialBlur();                                  // 0x00B5E0F0 ?ApplyRadialBlur@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ
    void ShowBlurTexture();                                  // 0x00B5E700 ?ShowBlurTexture@CEngineScreenEffectRadialBlurRenderer@@AAEXXZ
    CTexture*       BlurTexture256_1;                        // +0x0c
    CTexture*       BlurTexture256_2;                        // +0x10
    long            BlurTextureHandle256_1;                  // +0x14
    long            BlurTextureHandle256_2;                  // +0x18
    long            DynamicTexturePageHandle;                // +0x1c
    CVertexShader   ScreenCopyVertexShader;                  // +0x20
    CVertexShader   RadialBlurVertexShader;                  // +0x28
    CPixelShader    ScreenCopyPixelShader;                   // +0x30
    CPixelShader    RadialBlurPixelShader;                   // +0x38
    CPixelShader    TextureOnlyPixelShader;                  // +0x40
    CPixelShader    TextureDiffusePixelShader;               // +0x48
    CTVertexColTex1 BackBufferToTextureVerts[4];             // +0x50
    CTVertexColTex1 Blur256ToBlur256Verts[4];                // +0xb0
    CTVertexColTex1 TextureToBackBufferVerts[4];             // +0x110
    C2DVector       BlurCenter;                              // +0x170
    C2DVector       AreaCenter;                              // +0x178
    C3DVector       BlurCenterWorldTrackPosition;            // +0x180
    C3DVector       AreaCenterWorldTrackPosition;            // +0x18c
    CParameters     CurrentParameters;                       // +0x198
    bool            AreaTrack3DPos;                          // +0x1c0
    bool            BlurTrack3DPos;                          // +0x1c1
    long            ShowDebug;                               // +0x1c4
    bool            EffectSupported;                         // +0x1c8
    bool            UseEnvironmentParameters;                // +0x1c9
    CTVertexColTex1 BlurToBackBufferVerts[4];                // +0x1cc
};
#else
class CEngineScreenEffectRadialBlurRenderer : public CEngineComponent {
public:
    CEngineScreenEffectRadialBlurRenderer(const CEngineInit&); // 0x00B5C460 ??0CEngineScreenEffectRadialBlurRenderer@@QAE@ABVCEngineInit@@@Z
    virtual unsigned long PeekSceneFilterFlags() const;
private:
    unsigned char _pad_0xc[0x220];              // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
};
#endif
FABLE_STATIC_ASSERT(sizeof(CEngineScreenEffectRadialBlurRenderer) == 0x22c);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineScreenEffectRadialBlurRenderer = 0x22c.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CTexture;

#pragma pack(push, 1)
class CEngineScreenEffectRadialBlurRenderer {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    CTexture*     BlurTexture256_1;                          // +0x0c
    CTexture*     BlurTexture256_2;                          // +0x10
    long          BlurTextureHandle256_1;                    // +0x14
    long          BlurTextureHandle256_2;                    // +0x18
    long          DynamicTexturePageHandle;                  // +0x1c
    unsigned char ScreenCopyVertexShader[0x8];               // +0x20 CVertexShader
    unsigned char RadialBlurVertexShader[0x8];               // +0x28 CVertexShader
    unsigned char ScreenCopyPixelShader[0x8];                // +0x30 CPixelShader
    unsigned char RadialBlurPixelShader[0x8];                // +0x38 CPixelShader
    unsigned char TextureOnlyPixelShader[0x8];               // +0x40 CPixelShader
    unsigned char TextureDiffusePixelShader[0x8];            // +0x48 CPixelShader
    unsigned char BackBufferToTextureVerts[0x60];            // +0x50 CTVertexColTex1[4]
    unsigned char Blur256ToBlur256Verts[0x60];               // +0xb0 CTVertexColTex1[4]
    unsigned char TextureToBackBufferVerts[0x60];            // +0x110 CTVertexColTex1[4]
    float         BlurCenter_X;                              // +0x170 C2DVector::X
    float         BlurCenter_Y;                              // +0x174 C2DVector::Y
    float         AreaCenter_X;                              // +0x178 C2DVector::X
    float         AreaCenter_Y;                              // +0x17c C2DVector::Y
    float         BlurCenterWorldTrackPosition_X;            // +0x180 C3DVector::X
    float         BlurCenterWorldTrackPosition_Y;            // +0x184 C3DVector::Y
    float         BlurCenterWorldTrackPosition_Z;            // +0x188 C3DVector::Z
    float         AreaCenterWorldTrackPosition_X;            // +0x18c C3DVector::X
    float         AreaCenterWorldTrackPosition_Y;            // +0x190 C3DVector::Y
    float         AreaCenterWorldTrackPosition_Z;            // +0x194 C3DVector::Z
    float         CurrentParameters_ThresholdR;              // +0x198 CParameters::ThresholdR
    float         CurrentParameters_ThresholdG;              // +0x19c CParameters::ThresholdG
    float         CurrentParameters_ThresholdB;              // +0x1a0 CParameters::ThresholdB
    float         CurrentParameters_IntensityR;              // +0x1a4 CParameters::IntensityR
    float         CurrentParameters_IntensityG;              // +0x1a8 CParameters::IntensityG
    float         CurrentParameters_IntensityB;              // +0x1ac CParameters::IntensityB
    float         CurrentParameters_Length;                  // +0x1b0 CParameters::Length
    float         CurrentParameters_Fade;                    // +0x1b4 CParameters::Fade
    float         CurrentParameters_Range;                   // +0x1b8 CParameters::Range
    float         CurrentParameters_Twist;                   // +0x1bc CParameters::Twist
    bool          AreaTrack3DPos;                            // +0x1c0
    bool          BlurTrack3DPos;                            // +0x1c1
    unsigned char _pad_0x1c2[0x2];                           // +0x1c2
    long          ShowDebug;                                 // +0x1c4
    bool          EffectSupported;                           // +0x1c8
    bool          UseEnvironmentParameters;                  // +0x1c9
    unsigned char _pad_0x1ca[0x2];                           // +0x1ca
    unsigned char BlurToBackBufferVerts[0x60];               // +0x1cc CTVertexColTex1[4]
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineScreenEffectRadialBlurRenderer) == 0x22c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurTexture256_1) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurTexture256_2) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurTextureHandle256_1) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurTextureHandle256_2) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, DynamicTexturePageHandle) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, ScreenCopyVertexShader) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, RadialBlurVertexShader) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, ScreenCopyPixelShader) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, RadialBlurPixelShader) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, TextureOnlyPixelShader) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, TextureDiffusePixelShader) == 0x48);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BackBufferToTextureVerts) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, Blur256ToBlur256Verts) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, TextureToBackBufferVerts) == 0x110);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurCenter_X) == 0x170);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurCenter_Y) == 0x174);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, AreaCenter_X) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, AreaCenter_Y) == 0x17c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurCenterWorldTrackPosition_X) == 0x180);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurCenterWorldTrackPosition_Y) == 0x184);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurCenterWorldTrackPosition_Z) == 0x188);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, AreaCenterWorldTrackPosition_X) == 0x18c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, AreaCenterWorldTrackPosition_Y) == 0x190);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, AreaCenterWorldTrackPosition_Z) == 0x194);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_ThresholdR) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_ThresholdG) == 0x19c);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_ThresholdB) == 0x1a0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_IntensityR) == 0x1a4);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_IntensityG) == 0x1a8);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_IntensityB) == 0x1ac);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_Length) == 0x1b0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_Fade) == 0x1b4);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_Range) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, CurrentParameters_Twist) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, AreaTrack3DPos) == 0x1c0);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurTrack3DPos) == 0x1c1);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, ShowDebug) == 0x1c4);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, EffectSupported) == 0x1c8);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, UseEnvironmentParameters) == 0x1c9);
FABLE_STATIC_ASSERT(offsetof(CEngineScreenEffectRadialBlurRenderer, BlurToBackBufferVerts) == 0x1cc);

#endif // FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#endif // FABLE_ENGINE_CENGINESCREENEFFECTRADIALBLURRENDERER_H
