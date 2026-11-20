// CEngineWaterRenderer::RenderSeaNonReflectiveForeground(float time) -- bind the sea bump and environment maps, the sea blend states, shaders and foreground index buffer, upload the swell, bone, fresnel, bump and sea-colour constants, flush the shader state and render every non-reflective sea foreground patch mesh
// NF151 (2026-11-20, q4541): the row-local CShaderRenderManager declaration respelled in place to ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z (0x00988140; Ego_r.pdb 0x00AE47A0 + FableWin.pdb 0x030325A0: const CPixelShader&).
// NF127 (2026-11-20): the row-local CShaderRenderManager declarations respelled in place to ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z (0x00988020; Ego_r.pdb 0x00AE4680 + FableWin.pdb 0x03031510: const CVertexShader&, bool) and ?SetBoneTransform@CShaderRenderManager@@QAEXKABVCMatrix3x4@@@Z (0x0098A7F0; Ego_r.pdb 0x00AE5F80 + FableWin.pdb 0x03032D30: matrix by const reference).
// Leaf: RenderSeaNonReflectiveForeground
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B77E20, length 1488 bytes (+ int3 padding), `ret 4` (time unused).
// Callees: CRenderStateManager::RestoreCaptureBlock @ 0x00A05840 (inline OpenCaptureBlock on 0x013CB508);
// CShaderRenderManager::ApplyVertexShader @ 0x00988020, ApplyPixelShader @ 0x00988140, SetUserVertexShaderConstant
// @ 0x00989B00, SetBoneTransform @ 0x0098A7F0, BoneMatrices.resize @ 0x00B7EDF0, inline SetPixelShaderConstant and
// PrepareForRender(0x604); CWaterPatchMesh::Render(long) @ 0x00BEACA0 (landed); D3D SetTexture/SetIndices through the
// core's inline helpers; the environment brightness through 0x01436E8C ->+0x198 ->vslot1 ->vslot1 ->+0x54 ->+0x138.
// Layout: retail CEngineWaterRenderer (12-byte STLport CArrays where the PDB header has 16; Settings at retail +0x280 as
// in the landed Setup*ShaderConstants rows: EnableSwell +0x28e, EnableFresnel +0x293, WaterBumpFactor +0x5c0); the
// remaining Settings field names are descriptive. Float literals: 0x0123078C 0.25f, 0x0122DEDC 0.0f, 0x0122DED8 1.0f,
// 0x01238798 -0.5f.
// Shape: the swell constant is two whole calls (if/else, cross-jumped); the fresnel factor is a ternary over an inline
// Clamp (`v < lo` / `v > hi` returns); the sea-colour constant is a C4DVector local filled member by member.

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
    void ApplyPixelShader(const CPixelShader& shader);                 // 0x00988140
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


class CWaterPatchMesh { public:
    void Render(long triangleCount) const;         // 0x00BEACA0
};

struct CEnvironmentStateView {
    unsigned char _pad0[0x138];
    float Brightness;                              // +0x138
};
struct CEnvironmentView {
    unsigned char _pad0[0x54];
    CEnvironmentStateView* State;                  // +0x54
};
struct CEnvironmentSource {
    virtual void Slot0();
    virtual CEnvironmentView* GetEnvironment();    // slot 1
};
struct CEnvironmentOwner {
    virtual void Slot0();
    virtual CEnvironmentSource* GetSource();       // slot 1
};
struct CEngineEnvironmentManagerView {             // @ 0x01436E8C
    unsigned char _pad0[0x198];
    CEnvironmentOwner* Owner;                      // +0x198
};
extern CEngineEnvironmentManagerView* g_pEngineEnvironment;   // 0x01436E8C

