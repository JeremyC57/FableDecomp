#ifndef FABLE_ENGINE_CSHADERRENDERMANAGER_H
#define FABLE_ENGINE_CSHADERRENDERMANAGER_H
// CShaderRenderManager (chunk 723b, the water render passes): the retail class. Ego_r.pdb class (V-keyed); RTTI CShaderRenderManager : CBaseClassNonCopyable : CBaseClass, polymorphic
// (??1CShaderRenderManager@@UAE@XZ, ??_E); the engine's instance pointer is 0x01436E14 (bound g_pShaderRenderManager).
// Layout: retail = Ego_r.pdb names in Ego_r order with the 12-byte STLport CArray / 12-byte map in place of Ego_r's 16-byte
// Dinkumware ones (VertexShaderConstantLayouts, NullConstantLayout.ReservedPresets, LightGlobals, LightAttenuations: -0x10 by
// WorldMatrixIsIdentity; ResourceManagers and PSConstNames (CArray + map): -0x1C by PSConstTable); the 16-aligned CMatrix4x4
// block absorbs 4 of the first 0x10. Retail-proven offsets: D3DDevice +0x1D4 and CurrentPixelShader.Data +0x1E4 (UpdatePixelShader
// 0x00988A20), WorldMatrixIsIdentity +0x1E8, PSConstTable +0x3CC, UpdateFlags +0x3D8, BoneMatrices +0x3DC (PrepareForRender /
// SetPixelShaderConstant / SetBoneMatrixCount as expanded by 0x00B70230, 0x00B77E20, 0x00B76BC0). sizeof 0x3E8 (Ego_r 0x410).
// Functions: Ego_r.pdb / FableWin.pdb spellings. Out of line: ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140,
// SetWorldTransform 0x009881F0, ResetWorldTransform 0x00988290, SetUserVertexShaderConstant(KMMMM) 0x00989B00, SetBoneTransform
// 0x0098A7F0, SetVSConstants 0x0098B930 and the Update* helpers. Inline (no retail copy; expanded by every render pass):
// PrepareForRender(flags), SetPixelShaderConstant(index, value), SetBoneMatrixCount(count) (the STLport vector::resize it calls
// is out of line, 0x00B7EDF0).
#ifndef FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#endif
#ifndef FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4
#endif
#ifndef FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#endif
#ifndef FABLE_RETAIL_CSIMPLEMEMORYPOOL
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CRGBColour.h"
#include "engine/CMatrix3x4.h"
#include "engine/CPreTransposedBoneMatrix.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CVertexShaderConstantLayout.h"
#include "engine/CPixelShader.h"
struct IDirect3DDevice9;
struct IDirect3DVertexShader9;
struct IDirect3DVertexDeclaration9;
class CRenderManager;
class CShaderResource;
class CTexture;
class CVertexShader;
class CVertexBufferWin32;

