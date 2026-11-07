// CEngineClothRenderer::Render2DClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& cloth, const C3DPrimitive2& primitive, const CEngineSubPrimitiveMeshBase& sub_mesh, CEngineInternalPrimitiveMeshBase& mesh, const CMatrix3x4& matrix, ERenderPassType pass, bool flag) -- render a 2D (flat) cloth primitive: pass-dependent alpha / z render state, the cloth vertex and pixel shaders, the material colour constant, then the polygons
// Grade: DIFFER(3522v3538, md=167, 36 lines ignoring registers and frame offsets) -- frame 0x38 v retail 0x34 (every esp operand shifts 4); the inline GetRGBColour stores each __ftol2 byte at once where retail holds R in bl and G in a spilled byte and stores R/G/B together after the third call; the Fog SetRenderState bookmark load-order flip (CRenderStateManager.h lane gap); one alignment pad
// Next idea: find the 4-byte frame slot (retail shares preloading / two_sided with cloth's argument slot and lights with mesh's): the agent860 visible-callee lever does not reach the CEnginePrimitiveManagerMeshBase texture getters (another TU), so try scoping textures / preloading into the RenderPolygons block and the GetRGBColour temporaries as named unsigned char locals
// Leaf: ?Render2DClothPrimitive@CEngineClothRenderer@@QAEXABVCParticleSimulation@@ABVC3DClothPrimitive@@ABVC3DPrimitive2@@ABVCEngineSubPrimitiveMeshBase@@AAVCEngineInternalPrimitiveMeshBase@@ABVCMatrix3x4@@W4ERenderPassType@@_N@Z
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CVertexShader.h"
#include "engine/CRGBColour.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CPixelShader.h"

namespace NEngineInternal {
extern CRenderManager* GPRenderManager;                                                // 0x01436E18
extern CShaderRenderManager* GPShaderRenderManager;                                    // 0x01436E14
}

class CParticleSimulation;
class CParticleProgram;
class C3DPrimitive2;
class CEngineLocalLightArray;
class CGraphicDataBank;
class C3DMeshMaterial;
class CCompositeSurfaceInfo;
class CVertexShader;
class CPixelShader;
enum ERenderPassType { RENDER_PASS_0, RENDER_PASS_1 };

// Local models (no shared header carries these members; Ego_r.pdb names, retail offsets).
// C3DMeshMaterial (0x30 bytes): SelfIllumination +0x24, TwoSided +0x28.
class C3DMeshMaterial {
public:
    unsigned char _pad_0x00[0x24];
    unsigned long SelfIllumination;                                                    // +0x24
    bool          TwoSided;                                                            // +0x28
    unsigned char _pad_0x29[0x30 - 0x29];
};
// C3DMesh2: Materials +0x8C (retail; Ego_r +0x94).
class C3DMesh2 { public: unsigned char _pad_0x00[0x8c]; C3DMeshMaterial* Materials; };
// CEngineRenderInfoMeshBase: Lights +0x00, Mesh +0x04, LOD +0x08.
class CEngineRenderInfoMeshBase { public: CEngineLocalLightArray* Lights; C3DMesh2* Mesh; unsigned long LOD; };
// CMeshDataBank::TextureBank +0x3D4 (engine/CMeshDataBank.h) reached through CMeshDataBankEntry::Bank +0x04 of C3DMeshInfo.
class CMeshDataBankModel { public: unsigned char _pad_0x00[0x3d4]; const CGraphicDataBank* TextureBank; };
class C3DMeshInfoModel { public: void* __vftable; CMeshDataBankModel* Bank; };
// CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> > (opaque here).
class CCompositeSurfaceMap;
// CEngineInternalPrimitiveMorphInfo: TextureDecals +0x00 (a 0x10-byte CVectorMap), CompositeSurfaces +0x10.
class CEngineInternalPrimitiveMorphInfo {
public:
    unsigned char        TextureDecals[0x10];                                          // +0x00
    unsigned char        CompositeSurfaces[0x10];                                      // +0x10
};

// CEngineSubPrimitiveMeshBase : CEngineSubPrimitiveBase: RenderFlags +0x04, Alpha +0x08, Parent +0x0C.
class CEngineSubPrimitiveMeshBase {
public:
    void*                         __vftable;                                           // +0x00
    unsigned long                 RenderFlags;                                         // +0x04
    unsigned char                 Alpha;                                               // +0x08
    CEngineInternalPrimitiveBase* Parent;                                              // +0x0c
};

class CEngineInternalPrimitiveMeshBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual const CEngineRenderInfoMeshBase* PeekRenderInfoMeshBase() const;           // slot 29 (+0x74)
    virtual void v30(); virtual void v31();
    virtual CEngineInternalPrimitiveMorphInfo* GetMorphInfo();                         // slot 32 (+0x80)
    unsigned char       _pad_0x04[0x50 - 0x04];
    C3DMeshInfoModel*   MeshInfo;                                                      // +0x50
    unsigned char       _pad_0x54[0x5c - 0x54];
    CRGBColour          TintColour;                                                    // +0x5c
};

class C3DClothPrimitive {
public:
    void*             __vftable;                                                       // +0x00
    CParticleProgram* ParticleProgram;                                                 // +0x04 CCountedPointer<CParticleProgram>
    unsigned char     _pad_0x08[0x0c - 0x08];
    unsigned long     PrimitiveIndex;                                                  // +0x0c
    unsigned long     MaterialIndex;                                                   // +0x10
};

// The 2D cloth record of the primitive manager table slot 37 (+0x94): render settings from +0x0C.
class CCloth2DRenderSettings {
public:
    const CGraphicDataBank* AlphaMaskBank;                                             // +0x00
    unsigned long           AlphaMaskIndex;                                            // +0x04
    C3DVector               Wind;                                                      // +0x08
    float                   WindW;                                                     // +0x14
    unsigned char           _pad_0x18[0x29 - 0x18];
    bool                    LinearFiltering;                                           // +0x29 (record +0x35)
};
class CCloth2DSettings {
public:
    unsigned char           _pad_0x00[0x0c];
    CCloth2DRenderSettings  Settings;                                                  // +0x0c
};
class CEnginePrimitiveManagerTable { public: unsigned char _pad_0x00[0x94]; CCloth2DSettings* Cloth2D; };   // +0x94
extern CEnginePrimitiveManagerTable* g_pPrimitiveManagers;                            // 0x01436E84
class CEngineSettingsModel { public: unsigned char _pad_0x00[0x1ac]; CPixelShader* Cloth2DPixelShader; };
extern CEngineSettingsModel* g_pEngineSettings;                                       // 0x01436E78

class CEngineLightingManagerModel {
public:
    unsigned char _pad_0x00[0xe4];
    unsigned long StateUpdateFlags;                                                    // +0xe4
    long          SelfIlluminationAmbientPercentage;                                   // +0xe8
    void SetSelfIlluminationAmbient(long percentage)
    {
        if (SelfIlluminationAmbientPercentage != percentage) {
            SelfIlluminationAmbientPercentage = percentage;
            StateUpdateFlags |= 2;
        }
    }
    void UpdateGlobalLightingStates();                                                 // 0x00B46C80
};
extern CEngineLightingManagerModel* g_pEngineLightingManager;                         // 0x01436E9C

class CEnginePrimitiveManagerMeshBase {
public:
    static const CTexture* __fastcall GetDiffuseTexture(const C3DMeshMaterial* material, CCompositeSurfaceMap* surfaces, unsigned long mipmap,
                                                       bool& preloading, const CGraphicDataBank* bank, bool force);   // 0x00BA9360
    static const CTexture* __fastcall Get2DAlphaMaskTexture(unsigned long bank_index, unsigned long mipmap, bool& preloading,
                                                           const CGraphicDataBank* bank, bool force);                 // 0x00BA9A70
};

class CEngineClothRenderer {
public:
    void*           __vftable;                                                         // +0x00
    CVertexShader*  TriVertexShaders;                                                  // +0x04 CArray<CVertexShader> _M_start
    unsigned char   _pad_0x08[0x40 - 0x08];
    CVertexShader*  TriVertexShadersShadow;                                            // +0x40
    CVertexShader*  TriVertexShadersSpotShadow;                                        // +0x44
private:
    void RenderPolygons(const CParticleSimulation& simulation, const CParticleProgram& program, const CMatrix3x4& matrix,
                        const CEngineLocalLightArray& lights, const CTexture** textures, unsigned long texture_count,
                        bool a, bool b, unsigned long c, bool d, bool e, unsigned long f, ERenderPassType pass,
                        CEngineInternalPrimitiveMeshBase& mesh);                       // 0x00BB2540
public:
    void Render2DClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& cloth,
                                const C3DPrimitive2& primitive, const CEngineSubPrimitiveMeshBase& sub_mesh,
                                CEngineInternalPrimitiveMeshBase& mesh, const CMatrix3x4& matrix, ERenderPassType pass,
                                bool flag);
};

