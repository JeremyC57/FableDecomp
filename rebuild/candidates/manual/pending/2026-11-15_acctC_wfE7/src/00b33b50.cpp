// CEnginePrimitiveRenderer::CEnginePrimitiveRenderer() -- construct the primitive renderer: the main and repeated-mesh scenes, every primitive manager (registered in order), the cloth renderer, the default render settings (software vertex processing below vs_1_1), every shared pixel shader by name (the colour-buffer shadow set only for ps_1_4+), the shadow-shader pointers, the sort distance and debug switches, the sprite enable console variables, and publish the shared data
// Leaf: ??0CEnginePrimitiveRenderer@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B33B50, 8858 bytes, thiscall `ret`, 8-aligned ebp frame, no EH (conditional-temporary flags), no /GS.
// Identity: Ego_r.pdb / FableWin.pdb ??0CEnginePrimitiveRenderer@@QAE@XZ (header decl); vtables 0x012A1380 / 0x012A1348 (RTTI); sole caller
//   CEngine::Initialise 0x00B2670C; donor order AddPrimitiveManager then this ctor (as retail 0x00B33AE0 / 0x00B33B50).
// Spellings EVIDENCED by retail bytes: implicit const char* -> CCharString for the scene names, shader names and console variables (the
//   temporary's address re-taken with `lea` after its ctor, 0x00B33CD7); each manager `new`ed straight into Reset; one AddPrimitiveManager per
//   manager (the converting copy temporary, released after the call); AddPrimitiveManager's landed body defined above the row (VC7.1 then keeps
//   each temporary's Info in edi across the call, 0x00B3411E); GetMeshBank twice (one load, CSE). FITTED: none.
//   Globals: NEngineInternal::GPPrimitiveRendererSharedData 0x01436E78 (coordinator / lessons C19 spelling), g_pEnginePrimitiveTypeInfo
//   0x01436E84 and g_pEngineDef_01436e20 0x01436E20 (landed bindings; no PDB name found).
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SHADERS
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKMESHBANK
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CPIXELSHADER
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDZSPRITES_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSPRITES_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERFLARESPRITES_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONGROUPMANAGER_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLELINEGROUP_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITETRAILGROUP_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHES_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEGROUP_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEMESHGROUP_CLASS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_OVERRIDES
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLEDECALGROUP_OVERRIDES
#define FABLE_RETAIL_CDISPLAYMANAGER_VERTEXSHADERVERSION
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETMESHBANK
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_SETMAXSORTDISTANCE_DECL
#define FABLE_RETAIL_CENGINEDEF_LODERRORTOLERANCE
#define FABLE_RETAIL_CMESHDATABANK_SETDP3BUMPENABLE
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_REGISTER
#define FABLE_RETAIL_CENGINECLOTHRENDERER_MEMBERS
#include "engine/CArray.h"
#include "engine/CCountedPointer.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveManagerOcclusionBase.h"
#include "engine/CEnginePrimitiveManagerStaticMeshes.h"
#include "engine/CEnginePrimitiveManagerAnimatedMeshesBase.h"
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include "engine/CEnginePrimitiveManagerRepeatedZSprites.h"
#include "engine/CEnginePrimitiveManagerSprites.h"
#include "engine/CEnginePrimitiveManagerFlareSprites.h"
#include "engine/CEnginePrimitiveManagerSpriteTrails.h"
#include "engine/CEnginePrimitiveOcclusionGroupManager.h"
#include "engine/CEnginePrimitiveManagerParticleLineGroup.h"
#include "engine/CEnginePrimitiveManagerParticleMeshGroup.h"
#include "engine/CEnginePrimitiveManagerParticleSpriteGroup.h"
#include "engine/CEnginePrimitiveManagerParticleSpriteTrailGroup.h"
#include "engine/CEnginePrimitiveManagerParticleGroup.h"
#include "engine/CEnginePrimitiveManagerParticleDecalGroup.h"
#include "engine/CEngineClothRenderer.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/CShaderRenderManager.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CMeshDataBank.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEngineDef.h"
#include "engine/CEnginePrimitiveTypeInfo.h"

