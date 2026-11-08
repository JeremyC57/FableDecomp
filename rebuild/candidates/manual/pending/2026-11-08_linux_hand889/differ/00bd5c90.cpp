// CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive) -- draw an animated (skinned) mesh into the shadow buffer
// Grade: DIFFER(2935v2935) -- 86 normalized diff lines after 140 grades: constant-buffer X/Y store order (retail stores X=128.0f then Y=MinWClamp with the +0x25c load hoisted; ours Y first), the primitive Type load hoisted above the RenderInfo rep movsd, the radius GetMagnitude x87 order (retail Z*Z product first), the centre-transform first product (`fld st(2); fmul [E11]` vs retail `fld [E11]; fmul st(3)`), SetRenderState bookmark test/or order (documented lane gap), SetAlphaTestEnable edx/esi roles, mipmap sphere Y pair order.
// Next: run c2prio.py --trace on the constant block and the centre transform to read the slot/priority order, then move one value's first definition (named local vs temporary); swap the scoped-shaders and capture-block stand-ins for landed bodies once they exist.
// Inline-budget: /d2inlS100 -- without it (probe/best1_noinl.cpp) the body is 2659v2935 with 31 calls vs retail 36: the scoped helpers retail expands inline stay out of line; same device as landed-pending 0x00BA6240 (chunk 887 ladder S50..S54 leaves them called)
// Leaf: ?RenderPrimitiveToShadowBuffer@CEnginePrimitiveManagerAnimatedMeshesBase@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@@Z
// Flags: /d2inlS100
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_MULVECTOR
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CRenderStateCaptureBlock.h"
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
    unsigned char _pad224[0x23c - 0x224];
    float         BoundingRadiusFix;                          // +0x23c
};
extern CEnginePrimitiveRendererShared* g_pPrimitiveRenderer;  // 0x01436E78
extern CEnginePrimitiveRendererShared* g_pPrimitiveRenderer_01436e80;   // 0x01436E80
FABLE_FORCEINLINE float Magnitude(const C3DVector& d) { return (float)sqrt((d.Y*d.Y+d.Z*d.Z+d.X*d.X)); }
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

class C3DMeshInfo {
public:
    void*             _vftable;
    CMeshDataBank*    Bank;                                   // +0x04
    unsigned long     Index;                                  // +0x08
    void*             MeshData;                               // +0x0c
    unsigned char     BoundingBox[0x18];                      // +0x10
    C3DBoundingSphere BoundingSphere;                         // +0x28
    C3DBoundingSphere GetBoundingSphere() const { return BoundingSphere; }
};
FABLE_FORCEINLINE const C3DVector GetCenter(const C3DBoundingSphere& s) { return s.Center; }

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
class CMatrix4x4;
class CCompositeSurfaceInfo;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
typedef CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> > CCompositeSurfaceMap;