__forceinline bool CShaderRenderManager::GetPixelShadersEnabled() const { return PixelShadersEnabled; }
__forceinline void CRenderStateManager::SetTextureFactor(unsigned long factor) { SetRenderState(factor, DeviceState.TextureFactor); }
__forceinline void CRenderManagerCore::FlushRender()
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
__forceinline void __fastcall CEnginePrimitiveRenderUtil::SetTextureFactor(const CRGBFloatColour& colour)
{
    if (NEngineInternal::GPShaderRenderManager->GetPixelShadersEnabled())
        NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(2, C4DVector(colour.R, colour.G, colour.B, colour.A));
    else
        NEngineInternal::GPRenderManager->SetTextureFactor(colour.GetRGBColour().IntValue);
}

void CEngineClothRenderer::Render2DClothPrimitive(const CParticleSimulation& simulation, const C3DClothPrimitive& cloth,
                                                  const C3DPrimitive2& primitive, const CEngineSubPrimitiveMeshBase& sub_mesh,
                                                  CEngineInternalPrimitiveMeshBase& mesh, const CMatrix3x4& matrix,
                                                  ERenderPassType pass, bool flag)
{
    const CParticleProgram* program = cloth.ParticleProgram;
    CRenderStateScopedCaptureBlock rsblock;
    {
        unsigned long flags = sub_mesh.RenderFlags;
        unsigned char alpha = sub_mesh.Alpha;
        unsigned char alpha_ref = sub_mesh.Parent->AlphaRef;
        if (pass == RENDER_PASS_1) {
            NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
            NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
            NEngineInternal::GPRenderManager->SetAlphaRef(alpha_ref);
            NEngineInternal::GPRenderManager->SetColourWriteEnable(false, false, false, false);
        } else {
            if (flags & 2) {
                NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
                NEngineInternal::GPRenderManager->SetAlphaRef((alpha_ref * alpha) >> 8);
            } else {
                NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
                NEngineInternal::GPRenderManager->SetAlphaRef(0);
            }
            if (flags & 1) {
                NEngineInternal::GPRenderManager->SetZBufferWriteEnable(false);
                NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
                NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
                NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
                NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
            } else {
                NEngineInternal::GPRenderManager->SetZBufferWriteEnable(true);
                NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
            }
        }
    }
    CScopedShadersEnable enabled(true, true);
    const CCloth2DRenderSettings* settings = &g_pPrimitiveManagers->Cloth2D->Settings;
    if (!settings->LinearFiltering) {
        NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
        NEngineInternal::GPRenderManager->SetTextureMipMapMode(1, TEXTURE_MIPMAP_LINEAR);
    }
    NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(settings->Wind), settings->WindW);
    if (sub_mesh.RenderFlags & 0x200) {
        if (sub_mesh.RenderFlags & 0x10)
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*TriVertexShadersSpotShadow, false);
        else
            NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*TriVertexShadersShadow, false);
    } else
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(TriVertexShaders[0], false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*(const CPixelShader*)&g_pEngineSettings->Cloth2DPixelShader);
    CEnginePrimitiveRenderUtil::SetTextureFactor(CRGBFloatColour(CRGBColour(mesh.TintColour.R, mesh.TintColour.G, mesh.TintColour.B, sub_mesh.Alpha)));
    {
    const CEngineRenderInfoMeshBase* render_info = mesh.PeekRenderInfoMeshBase();
    const CEngineLocalLightArray& lights = *render_info->Lights;
    const C3DMeshMaterial* material = &render_info->Mesh->Materials[cloth.MaterialIndex];
    CCompositeSurfaceMap* surfaces = 0;
    CEngineInternalPrimitiveMorphInfo* morph_info = mesh.GetMorphInfo();
    if (morph_info)
        surfaces = (CCompositeSurfaceMap*)morph_info->CompositeSurfaces;
    bool preloading = false;
    const CTexture* textures[2];
    textures[0] = CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(material, surfaces, 1, preloading, mesh.MeshInfo->Bank->TextureBank, false);
    textures[1] = CEnginePrimitiveManagerMeshBase::Get2DAlphaMaskTexture(settings->AlphaMaskIndex, 0, preloading,
                                                                         settings->AlphaMaskBank, false);
    if (preloading) {
        NEngineInternal::GPRenderManager->FlushRender();
        return;
    }
    g_pEngineLightingManager->SetSelfIlluminationAmbient(material->SelfIllumination);
    g_pEngineLightingManager->UpdateGlobalLightingStates();
    bool two_sided = material->TwoSided;
    NEngineInternal::GPRenderManager->SetFogEnable(false);
    RenderPolygons(simulation, *program, matrix, lights, textures, 1, true, two_sided,
                   sub_mesh.RenderFlags, true, true, 9, pass, mesh);
    NEngineInternal::GPRenderManager->FlushRender();
    }
}
