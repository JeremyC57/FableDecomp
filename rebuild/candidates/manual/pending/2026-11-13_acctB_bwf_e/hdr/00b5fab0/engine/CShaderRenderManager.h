#ifndef FABLE_ENGINE_CSHADERRENDERMANAGER_H
#define FABLE_ENGINE_CSHADERRENDERMANAGER_H
#include "fable_compiler.h"
// CShaderRenderManager: one retail model (new 2026-11-03; merged from chunk 723b/723c (the water / sprite render passes) and
// Codex pass 13 (rows 0x00989CB0 UpdateObjectSpaceLighting, 0x0098ACF0 Initialise, 0x009DA9F0 CRenderManager2D::Render2DDrawList);
// Codex's per-feature FABLE_RETAIL_SHADER_LIGHTING / FABLE_RETAIL_RENDER2D_TYPES variants are folded in, the class has no macro).
// Ego_r.pdb class (V-keyed); RTTI CShaderRenderManager : CBaseClassNonCopyable : CBaseClass, polymorphic (vtable 0x0129A080,
// ??1CShaderRenderManager@@UAE@XZ, ??_E); constructor ??0CShaderRenderManager@@QAE@XZ 0x0098AB00. The singleton is the private
// static ?SingletonInstance@CShaderRenderManager@@0V1@A at 0x013BC470 (Codex's PDB DATA citation; ?Get@CShaderRenderManager@@SIAAV1@XZ
// returns it, inline in retail); the engine also keeps a pointer at 0x01436E14 (bound ?GPShaderRenderManager@NEngineInternal@@...).
// Layout: retail = Ego_r.pdb names in Ego_r order with the 12-byte STLport CArray / map in place of Ego_r's 16-byte Dinkumware
// ones (VertexShaderConstantLayouts, NullConstantLayout.ReservedPresets, LightGlobals, LightAttenuations: -0x10 by
// WorldMatrixIsIdentity; ResourceManagers and PSConstNames (CArray + map): -0x1C by PSConstTable); the 16-aligned CMatrix4x4
// block absorbs 4 of the first 0x10. Retail-proven offsets: D3DDevice +0x1D4 and CurrentPixelShader.Data +0x1E4 (UpdatePixelShader
// 0x00988A20), WorldMatrixIsIdentity +0x1E8, PSConstTable +0x3CC, UpdateFlags +0x3D8, BoneMatrices +0x3DC (PrepareForRender /
// SetPixelShaderConstant / SetBoneMatrixCount as expanded by 0x00B70230, 0x00B77E20, 0x00B76BC0); Lights +0x164, LightBuffer +0x168,
// LightCount +0x16C (UpdateObjectSpaceLighting 0x00989CB0); sizeof 0x3E8 (Ego_r 0x410; the 0x0098AB00 factory allocates 0x3E8).
// Functions: Ego_r.pdb / FableWin.pdb spellings. Out of line: ApplyVertexShader 0x00988020, ApplyPixelShader 0x00988140,
// SetWorldTransform 0x009881F0, ResetWorldTransform 0x00988290, SetUserVertexShaderConstant(KMMMM) 0x00989B00, SetBoneTransform
// 0x0098A7F0, SetVSConstants 0x0098B930 (the CShaderRenderManager.cpp rows define its body in-TU, where VC7.1 expands it:
// UpdateObjectSpaceLighting 0x00989CB0 calls IDirect3DDevice9::SetVertexShaderConstantF in place) and the Update* helpers.
// Inline (no retail copy; expanded by every render pass): PrepareForRender(flags), SetPixelShaderConstant(index, value),
// SetBoneMatrixCount(count) (the STLport vector::resize it calls is out of line, 0x00B7EDF0), Get(), Set{Vertex,Pixel}ShadersEnable,
// PeekVertexShaderConstantLayout.
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
#include "fable_string.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CRGBColour.h"
#include "engine/CMatrix3x4.h"
#include "engine/CMatrix4x4.h"
#include "engine/CIndexedArray.h"
#include "engine/CPreTransposedBoneMatrix.h"
#include "engine/CSimpleMemoryPool.h"
#include "engine/CVertexShaderConstant.h"
#include "engine/CVertexShaderConstantLayout.h"
#include "engine/CPixelShader.h"
#include "engine/CShaderGenericLight.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/EVertexType.h"
#include "engine/EPrimitiveType.h"
struct IDirect3DVertexShader9;
struct IDirect3DVertexDeclaration9;
struct _D3DMATRIX;
class CRenderManager;
class CShaderResource;
class CShaderLight;
class CTexture;
class CVertexShader;
class CVertexBufferWin32;
class CIndexBuffer;

