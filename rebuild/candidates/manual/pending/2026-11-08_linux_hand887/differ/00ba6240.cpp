// CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive) -- draw a static mesh into the shadow buffer
// Residue: DIFFER(2935v2937), 74 normalized diff lines after 89 graded sources: frame-slot order (tol above centre; retail p shares tol's slot with the induction offset at -0x2c, ours shares tol with the offset and puts p at -0x3c; small this/material/GFF slot shifts), the ClipSphere offset load order and plane Dot product order (both follow the slot layout), the Type load hoisted above the RenderInfo stores, and the technique esi load (depends on the stand-in callee bodies).
// Next: run c2prio.py --trace on this source to read the slot/priority order of tol/centre/p, then move one value's first definition (a temporary vs a named local, the lever that fixed the scene-data test order); swap the fake non-clobbering SetupFirstTextures / RenderMeshPrimitiveBlocksIndexBuffer stand-ins for the landed bodies once they exist in the repo.
// Inline-budget: /d2inlS100 -- the capture ctor, the shaders dtor, GetHiddenMaterials and PeekGraphicInfo expand inline in retail; /d2inlS50..S54 leave them called (probe ladder in WORKLOG.md)
// Flags: /d2inlS100
// Leaf: ?RenderPrimitiveToShadowBuffer@CEnginePrimitiveManagerStaticMeshesBase@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_MULVECTOR
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_DTOR
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CPlane.h"
#include "engine/CArray.h"
#include "engine/CTexture.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/GFFloatToLongFloor.h"

extern const float DELTA_NOTIONAL_ZERO;   // 0x0129BA3C
enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
extern signed char g_highestSetBitTable[256];                                      // 0x0143B990

class CShadowClipVolume {
public:
    CPlane            ClipPlanes[8];       // +0x00
    long              ClipPlaneCount;      // +0x80
    C3DBoundingSphere BoundingSphere;      // +0x84
    bool              ClipToSphere;        // +0x94
    FABLE_FORCEINLINE bool ClipSphere(const C3DVector& centre, float radius) const
    {
        if (ClipToSphere) {
            const C3DVector offset = centre - BoundingSphere.Center;
            float range = BoundingSphere.Radius + radius;
            if ((offset.Y * offset.Y + offset.Z * offset.Z + offset.X * offset.X) > range * range)
                return true;
        }
        for (long i = 0; i < ClipPlaneCount; ++i) {
            if (centre.Dot(ClipPlanes[i].Normal) - radius > ClipPlanes[i].Distance)
                return true;
        }
        return false;
    }
};
class CShadowRendererSceneData {
public:
    CShadowRendererSceneData(const CShadowRendererSceneData&);   // 0x00B38600
    unsigned char     Matrices[0x110];     // +0x00 (WorldToShadowBuffer matrices, falloff, fade, light vector, camera pos)
    CShadowClipVolume CasterClipVolume;    // +0x110
    CShadowClipVolume RecieverClipVolume;  // +0x1a8
};
class CEngineShadowRenderer {
public:
    unsigned char            _base[0xc];
    CShadowRendererSceneData SceneData;                       // +0x0c
    unsigned char            _pad24c[0x25c - 0x24c];
    float                    ColourBufferShadowMinWClamp;     // +0x25c
    unsigned char            _pad260[0x32c - 0x260];
    CTexture                 DepthPackingLookupTexture;       // +0x32c
    long                     RenderingTechnique;              // +0x334
    const CShadowRendererSceneData PeekSceneData() { return SceneData; }
    long PeekRenderingTechnique() const { return RenderingTechnique; }
    CTexture& GetDepthPackingTexture() { return DepthPackingLookupTexture; }
    float PeekColourBufferShadowMinWClamp() const { return ColourBufferShadowMinWClamp; }
};
extern CEngineShadowRenderer* g_pEngineShadowRenderer;       // 0x01436E60

class CEngineCamera {
public:
    unsigned char _pad[0x54];
    C3DVector     WorldPosition;                              // +0x54 (CameraDesc.WorldPosition)
    const C3DVector& PeekCameraPos() const { return WorldPosition; }
};
extern CEngineCamera* g_pEngineCamera;                        // 0x01436EA0

