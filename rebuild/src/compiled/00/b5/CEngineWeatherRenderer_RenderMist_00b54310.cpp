// CEngineWeatherRenderer::RenderMist(const CEngineComponentRenderParams&) -- bind the mist volume texture on stages 0-3, the mist shaders, index and vertex buffers, upload the ten mist vertex-shader constants, set one render state, flush shader state and draw the mist quad
// NF151 (2026-11-20, q4541): the row-local CShaderRenderManager declaration respelled in place to ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z (0x00988140; Ego_r.pdb 0x00AE47A0 + FableWin.pdb 0x030325A0: const CPixelShader&).
// NF127 (2026-11-20): the row-local CShaderRenderManager declarations respelled in place to ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z (0x00988020; Ego_r.pdb 0x00AE4680 + FableWin.pdb 0x03031510: const CVertexShader&, bool).
// Leaf: RenderMist
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B54310, length 1234 bytes (+ int3 padding), `ret 4` (params unused).
// Signature from Ego_r.pdb: ?RenderMist@CEngineWeatherRenderer@@AAEXABVCEngineComponentRenderParams@@@Z.
// Callees: CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (inline OpenCaptureBlock on 0x013CB508),
// CShaderRenderManager::ApplyPixelShader @ 0x00988140, ApplyVertexShader @ 0x00988020, SetUserVertexShaderConstant
// @ 0x00989B00, the inlined PrepareForRender(1) (SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20,
// UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals
// 0x0098A760, UpdateCombinedProjectionTransform 0x00988A50), CRenderManagerCore::RealiseRenderState @ 0x00A058C0,
// DrawCurrentBuffersAsIndexedTris @ 0x00A0AC60; D3D device slots SetTexture (+0x104), SetStreamSource (+0x190),
// SetIndices (+0x1a0).
// Globals: render-state manager 0x013CB508, shader manager 0x01436E14, render core 0x01436E18, mist parameter object
// 0x01436E9C (+0xd0 position, +0xdc scale), engine camera 0x01436EA0 (CMatrix3x4 at +0xe4), state value 0x01396FB8.
// Layout: engine/CEngineWeatherRenderer.h for `this` (MistVertexBuffer +0x88, MistIndexBuffer +0xbc, MistVertexShader
// +0x100, MistPixelShader +0x108, MistVolumeTexture +0x110, Settings +0x114). Settings field names past the landed
// RenderRainBox view are descriptive (PDB CEngineWeatherSettings offsets +0x74..+0xb4, +0x4c8, +0x4e4).
// Shape: the core inlines SetTexture (attached-texture cache +0x3d00, active stage count +0x3cb8, the landed
// AttachTextureToStage 0x009A0CF0 body), SetIndices (if/else arms so the device loads before the null test) and
// SetStreamSource (the landed CWaterPatchMesh::Render model). The mist-parameter global is read into a named local:
// with the global spelled at each use VC loads it first and swaps the fld/fmul of `height * scale`.
// NF45 (round 7): the render core 0x01436E18 is NEngineInternal::GPRenderManager (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore at offset 0): the local
//   CRenderManagerCore / IDirect3DDevice9 / CBaseTextureView / CIndexBufferView / CVertexBufferView model is the shared class and its
//   Ego_r.pdb inlines AttachVolumeTextureToStage(stage, MistVolumeTexture) (repo VOLUMETEXTURE_INLINE: PD3DVolumeTexture +4, stage
//   cache +0x3D00, TextureStagesInUse +0x3CB8), SelectIndexBuffer(&MistIndexBuffer, 0) and SelectVertexBuffer(0, &MistVertexBuffer)
//   (repo FORCEINLINE_HELPERS), SetPolyCullMode(POLY_CULL_NONE) (+0x2890, D3DPolyCullModeTable[2] = 0x01396FB8), RealiseRenderState
//   0x00A058C0 and DrawCurrentBuffersAsIndexedTris 0x00A0AC60. The capture open / RestoreCaptureBlock pair on 0x013CB508 (both
//   exits) is a CRenderStateScopedCaptureBlock (repo SCOPED_FORCEINLINE; was a local CRenderStateManager on g_pRenderStateManager).
//   0x01436E9C is NEngineInternal::GPLightingManager (?GPLightingManager@NEngineInternal@@3PAVCEngineLightingManager@@A): the
//   "mist position / scale" at +0xD0..+0xDC are CEngineLightingManager::MistEffectColour R / G / B / A (engine/CEngineLightingManager.h;
//   was CMistParamsView on g_pMistParams). 0x01436EA0 is NEngineInternal::GPCamera: +0xE4 is CEngineCamera::InverseViewTransform
//   (engine/CEngineCamera.h; was CEngineCameraView::ViewTransform on g_pEngineCamera), read through the row's CMatrix3x4 view.
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_VOLUMETEXTURE_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "fable_compiler.h"
#include "engine/CVolumeTexture.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CEngineWeatherRenderer.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLightingManager.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }            // 0x01436E18
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }  // 0x01436E9C
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                    // 0x01436EA0