// Classes without an engine header (local models, as the landed siblings do): RTTI bases from ghidra_out/rtti_bases.tsv, `new` sizes
// from the ctor, Ego_r.pdb / FableWin.pdb ctor and override names.
class CEngineScene : public CBaseClassNonCopyable {      // sizeof 0x5C
public:
    CEngineScene(float map_size, unsigned long max_map_levels, float max_primitive_cell_size_ratio, float max_average_primitives_per_cell,
                 const CCharString& name);               // 0x00B94A80 ??0CEngineScene@@QAE@MKMMABVCCharString@@@Z
private:
    unsigned char Unmodeled_0x04[0x5c - 0x04];
};
class CEnginePrimitiveManagerAnimatedMeshes : public CEnginePrimitiveManagerAnimatedMeshesBase {   // sizeof 0x690
public:
    CEnginePrimitiveManagerAnimatedMeshes(CArray<CEngineScene*>& scenes);   // 0x00BD1EF0
};
class CEnginePrimitiveManagerWeaponTrails : public CEnginePrimitiveManager {   // sizeof 0x30
public:
    CEnginePrimitiveManagerWeaponTrails(CArray<CEngineScene*>& scenes);   // 0x00BCC5F0
    virtual ~CEnginePrimitiveManagerWeaponTrails();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
private:
    unsigned char Unmodeled_0xc[0x30 - 0xc];
};
class CEnginePrimitiveManagerLines : public CEnginePrimitiveManager {   // sizeof 0x14
public:
    CEnginePrimitiveManagerLines(CArray<CEngineScene*>& scenes);   // 0x00BC7E90
    virtual ~CEnginePrimitiveManagerLines();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
private:
    unsigned char Unmodeled_0xc[0x14 - 0xc];
};
class CEnginePrimitiveManagerText : public CEnginePrimitiveManager {   // sizeof 0x2c
public:
    CEnginePrimitiveManagerText(CArray<CEngineScene*>& scenes);   // 0x00BC6B50
    virtual ~CEnginePrimitiveManagerText();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
private:
    unsigned char Unmodeled_0xc[0x2c - 0xc];
};
class CEnginePrimitiveGroupManager : public CEnginePrimitiveManagerOcclusionBase {   // sizeof 0x20
public:
    CEnginePrimitiveGroupManager(CArray<CEngineScene*>& scenes);   // 0x00BC61F0
    virtual ~CEnginePrimitiveGroupManager();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
};
class CEnginePrimitiveChangerManager : public CEnginePrimitiveManager {   // sizeof 0xc
public:
    CEnginePrimitiveChangerManager(CArray<CEngineScene*>& scenes);   // 0x00BC5570
    virtual ~CEnginePrimitiveChangerManager();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
};
class CEnginePrimitiveManagerDecal : public CEnginePrimitiveManager {   // sizeof 0x124
public:
    CEnginePrimitiveManagerDecal(CArray<CEngineScene*>& scenes);   // 0x00BBA480
    virtual ~CEnginePrimitiveManagerDecal();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
private:
    unsigned char Unmodeled_0xc[0x124 - 0xc];
};
class CEnginePrimitiveManagerWeatherMask : public CEnginePrimitiveManager {   // sizeof 0xc
public:
    CEnginePrimitiveManagerWeatherMask(CArray<CEngineScene*>& scenes);   // 0x00BB6A30
    virtual ~CEnginePrimitiveManagerWeatherMask();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType type);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList& types) const;
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& prim, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
};

namespace NEngineInternal {
extern CShaderRenderManager* GPShaderRenderManager;                     // 0x01436E14
extern CDisplayManager* GPDisplayManager;                               // 0x01436E1C
extern CEngineSubPrimitiveRenderer* GPSubPrimitiveRenderer;             // 0x01436E74
extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData;   // 0x01436E78
extern CEngineResourceManager* GPResourceManager;                       // 0x01436E98
}
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;           // 0x01436E84 (landed binding)
extern const CEngineDef* g_pEngineDef_01436e20;                         // 0x01436E20 (no PDB name found; landed binding)

// Same-TU callee defined above the row (retail 0x00B33AE0, the landed body's text on the shared headers): VC7.1 sees it does not
// write through its argument, so each AddPrimitiveManager temporary's Info stays in a register across the call (0x00B3411E).
void CEnginePrimitiveRenderer::AddPrimitiveManager(const CCountedPointer<CEnginePrimitiveManager>& manager)
{
    PrimitiveManagers.push_back(manager);
    g_pEnginePrimitiveTypeInfo->RegisterPrimitiveManager(manager.Get());
}