class CEnginePrimitiveRendererShared {
public:
    unsigned char _pad[0x9c];
    float         LODErrorTolerance;                          // +0x9c
    unsigned char _padA0[8];
    float         LODVirtualScreenSizeFOVAdjusted;            // +0xa8
    float         MipmapQuality;                              // +0xac
    float         MipmapSizeMultiplier;                       // +0xb0
    unsigned char _padB4[0x21c - 0xb4];
    CPixelShader* PixelShaderShadowBufferRender;              // +0x21c
    CPixelShader* PixelShaderShadowBufferRenderAlpha;         // +0x220
};
extern CEnginePrimitiveRendererShared* g_pPrimitiveRenderer;  // 0x01436E78
inline float GetLODErrorTolerance() { return g_pPrimitiveRenderer->LODErrorTolerance; }
FABLE_FORCEINLINE float CalcDistanceFromCamera(const C3DVector& pos) { C3DVector d = pos - g_pEngineCamera->PeekCameraPos(); return (float)sqrt((d.Y*d.Y+d.Z*d.Z+d.X*d.X)); }

class CGraphicHeader { public: unsigned short Width, Height, Depth, FrameWidth; unsigned char _pad8[0x14]; };
class CGraphicInfo { public: CGraphicHeader Header; unsigned char _pad1c[0x10]; };   // 0x2c
class CGraphicDataBank {
public:
    unsigned char        _pad[0x1e0];
    CArray<CGraphicInfo> GraphicList;                         // +0x1e0
    const CGraphicInfo* PeekGraphicInfo(unsigned long index) const { return index < GraphicList.size() ? &GraphicList[index] : 0; }
};
class CMeshDataBank { public: unsigned char _pad[0x3d4]; CGraphicDataBank* TextureBank; };   // +0x3d4

class C3DMesh2;
class C3DMeshInfo {
public:
    void*             _vftable;
    CMeshDataBank*    Bank;                                   // +0x04
    unsigned long     Index;                                  // +0x08
    void*             MeshData;                               // +0x0c
    unsigned char     BoundingBox[0x18];                      // +0x10
    C3DBoundingSphere BoundingSphere;                         // +0x28
    float             SafeBoundingRadius;                     // +0x38
    long              PhysicsMeshIndex;                       // +0x3c
    float             LODErrors[4];                           // +0x40
    unsigned long     LODDataSizes[4];                        // +0x50
    bool GetMesh(CCountedPointer<const C3DMesh2>& mesh, unsigned long lod, unsigned long flags) const;   // 0x00A25E70
    unsigned long GetMaxLODs() const { return 4; }
    bool HasLOD(unsigned long lod) const { return LODDataSizes[lod] != 0; }
    float GetLODError(unsigned long lod) const { return LODErrors[lod]; }
};

class C3DMeshMaterial {
public:
    unsigned char _pad[0x10];
    unsigned long DiffuseMapBankIndex;                        // +0x10
    unsigned char _pad14[0x14];
    bool          TwoSided;                                   // +0x28
    bool          Transparent;                                // +0x29
    unsigned char _pad2a[6];
};
class C3DPrimitive2 {
public:
    unsigned char       _pad0[4];
    long                MaterialIndex;                        // +0x04
    long                RepeatingMeshReps;                    // +0x08
    C3DBoundingSphere   BoundingSphere;                       // +0x0c
    float               AverageTextureStretch;                // +0x1c
    unsigned char       _pad20[0x10];
    CVertexBufferWin32* VertexBuffer;                         // +0x30
    unsigned char       _pad34[0x5c - 0x34];
    unsigned long       TriangleCount;                        // +0x5c
    unsigned char       _pad60[0x40];
};
class C3DMesh2 {
public:
    unsigned char    _pad[0x8c];
    C3DMeshMaterial* Materials;                               // +0x8c
    unsigned long    PrimitiveCount;                          // +0x90
    C3DPrimitive2*   Primitives;                              // +0x94
};
class C3DTextureStages {
public:
    C3DTextureStages() : BumpTextureStage(-1), UseBlankTextures(false) { }
    long DiffuseTextureStage;
    long BumpTextureStage;
    bool UseBlankTextures;
};

template <class T> class CArrayView { public: T* _M_start; T* _M_finish; T* _M_end; };
class CEngineInternalPrimitiveMorphInfo {
public:
    unsigned char           TextureDecals[0x10];
    unsigned char           CompositeSurfaces[0x10];
    unsigned char           HiddenMaterialNamesHeader[4];
    unsigned int            HiddenMaterialNamesCount;     // +0x24
    unsigned char           HiddenMaterialNamesCompare[4];
    CArray<unsigned char>   HiddenMaterials;              // +0x2c
    void InitialiseHiddenMaterials(const C3DMesh2& mesh);   // 0x00BEC8D0
    const unsigned char* GetHiddenMaterials(const C3DMesh2& mesh)
    {
        if (HiddenMaterialNamesCount == 0)
            return 0;
        if (HiddenMaterials.empty())
            InitialiseHiddenMaterials(mesh);
        return HiddenMaterials.GetAsCArray();
    }
};
class CEngineInternalPrimitiveBase {
public:
    void*         _vftable;
    unsigned char _pad4[4];
    long          Type;                                       // +0x08
    unsigned long EnginePrimitiveFlags;                       // +0x0c
    unsigned char _pad10[0x38];
};
class CEngineInternalPrimitiveStaticMesh : public CEngineInternalPrimitiveBase {
public:
    unsigned char      _pad48[0x54 - 0x48];
    C3DMeshInfo*       ShadowMeshInfo;                        // +0x54
    float              Scale;                                 // +0x58
    unsigned char      _pad5c[0x70 - 0x5c];
    CMatrix3x4         ObjectTransform;                       // +0x70
    void*              RenderInfoLights;                      // +0xa0
    C3DMesh2*          RenderInfoMesh;                        // +0xa4
    unsigned long      RenderInfoLOD;                         // +0xa8
};
class CEngineInternalPrimitiveMorphedStaticMesh : public CEngineInternalPrimitiveStaticMesh {
public:
    CEngineInternalPrimitiveMorphInfo Morph;                  // +0xac
};