class CShaderRenderManager : public CBaseClassNonCopyable {
public:
    class CLightInfo;                                                    // ?CLightInfo@CShaderRenderManager (0x38 bytes)
    CShaderRenderManager();                                              // ??0CShaderRenderManager@@QAE@XZ
    virtual ~CShaderRenderManager();                                     // ??1CShaderRenderManager@@UAE@XZ
    static CShaderRenderManager& __fastcall Get();                       // ?Get@CShaderRenderManager@@SIAAV1@XZ
    void ApplyVertexShader(const CVertexShader& shader, bool force);     // 0x00988020 ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z
    void ApplyPixelShader(const CPixelShader& shader);                   // 0x00988140 ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z
    void SetWorldTransform(const CMatrix3x4& matrix);                    // 0x009881F0 ?SetWorldTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z
    void ResetWorldTransform();                                          // 0x00988290 ?ResetWorldTransform@CShaderRenderManager@@QAEXXZ
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00 ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKMMMM@Z
    void SetUserVertexShaderConstant(unsigned long index, const C4DVector& value);              // 0x00989AB0 ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKABVC4DVector@@@Z
    void SetUserVertexShaderConstant(unsigned long index, const C3DVector& value, float w);     // 0x00989A60 ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKABVC3DVector@@M@Z
    void SetUserVertexShaderConstant(unsigned long index, const CRGBFloatColour& value);        // 0x00989B50 ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKABVCRGBFloatColour@@@Z
    void SetUserVertexShaderConstants(unsigned long index, long count, float* values);          // 0x009896D0 ?SetUserVertexShaderConstants@CShaderRenderManager@@QAEXKJPAM@Z
    void SetBoneTransform(unsigned long index, const CMatrix3x4& matrix);                       // 0x0098A7F0 ?SetBoneTransform@CShaderRenderManager@@QAEXKABVCMatrix3x4@@@Z
    void SetTextureTransform(unsigned long stage, const CMatrix3x4& matrix);                    // ?SetTextureTransform@CShaderRenderManager@@QAEXKABVCMatrix3x4@@@Z
    void SetTextureTransformCount(long count);                           // ?SetTextureTransformCount@CShaderRenderManager@@QAEXJ@Z
    void SetFogTransform(const C4DVector& transform);                    // ?SetFogTransform@CShaderRenderManager@@QAEXABVC4DVector@@@Z
    void SelectVertexBuffer(CVertexBufferWin32& buffer);                 // ?SelectVertexBuffer@CShaderRenderManager@@QAEXAAVCVertexBufferWin32@@@Z
    IDirect3DDevice9* GetD3DDevice();                                    // ?GetD3DDevice@CShaderRenderManager@@QAEPAUIDirect3DDevice9@@XZ
    bool GetVertexShadersEnabled() const;                                // ?GetVertexShadersEnabled@CShaderRenderManager@@QBE_NXZ
    bool GetPixelShadersEnabled() const;                                 // ?GetPixelShadersEnabled@CShaderRenderManager@@QBE_NXZ

    // ?SetPixelShaderConstant@CShaderRenderManager@@QAEXKABVC4DVector@@@Z (inline): copy the value into the constant table and mark
    // the pixel constants dirty (update flag 2).
    __forceinline void SetPixelShaderConstant(unsigned long index, const C4DVector& value)
    {
        *(PSConstTable.begin() + index) = value;
        UpdateFlags |= 2;
    }
    // ?SetBoneMatrixCount@CShaderRenderManager@@QAEXJ@Z (inline): size the bone-matrix table (STLport vector::resize, out of line).
    __forceinline void SetBoneMatrixCount(long count)
    {
        BoneMatrices.resize(count, CPreTransposedBoneMatrix());
    }
    // ?PrepareForRender@CShaderRenderManager@@QAEXK@Z (inline; the landed 0x00B77E20 / 0x00B76BC0 / 0x00B7C510 expansions): flush
    // every dirty piece of shader state the vertex format `flags` uses.
    __forceinline void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetAsCArray(),
                           CurrentVSConstantLayout->ReservedPresets.size());
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

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930 ?SetVSConstants@CShaderRenderManager@@AAEXJPBXK@Z
    void UpdatePixelShader();                                            // 0x00988A20 ?UpdatePixelShader@CShaderRenderManager@@AAEXXZ
    void UpdatePixelShaderConstants();                                   // 0x00989BF0
    void UpdateBoneTransform();                                          // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();                             // 0x009894B0
    void UpdateObjectSpaceTextureTransform();                            // 0x00989230
    void UpdateWorldSpaceFogTransform();                                 // 0x00989640
    void UpdateObjectSpaceFogTransform();                                // 0x00989540
    void UpdateAmbient();                                                // 0x00989760
    void UpdateFogColour();                                              // 0x009897C0
    void UpdateLightGlobals();                                           // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);                 // 0x00989CB0
    void UpdateWorldSpaceLighting();                                     // 0x0098A540
    void UpdateLocalisedSphereMapping();                                 // 0x00988E80
    void UpdateCombinedProjectionTransform();                            // 0x00988A50
    void UpdateCameraPos();                                              // 0x00989700