CEnginePrimitiveRenderer::CEnginePrimitiveRenderer()
{
    SceneList.push_back(new CEngineScene(128.0f, 6, 0.7f, 6.0f, "MainScene"));
    SceneList.push_back(new CEngineScene(128.0f, 6, 1.2f, 6.0f, "RepMeshScene"));
    RepeatedZSpriteManager.Reset(new CEnginePrimitiveManagerRepeatedZSprites(SceneList));
    StaticMeshManager.Reset(new CEnginePrimitiveManagerStaticMeshes(SceneList));
    AnimatedMeshManager.Reset(new CEnginePrimitiveManagerAnimatedMeshes(SceneList));
    RepeatedStaticMeshManager.Reset(new CEnginePrimitiveManagerRepeatedStaticMeshes(SceneList));
    SpriteManager.Reset(new CEnginePrimitiveManagerSprites(SceneList));
    FlareSpriteManager.Reset(new CEnginePrimitiveManagerFlareSprites(SceneList));
    WeaponTrailManager.Reset(new CEnginePrimitiveManagerWeaponTrails(SceneList));
    SpriteTrailManager.Reset(new CEnginePrimitiveManagerSpriteTrails(SceneList));
    LineManager.Reset(new CEnginePrimitiveManagerLines(SceneList));
    TextManager.Reset(new CEnginePrimitiveManagerText(SceneList));
    OcclusionGroupManager.Reset(new CEnginePrimitiveOcclusionGroupManager(SceneList));
    PrimitiveGroupManager.Reset(new CEnginePrimitiveGroupManager(SceneList));
    PrimitiveChangerManager.Reset(new CEnginePrimitiveChangerManager(SceneList));
    ParticleLineGroupManager.Reset(new CEnginePrimitiveManagerParticleLineGroup(SceneList));
    ParticleMeshGroupManager.Reset(new CEnginePrimitiveManagerParticleMeshGroup(SceneList));
    ParticleSpriteGroupManager.Reset(new CEnginePrimitiveManagerParticleSpriteGroup(SceneList));
    ParticleSpriteTrailGroupManager.Reset(new CEnginePrimitiveManagerParticleSpriteTrailGroup(SceneList, SpriteTrailManager));
    ParticleGroupManager.Reset(new CEnginePrimitiveManagerParticleGroup(SceneList));
    DecalManager.Reset(new CEnginePrimitiveManagerDecal(SceneList));
    DecalGroupManager.Reset(new CEnginePrimitiveManagerParticleDecalGroup(SceneList));
    WeatherMaskManager.Reset(new CEnginePrimitiveManagerWeatherMask(SceneList));
    AddPrimitiveManager(RepeatedZSpriteManager);
    AddPrimitiveManager(StaticMeshManager);
    AddPrimitiveManager(AnimatedMeshManager);
    AddPrimitiveManager(RepeatedStaticMeshManager);
    AddPrimitiveManager(SpriteManager);
    AddPrimitiveManager(FlareSpriteManager);
    AddPrimitiveManager(WeaponTrailManager);
    AddPrimitiveManager(SpriteTrailManager);
    AddPrimitiveManager(LineManager);
    AddPrimitiveManager(TextManager);
    AddPrimitiveManager(OcclusionGroupManager);
    AddPrimitiveManager(PrimitiveGroupManager);
    AddPrimitiveManager(PrimitiveChangerManager);
    AddPrimitiveManager(ParticleLineGroupManager);
    AddPrimitiveManager(ParticleMeshGroupManager);
    AddPrimitiveManager(ParticleSpriteGroupManager);
    AddPrimitiveManager(ParticleSpriteTrailGroupManager);
    AddPrimitiveManager(ParticleGroupManager);
    AddPrimitiveManager(DecalManager);
    AddPrimitiveManager(DecalGroupManager);
    AddPrimitiveManager(WeatherMaskManager);
    ClothRenderer.Reset(new CEngineClothRenderer());
    BlockOnLoad = false;
    OcclusionEnable = false;
    OcclusionHierarchyEnable = false;
    OcclusionHierarchyEnableConsole = false;
    CurrentOcclusionLayer = (EEngineOcclusionLayer)-1;
    BumpMappingEnable = false;
    DynamicVBUsage = 0;
    DynamicVBPool = (EVertexBufferPool)1;
    DynamicVBLockFlags = 0;
    DynamicIBUsage = 0;
    DynamicIBPool = (EIndexBufferPool)2;
    DynamicIBLockFlags = 0;
    GlobalPrimitiveInitFlags = 0;
    NEngineInternal::GPShaderRenderManager->SetSoftwareVertexShaderEnable(false);
    NEngineInternal::GPShaderRenderManager->SetSoftwareFixedFunctionEnable(false);
    if (NEngineInternal::GPDisplayManager->GetVertexShaderVersion() < 1100) {
        NEngineInternal::GPShaderRenderManager->SetSoftwareVertexShaderEnable(true);
        DynamicVBUsage |= 0x10;
        DynamicIBUsage |= 0x10;
        GlobalPrimitiveInitFlags |= 1;
    }
    if (NEngineInternal::GPResourceManager->PeekMeshBank((EMeshBankNo)0))
        NEngineInternal::GPResourceManager->GetMeshBank((EMeshBankNo)0)->SetDP3BumpEnable(BumpMappingEnable);
    PixelShaderDiffuseOnly = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_DIFFUSE_ONLY");
    PixelShaderDiffuseColourTextureDiffuseAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_DIFFUSE_COLOUR_TEXTURE_DIFFUSE_ALPHA");
    PixelShaderTextureOnly = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_ONLY");
    PixelShaderDiffuse = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE");
    PixelShaderDiffuseFog = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_FOG");
    PixelShaderDiffuseEnvFog = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_ENV_FOG");
    PixelShaderDiffuseEnvBumpFog = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_ENV_BUMP_FOG");
    PixelShader2DDiffuseOnly = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_DIFFUSE_ONLY");
    PixelShader2DTextureOnly = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_ONLY");
    PixelShader2DDiffuse = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_DIFFUSE");
    PixelShader2DDiffuseEnv = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_DIFFUSE_ENV");
    PixelShader2DDiffuseEnvBump = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_DIFFUSE_ENV_BUMP");
    PixelShader2DDiffuseOnlyAdditiveAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_DIFFUSE_ONLY_ADDITIVE_ALPHA");
    PixelShader2DTextureOnlyAdditiveAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_ONLY_ADDITIVE_ALPHA");
    PixelShader2DDiffuseAdditiveAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_DIFFUSE_ADDITIVE_ALPHA");
    PixelShader2DDiffuseEnvAdditiveAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_DIFFUSE_ENV_ADDITIVE_ALPHA");
    PixelShader2DDiffuseEnvBumpAdditiveAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_DIFFUSE_ENV_BUMP_ADDITIVE_ALPHA");
    PixelShaderOutlineSmoothFill = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_OUTLINE_SMOOTH_FILL");
    PixelShaderOutlineSmoothFillAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_OUTLINE_SMOOTH_FILL_ALPHA");
    PixelShaderOutlineSmoothFillDP3 = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_OUTLINE_SMOOTH_FILL_DP3");
    PixelShaderOutlineSmoothFillDP3Alpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_OUTLINE_SMOOTH_FILL_DP3_ALPHA");
    PixelShaderDepthShadowBufferRender = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SHADOW_BUFFER_RENDER");
    PixelShaderDepthShadowBufferRenderAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_SHADOW_BUFFER_RENDER_ALPHA");
    PixelShaderColourShadowBufferRender = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_COLBUFF_SHADOW_BUFFER_RENDER");
    PixelShaderColourShadowBufferRenderAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_COLBUFF_SHADOW_BUFFER_RENDER_ALPHA");
    PixelShaderDiffuseFogDepthBufferShadow = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_FOG_SHADOW");
    PixelShaderDiffuseFogDepthBufferShadowOutputFactor = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_FOG_SHADOW_OUTPUT_FACTOR");
    PixelShaderDiffuseDP3BumpFogDepthBufferShadow = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_DP3_BUMP_FOG_SHADOW");
    PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_DP3_BUMP_FOG_SHADOW_OUTPUT_FACTOR");
    PixelShaderDiffuseEnvFogDepthBufferShadow = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_ENV_FOG_SHADOW");
    PixelShaderDiffuseEnvBumpFogDepthBufferShadow = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_ENV_BUMP_FOG_SHADOW");
    PixelShader2DDiffuseDP3Bump = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_DIFFUSE_DP3_BUMP");
    PixelShader2DDiffuseDP3BumpAdditiveAlpha = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_2D_TEXTURE_DIFFUSE_DP3_BUMP_ADDITIVE_ALPHA");
    if (NEngineInternal::GPDisplayManager->GetPixelShaderVersion() >= 1400) {
        PixelShaderDiffuseFogColourBufferShadow = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_FOG_COLBUFF_SHADOW");
        PixelShaderDiffuseFogColourBufferShadowOutputFactor = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_FOG_COLBUFF_SHADOW_OUTPUT_FACTOR");
        PixelShaderDiffuseDP3BumpFogColourBufferShadow = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_DP3_BUMP_FOG_COLBUFF_SHADOW");
        PixelShaderDiffuseDP3BumpFogColourBufferShadowOutputFactor = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_DP3_BUMP_FOG_COLBUFF_SHADOW_OUTPUT_FACTOR");
        PixelShaderDiffuseEnvFogColourBufferShadow = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_ENV_FOG_COLBUFF_SHADOW");
        PixelShaderDiffuseEnvBumpFogColourBufferShadow = NEngineInternal::GPResourceManager->GetPShaderResource()->GetPixelShader("PSHADER_TEXTURE_DIFFUSE_ENV_BUMP_FOG_COLBUFF_SHADOW");
    }
    PixelShaderShadowBufferRender = &PixelShaderDepthShadowBufferRender;
    PixelShaderShadowBufferRenderAlpha = &PixelShaderDepthShadowBufferRenderAlpha;
    PixelShaderDiffuseFogShadow = &PixelShaderDiffuseFogDepthBufferShadow;
    PixelShaderDiffuseFogShadowOutputFactor = &PixelShaderDiffuseFogDepthBufferShadowOutputFactor;
    PixelShaderDiffuseEnvFogShadow = &PixelShaderDiffuseEnvFogDepthBufferShadow;
    PixelShaderDiffuseEnvBumpFogShadow = &PixelShaderDiffuseEnvBumpFogDepthBufferShadow;
    PixelShaderDiffuseDP3BumpFogShadow = &PixelShaderDiffuseDP3BumpFogDepthBufferShadow;
    PixelShaderDiffuseDP3BumpFogShadowOutputFactor = &PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor;
    NEngineInternal::GPSubPrimitiveRenderer->SetMaxSortDistance(10000.0f);
    LODVirtualScreenSize = 640.0f;
    LODVirtualScreenSizeFOVAdjusted = 640.0f;
    DrawDistanceFactor = 1.0f;
    DrawDistanceMinimum = 0.0f;
    DrawDistanceMaximum = 3.402823466e+38f;
    MaximumZSpriteAsMeshDistance = 3.402823466e+38f;
    DrawDistanceInvalid = false;
    RepeatedMeshDrawDistanceFactor = 1.0f;
    ZSpriteDrawDistanceFactor = 1.0f;
    MinimumZSpriteAsMeshDistance = 0.0f;
    LODErrorTolerance = g_pEngineDef_01436e20->LODErrorTolerance;
    CharacterLODErrorTolerance = g_pEngineDef_01436e20->CharacterLODErrorTolerance;
    MipmapQuality = 1.0f;
    MipmapSizeMultiplier = 0.0f;
    PreloadMipmapTolerance = 0;
    PreloadMipmapMaxSize = 256;
    BoundingSphereViewEnabled = false;
    BoundingBoxViewEnabled = false;
    ShowMeshInfo = 0;
    ShowPreloadColours = false;
    ShowVisibilityInfo = 0;
    ShowSilhouettes = 0;
    ShowSilhouettesConsole = 0;
    BoneViewEnabled = false;
    BoundingRadiusFix = 0.0f;
    MaxPreloadRadius = 15.0f;
    MinPreloadRadius = 5.0f;
    MaxPreloadMipmapTolerance = 4.0f;
    VisibilityRatiosUpdate = false;
    VisibilityRatiosReceiveUpdate = false;
    VisibilityRatiosValid = false;
    Show2DBoundingRect = false;
    Show2DBoundingSphere = false;
    ShowUser2DBoundingSphere = false;
    Show2DRotationPos = false;
    HighlightMirroredTexturesEnabled = false;
    LODForced = -1;
    SpriteManager->AddEnableConsoleVariable("EnableSprites");
    FlareSpriteManager->AddEnableConsoleVariable("EnableFlareSprites");
    NEngineInternal::GPPrimitiveRendererSharedData = this;
}