class CEnginePrimitiveManagerMeshBase : public CEnginePrimitiveManager {
public:
    static FABLE_FORCEINLINE unsigned long __fastcall GetAppropriateLOD(const C3DMeshInfo* mesh_info, float distance, float radius, float tolerance)
    {
        distance = distance > DELTA_NOTIONAL_ZERO ? distance : DELTA_NOTIONAL_ZERO;
        tolerance *= distance / (radius * g_pPrimitiveRenderer->LODVirtualScreenSizeFOVAdjusted);
        unsigned long lod = 0;
        bool searching = true;
        while (searching) {
            if (lod < mesh_info->GetMaxLODs() - 1 && mesh_info->HasLOD(lod + 1) && !(mesh_info->GetLODError(lod + 1) > tolerance))
                ++lod;
            else
                searching = false;
        }
        return lod;
    }
    static FABLE_FORCEINLINE unsigned long __fastcall GetAppropriateMipmap(const C3DVector& camera_pos, const CMatrix3x4& transform, const C3DPrimitive2& primitive,
                                                                        float scale, const C3DMeshMaterial* material, const CGraphicDataBank* texture_bank)
    {
        C3DBoundingSphere sphere = primitive.BoundingSphere;
        transform.TransformPoint(sphere.Center);
        const C3DVector offset = camera_pos - sphere.Center;
        float distance = ((float)sqrt((offset.Y * offset.Y + offset.Z * offset.Z + offset.X * offset.X)) - sphere.Radius * scale) * primitive.AverageTextureStretch;
        unsigned long mipmap = 0;
        long texture_index = material->DiffuseMapBankIndex;
        if (distance > DELTA_NOTIONAL_ZERO && texture_index > 0) {
            float size = texture_bank->PeekGraphicInfo(texture_index)->Header.FrameWidth / (g_pPrimitiveRenderer->MipmapSizeMultiplier / distance * scale);
            long level = GFFloatToLongFloor(size);
            if (level < 1)
                level = 1;
            else if (level > 255)
                level = 255;
            mipmap = (unsigned char)g_highestSetBitTable[(unsigned char)level];
        }
        return mipmap;
    }
    unsigned char _pad0c[0x14];
};
class CEnginePrimitiveManagerStaticMeshesBase : public CEnginePrimitiveManagerMeshBase {
public:
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& prim);
    bool SetupStaticBlocks(C3DPrimitive2& primitive, CEngineInternalPrimitiveStaticMesh& mesh, C3DTextureStages& stages,
                           const C3DMeshMaterial* material, void* compositeSurfaces, long mipmap, bool* usedBlank);   // 0x00BA50A0
    static void __fastcall SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& stages);                 // 0x00B9D670
    static void __fastcall RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const C3DTextureStages& stages);   // 0x00B9E5B0
    unsigned char  _pad20[0x308 - 0x20];
    CVertexShader* VertexShaderShadowBufferRender;            // +0x308
};

FABLE_FORCEINLINE void CRenderManagerCore::FlushRender()
{
    SelectVertexBuffer(0, 0);
    PD3DDevice->SetIndices(0);
    for (long stage = 0; stage < TextureStagesInUse; ++stage) {
        if (CurrentlySelectedTextures[stage]) {
            CurrentlySelectedTextures[stage] = 0;
            PD3DDevice->SetTexture(stage, 0);
        }
    }
    TextureStagesInUse = 0;
}
FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
}