class CShaderRenderManager : public CBaseClassNonCopyable {
public:
    // ?CLightInfo@CShaderRenderManager: the light record (0x38 bytes: CShaderGenericLight 0x34, IsEnabled +0x34, UpdateFlag +0x35).
    class CLightInfo { public: CShaderGenericLight Light; bool IsEnabled; bool UpdateFlag; };
    CShaderRenderManager();                                              // 0x0098AB00 ??0CShaderRenderManager@@QAE@XZ
    virtual ~CShaderRenderManager();                                     // ??1CShaderRenderManager@@UAE@XZ
    static CShaderRenderManager& __fastcall Get() { return SingletonInstance; }   // ?Get@CShaderRenderManager@@SIAAV1@XZ
    void Initialise(CRenderManager* render_manager, IDirect3DDevice9* d3ddevice);  // 0x0098ACF0
    void Uninitialise();                                                 // 0x0098A7A0
    void EnableVertexShaders();                                          // 0x00987FE0
    void DisableVertexShaders();                                         // 0x009880E0
    void EnablePixelShaders();                                           // 0x00988110
    void DisablePixelShaders();                                          // 0x00988190
    void SetVertexShadersEnable(bool enable, bool* old) { if (old) *old = VertexShadersEnabled; if (enable != VertexShadersEnabled) { if (enable) EnableVertexShaders(); else DisableVertexShaders(); } }
    void SetPixelShadersEnable(bool enable, bool* old) { if (old) *old = PixelShadersEnabled; if (enable != PixelShadersEnabled) { if (enable) EnablePixelShaders(); else DisablePixelShaders(); } }
    void RegisterVertexShaderConstantLayout(long index, const CVertexShaderConstantLayout* layout);
    long PeekVertexShaderConstantLayout() { return CurrentVSConstantLayoutIndex; }
    void SetVertexShaderConstantLayout(long index);                      // 0x0098B5E0
    void UnregisterVertexShaderConstantLayouts();                        // 0x0098B280
    void ApplyVertexShader(const CVertexShader& shader, bool force);     // 0x00988020 ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z
    void ApplyPixelShader(const CPixelShader& shader);                   // 0x00988140 ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z
    void ResetStats();
    void GetStatsText(CCharString&, unsigned long);
    bool GetPixelShadersEnabled() const;                                 // ?GetPixelShadersEnabled@CShaderRenderManager@@QBE_NXZ
    bool GetVertexShadersEnabled() const;                                // ?GetVertexShadersEnabled@CShaderRenderManager@@QBE_NXZ
    bool IsFixedFunctionVertexMode() const;
    void SelectVertexBuffer(CVertexBufferWin32& buffer);                 // ?SelectVertexBuffer@CShaderRenderManager@@QAEXAAVCVertexBufferWin32@@@Z
    void DrawBuffersIndexed(const CTexture**, unsigned long, CVertexBufferWin32&, unsigned long, unsigned long, CIndexBuffer&, unsigned long, unsigned long, EPrimitiveType);
    void DrawBuffersIndexed(const CTexture**, unsigned long, unsigned long, unsigned long, CIndexBuffer&, unsigned long, unsigned long, EPrimitiveType);
    void DrawBuffersIndexed(CVertexBufferWin32&, unsigned long, unsigned long, CIndexBuffer&, unsigned long, unsigned long, EPrimitiveType);
    void DrawBuffersIndexed(unsigned long, unsigned long, CIndexBuffer&, unsigned long, unsigned long, EPrimitiveType);
    void DrawBuffersNonIndexed(const CTexture**, unsigned long, CVertexBufferWin32&, unsigned long, unsigned long, EPrimitiveType);
    void DrawBuffersNonIndexed(CVertexBufferWin32&, unsigned long, unsigned long, EPrimitiveType);
    void SetWorldTransform(const CMatrix3x4& matrix);                    // 0x009881F0 ?SetWorldTransform@CShaderRenderManager@@QAEXABVCMatrix3x4@@@Z
    void ResetWorldTransform();                                          // 0x00988290 ?ResetWorldTransform@CShaderRenderManager@@QAEXXZ
    void SetBoneTransform(unsigned long index, const CMatrix3x4& matrix);   // 0x0098A7F0 ?SetBoneTransform@CShaderRenderManager@@QAEXKABVCMatrix3x4@@@Z
    void SetBoneTransformTransposed(unsigned long, const CMatrix4x4&);
    void SetBoneTransforms(long, long, const CPreTransposedBoneMatrix*);
    void SetViewTransform(const CMatrix3x4&);                            // 0x00988350
    void SetCameraPos(const C3DVector&);
    void SetProjectionTransform(const CMatrix4x4&);                      // 0x00988540
    void SetProjectionTransform(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    void SetProjectionTransform(float, float, float, float);
    void SetTextureTransform(unsigned long stage, const CMatrix3x4& matrix);   // ?SetTextureTransform@CShaderRenderManager@@QAEXKABVCMatrix3x4@@@Z
    void SetTextureTransformCount(long count);                           // ?SetTextureTransformCount@CShaderRenderManager@@QAEXJ@Z
    void SetFogTransform(const C4DVector& transform);                    // ?SetFogTransform@CShaderRenderManager@@QAEXABVC4DVector@@@Z
    void SetXBoxUserVertexShaderConstant(long, const CRGBFloatColour&);
    void SetShadowTextureTransform(const CMatrix4x4&);
    void SetShadowFadeTransform(const C4DVector&);
    void SetShadowFadeFactors(float, float, float);
    void SetShadowSpotLightFallloffParameters(const CMatrix3x4&, float, float, const CRGBFloatColour&);
    void ResetLight(unsigned long);                                      // 0x00988640
    void InitialiseLight(unsigned long, const CShaderGenericLight&);
    void InitialiseLight(unsigned long, const CShaderLight&);            // 0x00989830
    void SetLightGlobal(unsigned long, const C4DVector&);
    void SetPixelShaderConstant(const CCharString&, const C4DVector&);
    long GetPixelShaderConstantTableIndex(const CCharString&);
    void SetAmbientColour(const CRGBFloatColour&);
    void SetFogColour(const CRGBFloatColour&);                           // 0x00988320
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00 ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKMMMM@Z
    void SetUserVertexShaderConstant(unsigned long index, const C4DVector& value);              // 0x00989AB0 ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKABVC4DVector@@@Z
    void SetUserVertexShaderConstant(unsigned long index, const C3DVector& value, float w);     // 0x00989A60 ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKABVC3DVector@@M@Z
    void SetUserVertexShaderConstant(unsigned long index, const CRGBFloatColour& value);        // 0x00989B50 ?SetUserVertexShaderConstant@CShaderRenderManager@@QAEXKABVCRGBFloatColour@@@Z
    void SetUserVertexShaderConstants(unsigned long index, long count, float* values);          // 0x009896D0 ?SetUserVertexShaderConstants@CShaderRenderManager@@QAEXKJPAM@Z
    void SetPosCompressionVertexShaderConstant(float, float, float, float, float, float, float, float);
    void SetPosCompressionVertexShaderConstant(float, float, float, float, float, float);
    void SetUVCompressionVertexShaderConstant(float, float, float, float);
    unsigned long GetMaxUserVertexShaderConstants() const;
    void SetVertexShaderConstantsDirect(unsigned long, const void*, unsigned long);
    bool GetSoftwareVertexShaderEnable() const;
    bool GetSoftwareFixedFunctionEnable() const;
    void SetSoftwareVertexShaderEnable(bool);
    void SetSoftwareFixedFunctionEnable(bool);
    CCountedPointer<CShaderResource> CreateResource();                   // 0x0098B860
    CVertexShader GetFixedFunctionVertexShader(EVertexType);
    IDirect3DDevice9* GetD3DDevice();                                    // ?GetD3DDevice@CShaderRenderManager@@QAEPAUIDirect3DDevice9@@XZ
    long GetMaximumLights();
    void OnPostViewportChanged();
    void SetMultisampleViewportScale(float);

    // ?SetPixelShaderConstant@CShaderRenderManager@@QAEXKABVC4DVector@@@Z (inline): copy the value into the constant table and mark
    // the pixel constants dirty (update flag 2).
    FABLE_FORCEINLINE void SetPixelShaderConstant(unsigned long index, const C4DVector& value)
    {
#ifdef FABLE_RETAIL_CSHADERRENDERMANAGER_SETPSCONST_INDEX
        PSConstTable[index] = value;
#else
        *(PSConstTable.begin() + index) = value;
#endif
        UpdateFlags |= 2;
    }
    // ?SetBoneMatrixCount@CShaderRenderManager@@QAEXJ@Z (inline): size the bone-matrix table (STLport vector::resize, out of line).
    FABLE_FORCEINLINE void SetBoneMatrixCount(long count)
    {
        BoneMatrices.resize(count, CPreTransposedBoneMatrix());
    }
    // ?PrepareForRender@CShaderRenderManager@@QAEXK@Z (inline; the landed 0x00B77E20 / 0x00B76BC0 / 0x00B7C510 expansions and
    // CRenderManager2D::Render2DDrawList 0x009DA9F0): flush every dirty piece of shader state the vertex format `flags` uses.
    FABLE_FORCEINLINE void PrepareForRender(unsigned long flags)
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
    static CShaderRenderManager SingletonInstance;                       // 0x013BC470 ?SingletonInstance@CShaderRenderManager@@0V1@A
    unsigned long CreateFixedFunctionVertexShader(const unsigned long*);
    CPixelShader CreatePixelShader(const CArray<unsigned long>&, const CArray<CCharString>&, const CCharString&);   // 0x0098A9A0
    void UpdateWorldTransform();
    void UpdateScreenSpaceTransform();
    void SetVSVectorAsColour(const CRGBColour&, float*);
    void SetVSColourConstant(long, const CRGBFloatColour&, float);
    void SetVSColourConstant(long, const CRGBFloatColour&);
    void SetVSVectorConstant(long, const C4DVector&);
    void SetVSVectorConstant(long, const C3DVector&, float);
    void SetVSConstant(long, float, float, float, float);
    void SetVSMatrixConstantTransposed(long, const CMatrix4x4&, unsigned long);
    void SetVSMatrixConstantTransposed(long, const CMatrix3x4&, unsigned long);
    void SetPSConstants(unsigned long, const void*, unsigned long);
    void SetD3DMatrix(_D3DMATRIX&, const CMatrix3x4&);
    void SetD3DMatrix(_D3DMATRIX&, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
    CVertexShaderConstant MultiplyMatrixColumn(const CVertexShaderConstant&, const CMatrix3x4&);
    void AllocateLights(unsigned long, unsigned long);
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
    void UpdateObjectSpaceLighting(unsigned long flags);                 // 0x00989CB0 ?UpdateObjectSpaceLighting@CShaderRenderManager@@AAEXK@Z
    void UpdateWorldSpaceLighting();                                     // 0x0098A540
    void UpdateLocalisedSphereMapping();                                 // 0x00988E80
    void UpdateCombinedProjectionTransform();                            // 0x00988A50
    void UpdateCameraPos();                                              // 0x00989700

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
    IDirect3DVertexDeclaration9*   CurrentVertexShaderVertexDeclairation;   // +0x1dc (original spelling)
    CPixelShader                   CurrentPixelShader;                   // +0x1e0
    bool                           WorldMatrixIsIdentity;                // +0x1e8
    unsigned char                  _pad_0x1e9[0x7];                      // +0x1e9 (CMatrix4x4 alignment; Ego_r +0x1FC _padding_)
    CMatrix4x4                     WorldMatrix;                          // +0x1f0
    CMatrix4x4                     ViewMatrix;                           // +0x230
    CMatrix4x4                     ProjectionMatrix;                     // +0x270
    CMatrix4x4                     ViewProjectionMatrix;                 // +0x2b0
    CMatrix4x4                     CombinedProjectionMatrix;             // +0x2f0
    CMatrix4x4                     TextureTransforms[1];                 // +0x330
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
    CIndexedArray<CCharString>     PSConstNames;                         // +0x3b4 (CArray + STLport map, 0x18)
    CArray<C4DVector>              PSConstTable;                         // +0x3cc
    unsigned long                  UpdateFlags;                          // +0x3d8
    CArray<CPreTransposedBoneMatrix> BoneMatrices;                       // +0x3dc
};
FABLE_STATIC_ASSERT(sizeof(CShaderRenderManager) == 0x3e8);

#ifdef FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW
// Retail opt-in (chunk F2, row 0x00BCFB00 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocksIndexBuffer): the bodies
// of two members retail expands there. ?SetBoneTransforms@CShaderRenderManager@@QAEXJJPBUCPreTransposedBoneMatrix@@@Z (Ego_r.pdb
// 0x00AE6440; no retail copy): three constant rows per matrix through the private SetVSConstants 0x0098B930. The six-argument
// ?DrawBuffersIndexed@..@@QAEXKKAAVCIndexBuffer@@KKW4EPrimitiveType@@@Z: the body the landed 0x00B8EB00 / 0x00BA7DB0 overloads expand
// (SetIndices, PT_NULL = the index buffer's own primitive type, RealiseRenderState 0x00A058C0, DrawIndexedPrimitive).
// The includer defines FABLE_RETAIL_CRENDERMANAGERCORE (engine/CRenderManager.h is included here for RealiseRenderState).
#include "engine/CIndexBuffer.h"
#include "engine/CRenderManager.h"
inline void CShaderRenderManager::SetBoneTransforms(long first_register, long count, const CPreTransposedBoneMatrix* matrices)
{
    SetVSConstants(first_register, matrices, count * 3);
}
inline void CShaderRenderManager::DrawBuffersIndexed(unsigned long min_vertex_index, unsigned long num_vertices, CIndexBuffer& index_buffer,
                                                     unsigned long start_index, unsigned long primitive_count, EPrimitiveType primitive_type)
{
    D3DDevice->SetIndices(index_buffer.PD3DIndexBuffer);
    if (primitive_type == PT_NULL)
        primitive_type = *reinterpret_cast<EPrimitiveType*>(index_buffer.PrimitiveType);
    RenderManager->RealiseRenderState();
    D3DDevice->DrawIndexedPrimitive(primitive_type, 0, min_vertex_index, num_vertices, start_index, primitive_count);
}
#endif

#endif // FABLE_ENGINE_CSHADERRENDERMANAGER_H