class CEngineInternalPrimitiveMorphInfo {
public:
    unsigned char           TextureDecals[0x10];
    unsigned char           CompositeSurfaces[0x10];      // +0x10 CVectorMap<long, CCompositeSurfaceInfo, ...>
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

class CEnginePrimitive; class CEnginePrimitiveHandle; class CEnginePersistentPrimitiveBase; class CEngineFadeDistance;
class C3DBoundingBox; class CEnginePrimitiveAnimation; class CEnginePrimitivePosition; class CEngineMeshEffect;
class CEngineInternalPrimitiveOcclusionBase; class CEngineRenderInfoMeshBase;
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();                                                        // slot 0
    virtual void Delete();                                                                   // slot 1
    unsigned long IVRefCount;                                                                // +0x04
};
class CEnginePersistentPrimitiveBase_ : public CIVCountedPointeeBase {
public:
    long Type;                                                                               // +0x08
};
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase_ {
public:
    virtual void CollectStats();                                                             // slot 2
    virtual void NotifyReleased();                                                           // slot 3
    virtual void WasRendered();                                                              // slot 4
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);             // slot 5
    virtual unsigned long AddChildPrimitive(const CEnginePrimitive& child, unsigned long id);   // slot 6
    virtual void SetChildPrimitive(unsigned long index, CEnginePrimitiveHandle& handle, const CEnginePrimitive& child, unsigned long id);   // slot 7
    virtual void SetChildPrimitive(unsigned long index, const CEnginePrimitive& child, unsigned long id);   // slot 8
    virtual void GetChildPrimitiveHandle(unsigned long index, CEnginePrimitiveHandle& handle);  // slot 9
    virtual void ClearChildPrimitive(unsigned long index);                                   // slot 10
    virtual void ClearAllChildPrimitives();                                                  // slot 11
    virtual CEnginePersistentPrimitiveBase* GetChildPrimitive(unsigned long index);          // slot 12
    virtual void GetFadeDistance() const;                                                    // slot 13
    virtual void PostRender();                                                               // slot 14
    virtual bool GetWorldPosition(C3DVector& position);                                      // slot 15
    virtual bool GetBoundingBoxTransform(CMatrix3x4& transform);                             // slot 16
    virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);                              // slot 17
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere);                     // slot 18
    virtual bool GetBoundingSphereRadiusWorldSpace(float& radius);                           // slot 19
    virtual bool IsInsideFrustum(const CEngineCamera& camera, bool flag);                    // slot 20
    virtual bool GetObjectTransform(CMatrix3x4& transform) const;                            // slot 21
    virtual bool SetObjectTransform(const CMatrix3x4& transform);                            // slot 22
    virtual void SetAnimation(const CEnginePrimitiveAnimation& animation);                   // slot 23
    virtual void SetPosition(const CEnginePrimitivePosition& position);                      // slot 24
    virtual void SetMeshEffect(const void* effect);                                          // slot 25
    virtual bool GetMeshEffect(void* effect) const;                                          // slot 26
    virtual CEngineInternalPrimitiveOcclusionBase* GetAsOcclusionPrimitive();                // slot 27
    virtual void ResetOcclusionInfo();                                                       // slot 28
    unsigned long EnginePrimitiveFlags;                       // +0x0c
    unsigned char _pad10[0x2c - 0x10];
    long          InterpolationSet;                           // +0x2c
    unsigned char _pad30[0x48 - 0x30];
};
class CEngineInternalPrimitiveMeshBase : public CEngineInternalPrimitiveBase {
public:
    virtual const CEngineRenderInfoMeshBase* PeekRenderInfoMeshBase() const;                 // slot 29
    virtual bool GetTransformFrameStart(CMatrix3x4& transform) const;                        // slot 30
    virtual bool GetTransformFrameEnd(CMatrix3x4& transform) const;                          // slot 31
    virtual CEngineInternalPrimitiveMorphInfo* GetMorphInfo();                               // slot 32
    unsigned char      _pad48[0x50 - 0x48];
    const C3DMeshInfo* MeshInfo;                              // +0x50
    const C3DMeshInfo* ShadowMeshInfo;                        // +0x54
    float              Scale;                                 // +0x58
    unsigned char      _pad5c[0x70 - 0x5c];
};
class CEngineRenderInfoAnimatedMesh {
public:
    void*           Lights;                                   // +0x00
    const C3DMesh2* Mesh;                                     // +0x04
    unsigned long   LOD;                                      // +0x08
    CMatrix3x4      ObjectTransform;                          // +0x0c
    float           Distance;                                 // +0x3c
    CMatrix4x4*     BoneTransforms;                           // +0x40
    long            RenderFrame;                              // +0x44
};
class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase {
public:
    virtual void CalcInterpolatedObjectTransform(CMatrix3x4& transform, float time, bool scale) const;   // slot 33
    virtual void InitialiseSimulation(const C3DMeshInfo* info, const C3DMesh2& mesh);        // slot 34
    bool SetupMesh(float distance, float radius);              // 0x00BCF2E0
    const CMatrix4x4* GetBoneTransformations();                // 0x00BD2D90
    unsigned char                 _pad70[0xe0 - 0x70];
    CEngineRenderInfoAnimatedMesh RenderInfo;                 // +0xe0
};
class CEngineInternalPrimitiveMorphedAnimatedMesh : public CEngineInternalPrimitiveAnimatedMesh {
public:
    CEngineInternalPrimitiveMorphInfo Morph;                  // +0x128
};
class CEngineInternalPrimitive2DAnimatedMesh : public CEngineInternalPrimitiveAnimatedMesh {
public:
    unsigned char _pad128[0x150 - 0x128];                     // Initialised, Layer, ClipBox, ObjectBoundingSphere
};
class CEngineInternalPrimitive2DMorphedAnimatedMesh : public CEngineInternalPrimitive2DAnimatedMesh {
public:
    CEngineInternalPrimitiveMorphInfo Morph;                  // +0x150
};
struct SInterpolationTime { float Time; float m_4; float m_8; };   // 12-byte records
extern SInterpolationTime g_interpolationTimes[];                 // 0x0143B934

