// CEngineWaterRenderer::RenderIceBackground(float time) -- bind the ice bump map and environment map, set the ice blend states and shaders, upload the bump-scale constant and the four ice bone matrices, flush the shader state and draw the alphaed then the opaque ice background patch lists
// NF127 (2026-11-20): the row-local CShaderRenderManager declarations respelled in place to ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z (0x00988020; Ego_r.pdb 0x00AE4680 + FableWin.pdb 0x03031510: const CVertexShader&, bool) and ?SetBoneTransform@CShaderRenderManager@@QAEXKABVCMatrix3x4@@@Z (0x0098A7F0; Ego_r.pdb 0x00AE5F80 + FableWin.pdb 0x03032D30: matrix by const reference).
// Leaf: RenderIceBackground
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B75CA0, length 1320 bytes (+ int3 padding), `ret 4`.
// Signature from Ego_r.pdb: ?RenderIceBackground@CEngineWaterRenderer@@AAEXM@Z.
// Callees: CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (inline OpenCaptureBlock on 0x013CB508);
// CShaderRenderManager::ApplyVertexShader @ 0x00988020, ApplyPixelShader @ 0x00988140, SetUserVertexShaderConstant
// @ 0x00989B00, SetBoneTransform @ 0x0098A7F0, BoneMatrices.resize @ 0x00B7EDF0, the inlined PrepareForRender(0x604);
// CEngineWaterRenderer::RenderBackgroundList @ 0x00B6E260 (landed); D3D SetTexture (+0x104) through the core's inline
// texture-stage cache (+0x3d00, stage count +0x3cb8); six inlined render-core SetState calls.
// Layout: retail CEngineWaterRenderer (12-byte STLport CArrays where the PDB header has 16): IceBackgroundVertexShader
// +0x150, IceBackgroundPixelShader +0x170, EnvironmentMap +0x1e0, IceBumpMap +0x1f8, IceBackgroundRenderList +0x22c,
// IceAlphaedBackgroundRenderList +0x238 (PDB +0x15c/+0x17c/+0x1ec/+0x204/+0x248/+0x258); Settings at retail +0x280
// (the landed Setup*ShaderConstants rows): +0x28d bump enable, +0x2dc ice bump scale; bone matrices +0x414..+0x504.
// CTexture (engine/CTexture.h) holds the device texture at +0.
// Shape: the bump constant is `scale * 0.5f` (0x0122F59C is the 0.5f literal; an extern float swaps the fld/fmul).

// NF45 (round 7): the render core 0x01436E18 is NEngineInternal::GPRenderManager (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore at offset 0): the local
//   CRenderManagerCore / IDirect3DDevice9 / CTrackedRenderState / SetState model and the g_renderStateValue_* table reads are the shared
//   class and its Ego_r.pdb inlines: AttachTextureToStage (repo FORCEINLINE_HELPERS: PD3DTexture +0, stage cache +0x3D00,
//   TextureStagesInUse +0x3CB8, SetTexture slot 65), SelectIndexBuffer / SelectVertexBuffer, and the CRenderStateManager setters with
//   the D3D*Table enumerators (D3DTextureUVModeTable 0x01396DC8: WRAP / MIRROR / CLAMP = +0 / +4 / +8). The capture open /
//   RestoreCaptureBlock pair on 0x013CB508 is a CRenderStateScopedCaptureBlock (repo SCOPED_FORCEINLINE; was a local
//   CRenderStateManager on g_pRenderStateManager). The local CTexture view is engine/CTexture.h. The shader manager, CArray,
//   CMatrix3x4, C4DVector and CEngineWaterRenderer models are unchanged (row debt).
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "fable_compiler.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
#include "engine/C3DVector.h"
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
class CMatrix3x4 { public: C3DVector XAxis, YAxis, ZAxis, Position; };

struct CPreTransposedBoneMatrix {
    float M[12];
    CPreTransposedBoneMatrix() { }
};

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
    void resize(unsigned int n, const T& value);        // 0x00B7EDF0 for CPreTransposedBoneMatrix
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
    void ApplyPixelShader(const CPixelShader* shader);                 // 0x00988140
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00
    void SetBoneTransform(unsigned long index, const CMatrix3x4& matrix);                        // 0x0098A7F0

    void SetPixelShaderConstant(unsigned long index, const C4DVector& value)
    {
        *(PSConstTable.First + index) = value;
        UpdateFlags |= 2;
    }

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
    unsigned char _pad1e9[0x1e3];
    CArray<C4DVector> PSConstTable;                     // +0x3cc
    unsigned long UpdateFlags;                          // +0x3d8
    CArray<CPreTransposedBoneMatrix> BoneMatrices;      // +0x3dc

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
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
public:
    void ResetWorldTransform();   // 0x00988290
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14


class CEngineWaterBackgroundSubPatch;

class CEngineWaterRenderer { public:                      // retail layout (12-byte CArrays)
    unsigned char _pad000[0x150];
    unsigned char IceBackgroundVertexShader[8];    // +0x150 (PDB +0x15c)
    unsigned char _pad158[0x170 - 0x158];
    unsigned char IceBackgroundPixelShader[8];     // +0x170 (PDB +0x17c)
    unsigned char _pad178[0x1e0 - 0x178];
    CTexture* EnvironmentMap;                      // +0x1E0 (PDB +0x1ec)
    unsigned char _pad1e4[0x1f8 - 0x1e4];
    CTexture* IceBumpMap;                          // +0x1F8 (PDB +0x204)
    unsigned char _pad1fc[0x22c - 0x1fc];
    CArray<CEngineWaterBackgroundSubPatch*> IceBackgroundRenderList;          // +0x22C (PDB +0x248)
    CArray<CEngineWaterBackgroundSubPatch*> IceAlphaedBackgroundRenderList;   // +0x238 (PDB +0x258)
    unsigned char _pad244[0x28d - 0x244];
    bool EnableBumpMap;                            // +0x28D (Settings +0x0d)
    unsigned char _pad28e[0x2dc - 0x28e];
    float IceBumpScale;                            // +0x2DC (Settings +0x5c)
    unsigned char _pad2e0[0x414 - 0x2e0];
    CMatrix3x4 BoneMatrix1;                        // +0x414
    CMatrix3x4 BoneMatrix3;                        // +0x444
    CMatrix3x4 BoneMatrix2;                        // +0x474
    unsigned char _pad4a4[0x504 - 0x4a4];
    CMatrix3x4 BoneMatrix0;                        // +0x504
    void RenderBackgroundList(CArray<CEngineWaterBackgroundSubPatch*>& list, float time);   // 0x00B6E260
    void RenderIceBackground(float time);
};

void CEngineWaterRenderer::RenderIceBackground(float time)
{
    CRenderStateScopedCaptureBlock capture_block;
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, IceBumpMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(1, EnvironmentMap);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_MIRROR);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*reinterpret_cast<const CVertexShader*>(IceBackgroundVertexShader), false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(reinterpret_cast<const CPixelShader*>(IceBackgroundPixelShader));
    float bump = 0.0f;
    if (EnableBumpMap)
        bump = IceBumpScale * 0.5f;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, bump, bump, bump, bump);
    NEngineInternal::GPShaderRenderManager->BoneMatrices.resize(4, CPreTransposedBoneMatrix());
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(0, BoneMatrix0);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(1, BoneMatrix1);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(2, BoneMatrix2);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(3, BoneMatrix3);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x604);
    RenderBackgroundList(IceAlphaedBackgroundRenderList, time);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    RenderBackgroundList(IceBackgroundRenderList, time);
}