static inline float Clamp(float v, float lo, float hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

class CEngineWaterRenderer { public:                      // retail layout (12-byte CArrays)
    unsigned char _pad000[0xc];
    unsigned char ForegroundIndexBuffers[0x14];    // +0x0C
    unsigned char _pad020[0xb8 - 0x20];
    long WaterForegroundPolyCount;                 // +0xB8
    unsigned char _padbc[0xdc - 0xbc];
    unsigned char SeaForegroundNonReflectiveVertexShader[8];   // +0xDC
    unsigned char SeaForegroundNonReflectivePixelShader[8];    // +0xE4
    unsigned char _padec[0x1e0 - 0xec];
    CTexture* EnvironmentMap;                      // +0x1E0 (PDB +0x1ec)
    unsigned char _pad1e4[0x1e8 - 0x1e4];
    CTexture* SeaBumpMap;                          // +0x1E8 (PDB +0x1f4)
    unsigned char _pad1ec[0x250 - 0x1ec];
    CArray<CWaterPatchMesh*> SeaNonReflectiveForegroundRenderList;   // +0x250
    unsigned char _pad25c[0x28e - 0x25c];
    bool EnableSwell;                              // +0x28E (Settings +0x0e)
    unsigned char _pad28f[0x293 - 0x28f];
    bool EnableFresnel;                            // +0x293 (Settings +0x13)
    unsigned char _pad294[0x298 - 0x294];
    float FresnelFactor;                           // +0x298
    float SwellX;                                  // +0x29C
    float SwellY;                                  // +0x2A0
    unsigned char _pad2a4[0x2a8 - 0x2a4];
    C3DVector SeaColour;                           // +0x2A8
    unsigned char _pad2b4[0x2dc - 0x2b4];
    float BumpScale;                               // +0x2DC
    unsigned char _pad2e0[0x304 - 0x2e0];
    float SeaColourBrightness;                     // +0x304
    float SwellZ;                                  // +0x308
    unsigned char _pad30c[0x414 - 0x30c];
    CMatrix3x4 BoneMatrix1;                        // +0x414
    CMatrix3x4 BoneMatrix3;                        // +0x444
    CMatrix3x4 BoneMatrix2;                        // +0x474
    unsigned char _pad4a4[0x4d4 - 0x4a4];
    CMatrix3x4 BoneMatrix0;                        // +0x4D4
    unsigned char _pad504[0x5c0 - 0x504];
    float WaterBumpFactor;                         // +0x5C0
    void RenderSeaNonReflectiveForeground(float time);
};

void CEngineWaterRenderer::RenderSeaNonReflectiveForeground(float time)
{
    CRenderStateScopedCaptureBlock capture_block;
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, SeaBumpMap);
    NEngineInternal::GPRenderManager->AttachTextureToStage(1, EnvironmentMap);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*reinterpret_cast<const CVertexShader*>(SeaForegroundNonReflectiveVertexShader), false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*reinterpret_cast<const CPixelShader*>(SeaForegroundNonReflectivePixelShader));
    NEngineInternal::GPRenderManager->SelectIndexBuffer(reinterpret_cast<CIndexBuffer*>(ForegroundIndexBuffers), 0);
    if (EnableSwell)
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, SwellX, SwellY, SwellZ, 0.0f);
    else
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, 0.0f, 0.0f, 0.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->BoneMatrices.resize(4, CPreTransposedBoneMatrix());
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(0, BoneMatrix0);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(1, BoneMatrix1);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(2, BoneMatrix2);
    NEngineInternal::GPShaderRenderManager->SetBoneTransform(3, BoneMatrix3);
    float fresnel = EnableFresnel ? Clamp(FresnelFactor * 0.25f, 0.0f, 1.0f) : 0.0f;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(fresnel, fresnel, fresnel, fresnel));
    float bump = WaterBumpFactor * BumpScale;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(3, bump, 0.0f, bump * -0.5f, 1.0f);
    C4DVector colour;
    colour.X = SeaColour.X;
    colour.Y = SeaColour.Y;
    colour.Z = SeaColour.Z;
    colour.W = g_pEngineEnvironment->Owner->GetSource()->GetEnvironment()->State->Brightness * SeaColourBrightness;
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(7, colour);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0x604);
    for (CWaterPatchMesh** it = SeaNonReflectiveForegroundRenderList.First; it != SeaNonReflectiveForegroundRenderList.Last; ++it)
        (*it)->Render(WaterForegroundPolyCount);
}