#include "engine/C3DVector.h"
class C4DVector { public: float X, Y, Z, W; };
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};

class CShaderConstantRange { public:
    void* _vptr;
    long Offset;
    unsigned long Count;
};

class CVertexShaderConstantLayout { public:
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10
    CArray<C4DVector> ReservedPresets;                  // +0x140
};

class CVertexShader;
class CPixelShader;

class CShaderRenderManager {
public:
    void ApplyVertexShader(const CVertexShader& shader, bool force);   // 0x00988020
    void ApplyPixelShader(const CPixelShader& shader);                 // 0x00988140
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00

    FABLE_FORCEINLINE void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1e0];
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1ef];
    unsigned long UpdateFlags;                          // +0x3d8

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
    void ResetWorldTransform();                         // 0x00988290
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14
class CEngineComponentRenderParams;

struct CEngineWeatherSettingsView {                // CEngineWeatherSettings at this+0x114
    unsigned char _pad00[0x74];
    float MistHeight;                              // +0x74  (this+0x188)
    C3DVector MistColour;                          // +0x78  (this+0x18c)
    C3DVector MistScale;                           // +0x84  (this+0x198)
    C3DVector MistParam1;                          // +0x90  (this+0x1a4)
    C3DVector MistParam2;                          // +0x9c  (this+0x1b0)
    C3DVector MistParam3;                          // +0xa8  (this+0x1bc)
    unsigned char _padb4[0x4c8 - 0xb4];
    float FogScale;                                // +0x4c8 (this+0x5dc)
    unsigned char _pad4cc[0x4e4 - 0x4cc];
    C3DVector FogColour;                           // +0x4e4 (this+0x5f8)
};

struct CEngineWeatherRendererMethods : CEngineWeatherRenderer {
    CEngineWeatherSettingsView& GetSettings() { return *reinterpret_cast<CEngineWeatherSettingsView*>(Settings); }
    void RenderMist(const CEngineComponentRenderParams& params);
};


void CEngineWeatherRendererMethods::RenderMist(const CEngineComponentRenderParams& params)
{
    CRenderStateScopedCaptureBlock capture_block;
    if (!MistVolumeTexture)
        return;
    NEngineInternal::GPRenderManager->AttachVolumeTextureToStage(0, MistVolumeTexture);
    NEngineInternal::GPRenderManager->AttachVolumeTextureToStage(1, MistVolumeTexture);
    NEngineInternal::GPRenderManager->AttachVolumeTextureToStage(2, MistVolumeTexture);
    NEngineInternal::GPRenderManager->AttachVolumeTextureToStage(3, MistVolumeTexture);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*reinterpret_cast<const CPixelShader*>(MistPixelShader));
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*reinterpret_cast<const CVertexShader*>(MistVertexShader), false);
    NEngineInternal::GPRenderManager->SelectIndexBuffer(reinterpret_cast<CIndexBuffer*>(MistIndexBuffer), 0);
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, reinterpret_cast<CVertexBufferWin32*>(MistVertexBuffer));
    const CEngineLightingManager* lighting = NEngineInternal::GPLightingManager;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, lighting->MistEffectColour_R, lighting->MistEffectColour_G,
                                                        lighting->MistEffectColour_B,
                                                        GetSettings().MistHeight * lighting->MistEffectColour_A);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(1, GetSettings().MistScale.X, GetSettings().MistScale.Y,
                                                        GetSettings().MistScale.Z, 1.0f);
    const CMatrix3x4& view = reinterpret_cast<const CMatrix3x4&>(NEngineInternal::GPCamera->InverseViewTransform);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, view.XAxis.X, view.YAxis.X, view.ZAxis.X, view.Position.X);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, view.XAxis.Y, view.YAxis.Y, view.ZAxis.Y, view.Position.Y);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(4, view.XAxis.Z, view.YAxis.Z, view.ZAxis.Z, view.Position.Z);
    float fogScale = GetSettings().FogScale;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(5, fogScale * GetSettings().FogColour.X,
                                                        fogScale * GetSettings().FogColour.Y,
                                                        fogScale * GetSettings().FogColour.Z, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(6, GetSettings().MistColour.X, GetSettings().MistColour.Y,
                                                        GetSettings().MistColour.Z, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(7, GetSettings().MistParam1.X, GetSettings().MistParam1.Y,
                                                        GetSettings().MistParam1.Z, 1.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(8, GetSettings().MistParam2.X, GetSettings().MistParam2.Y,
                                                        GetSettings().MistParam2.Z, 1.0f);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(9, GetSettings().MistParam3.X, GetSettings().MistParam3.Y,
                                                        GetSettings().MistParam3.Z, 1.0f);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
    NEngineInternal::GPRenderManager->RealiseRenderState();
    NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, 4, 0, 2);
}