#pragma auto_inline(off)
bool CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks(C3DPrimitive2& primitive, CEngineInternalPrimitiveStaticMesh& mesh, C3DTextureStages& stages,
                                                                const C3DMeshMaterial* material, void* compositeSurfaces, long mipmap, bool* usedBlank)
{
    *usedBlank = stages.UseBlankTextures;
    return primitive.TriangleCount != 0;
}
void __fastcall CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& stages)
{
    if (stages.DiffuseTextureStage != -1)
        primitive.RepeatingMeshReps = stages.DiffuseTextureStage;
}
void __fastcall CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const C3DTextureStages& stages)
{
    if (stages.BumpTextureStage != -1)
        primitive.RepeatingMeshReps = stages.BumpTextureStage;
}
#pragma auto_inline(on)
ERendered CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& prim)
{
    if (!Enable)
        return RENDERED_NOTHING;
    CEngineInternalPrimitiveStaticMesh& primitive = static_cast<CEngineInternalPrimitiveStaticMesh&>(prim);
    if (primitive.EnginePrimitiveFlags & 0x2000)
        return RENDERED_NOTHING;
    if (primitive.ShadowMeshInfo == 0)
        return RENDERED_NOTHING;

    CScopedShadersEnable shaders_enable(true, true);
    CRenderStateScopedCaptureBlock capture_block;

    float radius = primitive.ShadowMeshInfo->BoundingSphere.Radius * primitive.Scale;
    C3DVector centre = primitive.ObjectTransform * primitive.ShadowMeshInfo->BoundingSphere.Center;

    if (g_pEngineShadowRenderer->PeekSceneData().CasterClipVolume.ClipSphere(centre, radius))
        return RENDERED_NOTHING;

    unsigned long lod = GetAppropriateLOD(primitive.ShadowMeshInfo, CalcDistanceFromCamera(centre) - radius, radius,
                                          GetLODErrorTolerance());
    const C3DMeshInfo* mesh_info = primitive.ShadowMeshInfo;
    CCountedPointer<const C3DMesh2> mesh;
    if (!mesh_info->GetMesh(mesh, lod, 0))
        return RENDERED_NOTHING;
    primitive.RenderInfoMesh = const_cast<C3DMesh2*>(mesh.Data);
    primitive.RenderInfoLOD = lod;

    const unsigned char* hidden_materials = 0;
    if (primitive.Type == 10)
        hidden_materials = static_cast<CEngineInternalPrimitiveMorphedStaticMesh&>(primitive).Morph.GetHiddenMaterials(*mesh.Data);

    const CGraphicDataBank* texture_bank = primitive.ShadowMeshInfo->Bank->TextureBank;
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(primitive.ObjectTransform);
    NEngineInternal::GPRenderManager->SetAlphaRef(128);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER_EQUAL);

    for (unsigned long p = 0; p < mesh.Data->PrimitiveCount; ++p) {
        C3DPrimitive2& mesh_primitive = mesh.Data->Primitives[p];
        if (mesh_primitive.TriangleCount == 0)
            continue;
        const C3DMeshMaterial* material = &mesh.Data->Materials[mesh_primitive.MaterialIndex];
        if (hidden_materials && hidden_materials[mesh_primitive.MaterialIndex])
            continue;
        unsigned long mipmap = 0;
        C3DTextureStages texture_stages;
        if (g_pEngineShadowRenderer->PeekRenderingTechnique() == 2) {
            texture_stages.DiffuseTextureStage = 1;
            NEngineInternal::GPRenderManager->AttachTextureToStage(0, &g_pEngineShadowRenderer->GetDepthPackingTexture());
            NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
            NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
            NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
            C4DVector constant;
            constant.X = 128.0f;
            constant.Y = g_pEngineShadowRenderer->PeekColourBufferShadowMinWClamp();
            constant.Z = 0.0f;
            constant.W = 0.0f;
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, constant);
        } else {
            texture_stages.DiffuseTextureStage = 0;
        }
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*VertexShaderShadowBufferRender, true);
        if (material->Transparent) {
            if (material->DiffuseMapBankIndex == 0)
                continue;
            mipmap = GetAppropriateMipmap(g_pEngineCamera->PeekCameraPos(), primitive.ObjectTransform, mesh_primitive, primitive.Scale, material, texture_bank);
            texture_stages.UseBlankTextures = false;
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderShadowBufferRenderAlpha);
        } else {
            texture_stages.UseBlankTextures = true;
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderShadowBufferRender);
        }
        NEngineInternal::GPRenderManager->RealiseRenderState();
        NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
        NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive.VertexBuffer);
        bool used_blank_textures;
        if (SetupStaticBlocks(mesh_primitive, primitive, texture_stages, material, 0, mipmap, &used_blank_textures)) {
            SetupFirstTextures(mesh_primitive, texture_stages);
            RenderMeshPrimitiveBlocksIndexBuffer(mesh_primitive, texture_stages);
        }
    }
    NEngineInternal::GPRenderManager->FlushRender();
    return RENDERED_SUB_PRIMITIVES;
}