public:
    CVertexShaderConstantLayout*   CurrentVSConstantLayout;              // +0x004
    long                           CurrentVSConstantLayoutIndex;         // +0x008
    CArray<const CVertexShaderConstantLayout*> VertexShaderConstantLayouts;   // +0x00c
    CVertexShaderConstantLayout    NullConstantLayout;                   // +0x018
    CLightInfo*                    Lights;                               // +0x164
    C4DVector*                     LightBuffer;                          // +0x168
    unsigned long                  LightCount;                           // +0x16c
    unsigned long                  AllocatedLights;                      // +0x170
    unsigned long                  AllocatedLightSize;                   // +0x174
    CSimpleMemoryPool              LightPool;                            // +0x178
    CArray<C4DVector>              LightGlobals;                         // +0x194
    CArray<C4DVector>              LightAttenuations;                    // +0x1a0
    CRGBFloatColour                AmbientColour;                        // +0x1ac
    CRGBFloatColour                FogColour;                            // +0x1bc
    CRGBColour                     TextureFactor;                        // +0x1cc
    CRenderManager*                RenderManager;                        // +0x1d0
    IDirect3DDevice9*              D3DDevice;                            // +0x1d4
    IDirect3DVertexShader9*        CurrentVertexShader;                  // +0x1d8
    IDirect3DVertexDeclaration9*   CurrentVertexShaderVertexDeclairation;   // +0x1dc
    CPixelShader                   CurrentPixelShader;                   // +0x1e0
    bool                           WorldMatrixIsIdentity;                // +0x1e8
    unsigned char                  _pad_0x1e9[0x7];                      // +0x1e9 (CMatrix4x4 alignment)
    unsigned char                  WorldMatrix[0x40];                    // +0x1f0 CMatrix4x4
    unsigned char                  ViewMatrix[0x40];                     // +0x230 CMatrix4x4
    unsigned char                  ProjectionMatrix[0x40];               // +0x270 CMatrix4x4
    unsigned char                  ViewProjectionMatrix[0x40];           // +0x2b0 CMatrix4x4
    unsigned char                  CombinedProjectionMatrix[0x40];       // +0x2f0 CMatrix4x4
    unsigned char                  TextureTransforms[0x40];              // +0x330 CMatrix4x4[1]
    C4DVector                      FogTransform;                         // +0x370
    C3DVector                      CameraPos;                            // +0x380
    long                           TextureTransformCount;                // +0x38c
    bool                           VertexShadersEnabled;                 // +0x390
    bool                           PixelShadersEnabled;                  // +0x391
    bool                           SWLightingEnable;                     // +0x392
    bool                           SoftwareVertexShaderEnable;           // +0x393
    bool                           SoftwareFixedFunctionEnable;          // +0x394
    bool                           FixedFunctionVertexMode;              // +0x395
    float                          MultisampleViewPortScale;             // +0x398
    CArray<CTexture>*              CycleTextures;                        // +0x39c
    unsigned long                  NextCycleTexture;                     // +0x3a0
    unsigned long                  CycleTextureStage;                    // +0x3a4
    CArray<CCountedPointer<CShaderResource> > ResourceManagers;          // +0x3a8
    unsigned char                  PSConstNames[0x18];                   // +0x3b4 CIndexedArray<CCharString> (CArray + STLport map)
    CArray<C4DVector>              PSConstTable;                         // +0x3cc
    unsigned long                  UpdateFlags;                          // +0x3d8
    CArray<CPreTransposedBoneMatrix> BoneMatrices;                       // +0x3dc
};
FABLE_STATIC_ASSERT(sizeof(CShaderRenderManager) == 0x3e8);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, NullConstantLayout) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, LightPool) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, D3DDevice) == 0x1d4);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, CurrentPixelShader) == 0x1e0);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, WorldMatrixIsIdentity) == 0x1e8);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, FogTransform) == 0x370);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, ResourceManagers) == 0x3a8);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, PSConstTable) == 0x3cc);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, UpdateFlags) == 0x3d8);
FABLE_STATIC_ASSERT(offsetof(CShaderRenderManager, BoneMatrices) == 0x3dc);

#endif // FABLE_ENGINE_CSHADERRENDERMANAGER_H