FABLE_FORCEINLINE void TP(const CMatrix3x4& m, C3DVector& point) {
    const float xx=point.X;
    const float yy=point.Y;
    const float zz=point.Z;
    point.X=m.E11*xx+(m.E21*yy+m.E31*zz)+m.E41;
    point.Y=m.E22*yy+(m.E12*xx+m.E32*zz)+m.E42;
    point.Z=m.E33*zz+(m.E13*xx+m.E23*yy)+m.E43;
}
class CEnginePrimitiveManagerMeshBase : public CEnginePrimitiveManager {
public:
    static FABLE_FORCEINLINE unsigned long __fastcall GetAppropriateMipmap(const C3DVector& camera_pos, const CMatrix3x4& transform, const C3DPrimitive2& primitive,
                                                                        float scale, const C3DMeshMaterial* material, const CGraphicDataBank* texture_bank)
    {
        C3DBoundingSphere sphere = primitive.BoundingSphere;
        TP(transform, sphere.Center);
        unsigned long mipmap = 0;
        const C3DVector offset = camera_pos - sphere.Center;
        float distance = ((float)sqrt((offset.Y * offset.Y + offset.Z * offset.Z + offset.X * offset.X)) - sphere.Radius * scale) * primitive.AverageTextureStretch;
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
    static const CTexture* __fastcall GetDiffuseTexture(const C3DMeshMaterial* material, CCompositeSurfaceMap* surfaces, unsigned long mipmap,
                                                        bool& preloading, const CGraphicDataBank* bank, bool force);   // 0x00BA9360
    unsigned char _pad0c[0x14];
};
class CEnginePrimitiveManagerAnimatedMeshesBase : public CEnginePrimitiveManagerMeshBase {
public:
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& prim);
    void RenderPrimitiveBlocksIndexBuffer(C3DPrimitive2& primitive, const CMatrix4x4* bones);   // 0x00BCFB00
    unsigned char  _pad20[0x2e8 - 0x20];
    CVertexShader* VertexShadersPalSkinShadowBufferRender;    // +0x2e8
};

FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& vertexBuffer)
{
    D3DDevice->SetStreamSource(0, vertexBuffer.PVertexBuffer, 0, vertexBuffer.Stride);
}

FABLE_FORCEINLINE C3DVector MulPoint(const CMatrix3x4& m, C3DVector v) {
    return C3DVector(m.E11*v.X+m.E31*v.Z+m.E21*v.Y+m.E41, m.E12*v.X+m.E32*v.Z+m.E22*v.Y+m.E42, m.E13*v.X+m.E33*v.Z+m.E23*v.Y+m.E43);
}
ERendered CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& prim)
{
    if (!Enable)
        return RENDERED_NOTHING;
    CEngineInternalPrimitiveAnimatedMesh& primitive = static_cast<CEngineInternalPrimitiveAnimatedMesh&>(prim);
    if (primitive.EnginePrimitiveFlags & 0x2000)
        return RENDERED_NOTHING;

    CScopedShadersEnable shaders_enable(true, true);
    CRenderStateScopedCaptureBlock capture_block;

    CMatrix3x4 transform;
    primitive.CalcInterpolatedObjectTransform(transform, g_interpolationTimes[primitive.InterpolationSet].Time, true);
    float radius = (primitive.MeshInfo->GetBoundingSphere().Radius + Magnitude(primitive.MeshInfo->GetBoundingSphere().Center)) * primitive.Scale
                 + g_pPrimitiveRenderer_01436e80->BoundingRadiusFix;
    C3DVector centre = MulPoint(transform, primitive.MeshInfo->BoundingSphere.Center);

    if (g_pEngineShadowRenderer->PeekSceneData().CasterClipVolume.ClipSphere(centre, radius))
        return RENDERED_NOTHING;
    float distance = CalcDistanceFromCamera(centre);
    float mesh_radius = primitive.MeshInfo->GetBoundingSphere().GetRadius() * primitive.Scale;
    if (!primitive.SetupMesh(distance, mesh_radius))
        return RENDERED_NOTHING;

    const C3DMesh2* mesh = primitive.RenderInfo.Mesh;
    primitive.RenderInfo.ObjectTransform = transform;
    CCompositeSurfaceMap* surfaces = 0;
    const unsigned char* hidden_materials = 0;
    if (primitive.Type == 11) {
        hidden_materials = static_cast<CEngineInternalPrimitiveMorphedAnimatedMesh&>(primitive).Morph.GetHiddenMaterials(*mesh);
        surfaces = (CCompositeSurfaceMap*)static_cast<CEngineInternalPrimitiveMorphedAnimatedMesh&>(primitive).Morph.CompositeSurfaces;
    } else if (primitive.Type == 38) {
        hidden_materials = static_cast<CEngineInternalPrimitive2DMorphedAnimatedMesh&>(primitive).Morph.GetHiddenMaterials(*mesh);
        surfaces = (CCompositeSurfaceMap*)static_cast<CEngineInternalPrimitive2DMorphedAnimatedMesh&>(primitive).Morph.CompositeSurfaces;
    }

    const CGraphicDataBank* texture_bank = primitive.MeshInfo->Bank->TextureBank;
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
    NEngineInternal::GPRenderManager->SetAlphaRef(128);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER_EQUAL);

    long diffuse_stage = 0;
    if (g_pEngineShadowRenderer->PeekRenderingTechnique() == 2) {
        diffuse_stage = 1;
        NEngineInternal::GPRenderManager->AttachTextureToStage(0, &g_pEngineShadowRenderer->GetDepthPackingTexture());
        NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
        {
            C4DVector constant;
            constant.Y = g_pEngineShadowRenderer->PeekColourBufferShadowMinWClamp();
            constant.X = 128.0f;
            constant.Z = 0.0f;
            constant.W = 0.0f;
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, constant);
        }
    }

    for (unsigned long p = 0; p < mesh->PrimitiveCount; ++p) {
        C3DPrimitive2& mesh_primitive = mesh->Primitives[p];
        if (mesh_primitive.TriangleCount == 0)
            continue;
        const C3DMeshMaterial* material = &mesh->Materials[mesh_primitive.MaterialIndex];
        if (hidden_materials && hidden_materials[mesh_primitive.MaterialIndex])
            continue;
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*VertexShadersPalSkinShadowBufferRender, true);
        const bool transparent = material->Transparent;
        if (transparent) {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderShadowBufferRenderAlpha);
        } else {
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*g_pPrimitiveRenderer->PixelShaderShadowBufferRender);
        }
        bool preloading = false;
        if (transparent) {
            if (material->DiffuseMapBankIndex == 0)
                continue;
            unsigned long mipmap = GetAppropriateMipmap(g_pEngineCamera->PeekCameraPos(), transform, mesh_primitive, primitive.Scale, material, texture_bank);
            const CTexture* texture = GetDiffuseTexture(material, surfaces, mipmap, preloading, texture_bank, false);
            if (!preloading)
                NEngineInternal::GPRenderManager->AttachTextureToStage(diffuse_stage, texture);
        }
        NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive.VertexBuffer);
        NEngineInternal::GPRenderManager->RealiseRenderState();
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0x401);
        if (!preloading)
            RenderPrimitiveBlocksIndexBuffer(mesh_primitive, primitive.GetBoneTransformations());
    }
    return RENDERED_SUB_PRIMITIVES;
}
