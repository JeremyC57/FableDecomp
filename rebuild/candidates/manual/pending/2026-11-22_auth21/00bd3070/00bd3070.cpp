// CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive(CEngineSubPrimitiveAnimatedMesh& sub_primitive, ERenderPassType pass) -- draw one animated (skinned) mesh primitive for a render pass inside a render-state capture block
// Grade: DIFFER(11288v11288) -- 29 struct hunks (frame-masked; e17/s10C1Gn16; 14 struct hunks are relocation-mask artefacts, checked table+addend), 14 real hunk lines: AddStateToUpdateList load order sites 2-5 (0x00BD342A/3491/34FC/3567) and site 36 (0x00BD5003); bookmark test/or site 33 (0x00BD4C30); attach-loop SIB [esi+ecx] v [ecx+esi] (0x00BD5712/5718)
// Inferred: 16 dead release asserts (FABLE_RETAIL_ASSERT, fable_assert.h) -- a FITTED count (n=16 best of 0..22 with this text; 17 = same hunks plus 32aa x87 order with the old GetCurrent2DAlphaMaskInfo text); FableWin's row size (1674i = 1432i + 3 x 81i) implies 3 real asserts, so the count is not evidenced; they key the bookmark test/or windows (sites 3..14 / 22..32)
// Translation unit: the four CEnginePrimitiveManagerMeshBase texture getters (GetDiffuseTexture 0x00BA9360, GetBumpTexture 0x00BA95D0, GetReflectionTexture 0x00BA9830, Get2DAlphaMaskTexture 0x00BA9A70) lie in the static-mesh TU's range, not MeshBase's (0x00BEC000..): header COMDATs every caller TU compiles, so their bodies are defined above the row (wfB3's texts; each RELOCATION_MATCHes its retail row when graded from this file). c2 reads them for address capture of `failed` (lesson wfB5); without them material holds edi in the switch arms (171 struct); diagnostic e14/G2 (bodies with the same capture behaviour) gave the same flip
// Names: 0x01436E80 is spelled g_pEnginePrimitiveRenderer as in the landed getter rows (wfB3's text said NEngineInternal::GPPrimitiveRenderer; NAME_FIXES.tsv). FITTED (graded only): the dead-assert count (line 3); Z_WRITE `abort_render = failed;` order. EVIDENCED: getter bodies (each RELOCATION_MATCH), case order (jump table 0x00BD5C40)
// Fourteenth author (e17): EVIDENCED: arm 16 uses CEnginePrimitiveRenderUtil::Set2DAlphaMaskTextureMode(stage, filter, clamp) / DisableTextureFiltering(stage) (FableWin.pdb names, wfB3 0x00BA2350 + agentC6 0x00BB30A0 texts, fwsize SAME 24i/67b, 22i/64b; overlay opt-in FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ALPHAMASKTEXTUREMODE); GetCurrent2DAlphaMaskInfo through GFDebugPDynamicCast (twin text, fwsize SAME 16i/46b); the 2D pixel-shader getters with ADDITIVE || ADDSMOOTH (twin text); named `const CRGBFloatColour& reflection` (twin arm 17) in arms 7/10/13/17. FITTED: the helper in arm 16 only (the twin uses it in every 2D arm; all three here = every tie window one site early), the `{ }` around arms 7/10/13 reflection blocks, `&GraphicList.begin()[index]` in PeekGraphic
// Statement shape (fifth author): Z_WRITE `abort_render = failed;` before SetTextureFactor (FITTED order; gives retail block order 0x00BD4E71/0x00BD4F9D); SMOOTH_FILL ApplyPixelShader as if/else (EVIDENCED: hoisted push + shared call 0x00BD5379..0x00BD53D4); locals declared failed, environment_mapped, force_load, render_flags (retail store order 0x00BD33FF..0x00BD340A)
// Inline-budget: every FABLE_FORCEINLINE member body now comes from its shared header (AUTH21; advisor 2026-11-21 ruling (a)): CGraphicInfo.h
// _PEEKGRAPHIC + _PEEKGRAPHIC_FORCEINLINE / _PEEKGRAPHICINFOFRAME / _GETTEXTURE_INLINE, CRenderStateManager.h _SETTEXTUREFACTOR_INLINE,
// CRenderManagerCore.h _FLUSHRENDER_FORCEINLINE, CEngineInternalPrimitiveBase.h _GETALPHAINCLUDINGDISTANCEFADE(_FORCEINLINE),
// CEnginePrimitiveRenderUtil.h _APPLYPIXELSHADER / _SETTEXTUREFACTOR / _GETCURRENT2DALPHAMASKINFO / _ALPHAMASKTEXTUREMODE / _ALPHABLENDSTATES
// _FORCEINLINE, CEnginePrimitiveManagerMeshBase.h _GETAPPROPRIATEMIPMAP_FORCEINLINE; C3DVector.h _CTOR_FORCEINLINE (the C3DVector(x,y,z)
// at one of the three SetUserVertexShaderConstant sites is otherwise called once the budget is spent, as in 0x00CBFB7D). Plain inline
// leaves ApplyPixelShader called at 3 sites (retail expands all 25); /d2inlT /d2inlS move nothing (budget inliner).
// Leaf: ?RenderMeshPrimitive@CEnginePrimitiveManagerAnimatedMeshesBase@@QAEXAAVCEngineSubPrimitiveAnimatedMesh@@W4ERenderPassType@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_PASS22_ANIMATED_MESH_RENDER
#define FABLE_RETAIL_CENGINESUBPRIMITIVEANIMATEDMESH_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_PUBLICMEMBERS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_GETTEXTUREDECAL
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETTEXTURE_INLINE
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_SATRGBATEXTURE
#define FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES
#define FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#include "fable_assert.h"
#include "engine/GFDebugPDynamicCast.h"
#include "engine/CEnginePrimitiveEnums.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_GETALPHAINCLUDINGDISTANCEFADE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_GETALPHAINCLUDINGDISTANCEFADE_FORCEINLINE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_RESETOCCLUSIONINFO
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS_FORCEINLINE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_GETTEXTUREDECAL_FORCEINLINE
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_C3DPRIMITIVE2_ISDP3BUMP
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CRGBCOLOUR_SETCHANNEL
#define FABLE_RETAIL_C3DPRIMITIVE2_GETVERTEXBUFFER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_OCCLUSIONTEST
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_ANIMATEDMESH
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERANIMATEDMESHESBASE_HELPERS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ALPHABLENDSTATES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ANIMATEDMESH
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_GETMIPMAPSIZEMULTIPLIER_INLINE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ALPHAMASKTEXTUREMODE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_APPLYPIXELSHADER_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_GETCURRENT2DALPHAMASKINFO_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ALPHAMASKTEXTUREMODE_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_ALPHABLENDSTATES_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SHADERS
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER_ALPHAMASK
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_PEEKGLOBALSHADOWMODE
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_FADEDSHADOW_MODE_CHECK
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT_REFLECTION
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_SETTERS_ALPHAFLOAT
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_DEPTHTRANSFORM
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_CGRAPHICINFOFRAME_ISALPHA
#define FABLE_RETAIL_CGRAPHICINFO_GETTRANSPARENCYTYPE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CTOR_FORCEINLINE
#define FABLE_RETAIL_GFGETTOPBITINBYTE
#define FABLE_RETAIL_STLP_DECAL_HEAP
#define FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_XY_THEN_Z
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_GETASINT
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETRGBCOLOUR_LOCALS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#define FABLE_RETAIL_C4DVECTOR_INITIALISEFROMCOLOUR
#define FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER
#define FABLE_RETAIL_C3DMESHMATERIAL_GETDECALID
#define FABLE_RETAIL_CRGBFLOATCOLOUR_GETCHANNEL
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERSTATEMANAGER_SETTEXTUREFACTOR_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER_COLOURWRITE4
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRENDERMANAGERCORE_FLUSHRENDER_FORCEINLINE
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_CSHADERRENDERMANAGER_ENABLED_INLINE
#define FABLE_RETAIL_CSHADERRENDERMANAGER_SELECTVERTEXBUFFER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ZERORGBATEXTURE
#include "engine/CMatrix3x4.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEngineInternalPrimitiveMeshBase.h"
#include "engine/CEngineInternalPrimitiveAnimatedMesh.h"
#include "engine/CEngineInternalPrimitiveMorphInfo.h"
#include "engine/CEngineSubPrimitiveAnimatedMesh.h"
#include "engine/CEnginePrimitiveManagerOcclusionBase.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveManagerAnimatedMeshesBase.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/C2DAlphaMaskInfo.h"
#include "engine/CGraphicInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/C3DPrimitive2.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CEngineTextureDecal.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFastFloatToLong.h"
#include "engine/GFLimit.h"
#include "engine/GFGetNextPowerOfTwo.h"
#include "engine/CEnginePrimitiveRenderer.h"

extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;   // 0x01436E80 (landed spelling, rows 0x00BA9360 / 0x00BA95D0 / 0x00BA9A70)
namespace NEngineInternal {
extern CRenderManager* GPRenderManager;                                  // 0x01436E18
extern CEngineResourceManager* GPResourceManager;                        // 0x01436E98
extern CEngineOcclusionManager* GPOcclusionManager;                      // 0x01436E5C
extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; // 0x01436E78
extern CEngineLightingManager*             GPLightingManager;             // 0x01436E9C
extern CEngineCamera*                      GPCamera;                      // 0x01436EA0
extern CEngineShadowRenderer*              GPShadowRenderer;              // 0x01436E60
extern CEngineScreenEffectOutlineGlow*     GPScreenEffectOutlineGlow;     // 0x01436E3C
}
extern const float DELTA_NOTIONAL_ZERO;                                    // 0x0129BA3C

inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseOnly() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseOnly; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseColourTextureDiffuseAlpha() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseColourTextureDiffuseAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFill() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFill; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillAlpha() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillAlpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillDP3() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillDP3; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderOutlineSmoothFillDP3Alpha() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderOutlineSmoothFillDP3Alpha; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuse() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuse; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFog() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseFog; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFog() { return NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseEnvFog; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadow() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseFogShadowOutputFactor() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseFogShadowOutputFactor; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseEnvFogShadow() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseEnvFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseDP3BumpFogShadow() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseDP3BumpFogShadow; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderDiffuseDP3BumpFogShadowOutputFactor; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuse(EEnginePrimitiveBlendMode mode)
{
    if (mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE || mode == ENGINE_PRIMITIVE_BLEND_ADDSMOOTH)
        return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseAdditiveAlpha;
    return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuse;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseEnv(EEnginePrimitiveBlendMode mode)
{
    if (mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE || mode == ENGINE_PRIMITIVE_BLEND_ADDSMOOTH)
        return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseEnvAdditiveAlpha;
    return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseEnv;
}
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShader2DDiffuseDP3Bump(EEnginePrimitiveBlendMode mode)
{
    if (mode == ENGINE_PRIMITIVE_BLEND_ADDITIVE || mode == ENGINE_PRIMITIVE_BLEND_ADDSMOOTH)
        return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseDP3BumpAdditiveAlpha;
    return NEngineInternal::GPPrimitiveRendererSharedData->PixelShader2DDiffuseDP3Bump;
}
inline void __fastcall CEnginePrimitiveRenderUtil::SetWorldTransform(const CMatrix3x4& transform)
{
    if (NEngineInternal::GPShaderRenderManager->IsFixedFunctionVertexMode())
        NEngineInternal::GPRenderManager->SetWorldTransform(transform);
    else
        NEngineInternal::GPShaderRenderManager->SetWorldTransform(transform);
}
inline const CMatrix3x4& CEngineSubPrimitiveAnimatedMesh::PeekObjectTransform() const { return PeekParent()->PeekRenderInfo()->ObjectTransform; }
inline void CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitiveBlocks(C3DPrimitive2& primitive, const CMatrix4x4* bone_transforms)
{
    RenderPrimitiveBlocksIndexBuffer(primitive, bone_transforms);
}
const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(const C3DMeshMaterial* material,
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces,
    unsigned long mipmap_level,bool& failed,const CGraphicDataBank* graphic_bank,const bool block_on_load)
{
    long graphic_index=material->GetDiffuseMapBankIndex();
    if(graphic_index==0) return graphic_bank->GetBlankTexture(CGraphicDataBank::TEXTURE_TYPE_OPAQUE);
    if(composite_surfaces) {
        const CTexture* texture=GetComposedTexture(graphic_index,*composite_surfaces,mipmap_level,false,graphic_bank);
        if(texture) return texture;
    }
    const CGraphicInfoFrame* frame_info=graphic_bank->PeekGraphicInfoFrame(graphic_index,0);
    unsigned long get_flags=0;
    if(g_pEnginePrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=17;
    if(frame_info) {
        const CTexture* texture;
        if(frame_info->GetTexture(&texture,mipmap_level,get_flags)) return texture;
    }
    failed=true;
    return 0;
}
const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetBumpTexture(const C3DMeshMaterial* material,
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces,
    unsigned long mipmap_level,bool& failed,const CGraphicDataBank* graphic_bank,const bool block_on_load)
{
    long graphic_index=material->GetBumpMapBankIndex();
    if(graphic_index==0) return graphic_bank->GetBlankTexture(CGraphicDataBank::TEXTURE_TYPE_BUMP);
    if(composite_surfaces) {
        const CTexture* texture=GetComposedTexture(graphic_index,*composite_surfaces,mipmap_level,true,graphic_bank);
        if(texture) return texture;
    }
    const CGraphicInfoFrame* frame_info=graphic_bank->PeekGraphicInfoFrame(graphic_index,0);
    unsigned long get_flags=0;
    if(g_pEnginePrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=17;
    const CTexture* texture;
    if(frame_info->GetTexture(&texture,mipmap_level,get_flags)) return texture;
    return frame_info->GetBlankTexture();
}

const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetReflectionTexture(const C3DMeshMaterial* material, unsigned long mipmap_level, bool& used_blank,
    const CGraphicDataBank* bank, bool block_on_load)
{
    long graphic_index=material->GetReflectionMapBankIndex();
    if(graphic_index==0) return bank->GetBlankTexture(CGraphicDataBank::TEXTURE_TYPE_OPAQUE);
    const CGraphicInfoFrame* frame_info=bank->PeekGraphicInfoFrame(graphic_index,0);
    unsigned long get_flags=0;
    if(g_pEnginePrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=1;
    const CTexture* texture;
    if(frame_info->GetTexture(&texture,mipmap_level,get_flags)) return texture;
    return frame_info->GetBlankTexture();
}
const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::Get2DAlphaMaskTexture(unsigned long bank_index, unsigned long mipmap, bool& preloading,
    const CGraphicDataBank* bank, bool force)
{
    if(bank_index==0) return NEngineInternal::GPResourceManager->GetSatRGBATexture();
    const CGraphicInfoFrame* frame_info=bank->PeekGraphicInfoFrame(bank_index,0);
    unsigned long get_flags=(g_pEnginePrimitiveRenderer->GetBlockOnLoad() || force) ? 0x11 : 0;
    const CTexture* texture;
    if(frame_info->GetTexture(&texture,mipmap,get_flags)) return texture;
    return NEngineInternal::GPResourceManager->GetSatRGBATexture();
}
void CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive(CEngineSubPrimitiveAnimatedMesh& sub_primitive, ERenderPassType pass)
{
    CRenderStateScopedCaptureBlock capture_block;
    NEngineInternal::GPRenderManager->SetSpecularEnable(true);
    NEngineInternal::GPRenderManager->SetFogEnable(true);
    CEngineInternalPrimitiveAnimatedMesh* primitive = sub_primitive.GetParent();
    C3DPrimitive2* mesh_primitive = sub_primitive.MeshPrimitive;
    const C3DMesh2& mesh = primitive->PeekMesh();
    const C3DMeshMaterial* material = mesh.GetMaterial(mesh_primitive->GetMaterialIndex());
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces = 0;
    CEngineTextureDecal* decal = 0;
    CEngineInternalPrimitiveMorphInfo* morph_info = primitive->GetMorphInfo();
    if (morph_info) {
        composite_surfaces = &morph_info->CompositeSurfaces;
        decal = morph_info->GetTextureDecal(material->GetDiffuseMapBankIndex(), material->GetDecalID());
    }
    const CGraphicDataBank* graphic_bank = primitive->MeshInfo->PeekTextureBank();
    const CTexture* textures[4];
    unsigned long num_textures = 0;
    unsigned long mipmap_level = CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(NEngineInternal::GPCamera->PeekCameraPos(), sub_primitive.PeekObjectTransform(),
                                                                                       *mesh_primitive, sub_primitive.GetScale(), material, graphic_bank);
    bool failed = false;
    bool environment_mapped = false;
    bool force_load = (primitive->EnginePrimitiveFlags >> 7) & 1;
    unsigned long render_flags = sub_primitive.PeekRenderFlags();
    SetupAlphaBlendingStatesFromRenderFlags(render_flags, pass, sub_primitive.PeekAlpha(), sub_primitive.GetParent()->AlphaRef);
    if (render_flags & 0x80)
        SetupStencilMask(primitive->MeshEffect.Get(), render_flags, pass);
    CRGBColour tint = primitive->TintColour;
    bool use_output_factor = (tint.GetAsInt() & 0xffffff) != 0xffffff;
    if (decal)
        use_output_factor = true;
    bool abort_render = false;
    if (pass == RENDER_PASS_COMBINED) {
        NEngineInternal::GPLightingManager->SetCurrentEnvironmentThemeChannel(primitive->ThemeChannelDesc);
        const CEngineLocalLightArray& lights = sub_primitive.GetParent()->PeekLights();
        switch (sub_primitive.GetRenderMode()) {
        case 2:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShader(lights.LightCount, 0);
            ApplyPixelShader(GetPixelShaderDiffuse());
            break;
        case 3:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShaderSpotLight(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuse());
            NEngineInternal::GPLightingManager->SetupShaderSpotLight();
            break;
        case 4:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShader(lights.LightCount, 0);
            ApplyPixelShader(GetPixelShaderDiffuseFog());
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 8:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShader(lights.LightCount, 0);
            ApplyPixelShader(GetPixelShaderDiffuseFog());
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 6:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = NEngineInternal::GPResourceManager->GetZeroRGBATexture();
            textures[2] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 3;
            environment_mapped = true;
            ApplyPalSkinVertexShaderEnv(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuseEnvFog());
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 7:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 2;
            {
                const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
                NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            }
            environment_mapped = true;
            ApplyPalSkinVertexShaderEnv(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuseEnvFog());
            NEngineInternal::GPLightingManager->SetEnableFadedShadowFactor(true);
            break;
        case 9:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShaderShadow(lights.LightCount, 0);
            if (use_output_factor)
                ApplyPixelShader(GetPixelShaderDiffuseFogShadowOutputFactor());
            else
                ApplyPixelShader(GetPixelShaderDiffuseFogShadow());
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(1, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        case 10:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 2;
            {
                const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
                NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            }
            environment_mapped = true;
            ApplyPalSkinVertexShaderEnvShadow(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuseEnvFogShadow());
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(2, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        case 11:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = GetBumpTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 2;
            ApplyPalSkinVertexShaderDP3BumpShadow(lights.LightCount);
            if (use_output_factor)
                ApplyPixelShader(GetPixelShaderDiffuseDP3BumpFogShadowOutputFactor());
            else
                ApplyPixelShader(GetPixelShaderDiffuseDP3BumpFogShadow());
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(3, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            break;
        case 12:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 1;
            ApplyPalSkinVertexShaderSpotShadow(lights.LightCount, 0);
            if (use_output_factor)
                ApplyPixelShader(GetPixelShaderDiffuseFogShadowOutputFactor());
            else
                ApplyPixelShader(GetPixelShaderDiffuseFogShadow());
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(1, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            NEngineInternal::GPLightingManager->SetupShaderSpotLight(primitive->PeekRenderInfo()->ObjectTransform);
            break;
        case 13:
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 2;
            {
                const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
                NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            }
            environment_mapped = true;
            ApplyPalSkinVertexShaderEnvSpotShadow(lights.LightCount);
            ApplyPixelShader(GetPixelShaderDiffuseEnvFogShadow());
            NEngineInternal::GPShadowRenderer->SetupShadowFuncRenderStates(2, primitive->PeekRenderInfo()->ObjectTransform,
                                                                 CEngineShadowRenderer::CColorBufferDepthPackParameters(4, 5, 0));
            NEngineInternal::GPLightingManager->SetupShaderSpotLight(primitive->PeekRenderInfo()->ObjectTransform);
            break;
        case 1:
            ApplyPalSkinVertexShader(0, 0);
            ApplyPixelShader(GetPixelShaderDiffuseOnly());
            break;
        case 16: {
            const C2DAlphaMaskInfo* mask_info = GetCurrent2DAlphaMaskInfo();
            textures[0] = GetDiffuseTexture(material, composite_surfaces, 0, failed, graphic_bank, force_load);
            textures[1] = Get2DAlphaMaskTexture(mask_info->AlphaMaskBankIndex, 0, failed, mask_info->AlphaMaskBank, force_load);
            num_textures = 2;
            Set2DAlphaMaskTextureMode(1, mask_info->FilterMask, true);
            Apply2DPalSkinVertexShader();
            ApplyPixelShader(GetPixelShader2DDiffuse(ENGINE_PRIMITIVE_BLEND_SOLID));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask_info->AlphaMaskPositionFactors.X, mask_info->AlphaMaskPositionFactors.Y, mask_info->AlphaMaskPositionFactors.Z), mask_info->AlphaMaskPositionFactors.W);
            SetTextureFactor(CRGBFloatColour(1.0f, 1.0f, 1.0f, sub_primitive.GetAlphaAsFloat()));
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 17: {
            const C2DAlphaMaskInfo* mask_info = GetCurrent2DAlphaMaskInfo();
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, force_load);
            textures[1] = Get2DAlphaMaskTexture(mask_info->AlphaMaskBankIndex, 0, failed, mask_info->AlphaMaskBank, force_load);
            textures[2] = GetReflectionTexture(material, mipmap_level, failed, graphic_bank, force_load);
            num_textures = 3;
            if (!mask_info->FilterMask) {
                NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureMipMapMode(1, TEXTURE_MIPMAP_DISABLE);
            }
            NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_CLAMP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_CLAMP);
            const CRGBFloatColour& reflection = NEngineInternal::GPLightingManager->ThemeChannels[NEngineInternal::GPLightingManager->CurrentThemeChannel.ChannelIndex0].ReflectionColour;
            NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(0, C4DVector(reflection));
            environment_mapped = true;
            Apply2DPalSkinVertexShaderEnv();
            ApplyPixelShader(GetPixelShader2DDiffuseEnv(ENGINE_PRIMITIVE_BLEND_SOLID));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask_info->AlphaMaskPositionFactors.X, mask_info->AlphaMaskPositionFactors.Y, mask_info->AlphaMaskPositionFactors.Z), mask_info->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        case 18: {
            const C2DAlphaMaskInfo* mask_info = GetCurrent2DAlphaMaskInfo();
            textures[0] = GetDiffuseTexture(material, composite_surfaces, 0, failed, graphic_bank, force_load);
            textures[1] = GetBumpTexture(material, composite_surfaces, 0, failed, graphic_bank, force_load);
            textures[2] = textures[3] = Get2DAlphaMaskTexture(mask_info->AlphaMaskBankIndex, 0, failed, mask_info->AlphaMaskBank, force_load);
            num_textures = 4;
            if (!mask_info->FilterMask) {
                NEngineInternal::GPRenderManager->SetTextureFilterModeMag(2, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureFilterModeMin(2, TEXTURE_FILTER_POINT);
                NEngineInternal::GPRenderManager->SetTextureMipMapMode(2, TEXTURE_MIPMAP_DISABLE);
            }
            NEngineInternal::GPRenderManager->SetTextureUVModeU(2, TEXTURE_UV_CLAMP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(2, TEXTURE_UV_CLAMP);
            Apply2DPalSkinVertexShaderDP3Bump();
            ApplyPixelShader(GetPixelShader2DDiffuseDP3Bump(ENGINE_PRIMITIVE_BLEND_SOLID));
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C3DVector(mask_info->AlphaMaskPositionFactors.X, mask_info->AlphaMaskPositionFactors.Y, mask_info->AlphaMaskPositionFactors.Z), mask_info->AlphaMaskPositionFactors.W);
            NEngineInternal::GPRenderManager->SetFogEnable(false);
            break;
        }
        default:
            __assume(0);
        }
        NEngineInternal::GPLightingManager->SetSelfIlluminationAmbient(material->SelfIllumination);
        NEngineInternal::GPLightingManager->UpdateGlobalLightingStates();
        NEngineInternal::GPLightingManager->SetupShaderPointLights(lights);
        tint.SetA(sub_primitive.PeekAlpha());
        SetTextureFactor(CRGBFloatColour(tint));
        abort_render = failed;
    } else if (pass == RENDER_PASS_Z_WRITE) {
        num_textures = 0;
        if (material->GetDiffuseMapBankIndex() != 0 && graphic_bank->PeekGraphicInfoFrame(material->GetDiffuseMapBankIndex(), 0)->IsAlpha()) {
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, false);
            num_textures = 1;
        }
        ApplyPalSkinVertexShader(0, 0);
        ApplyPixelShader(GetPixelShaderDiffuseFog());
        abort_render = failed;
        SetTextureFactor(CRGBFloatColour(1.0f, 1.0f, 1.0f, 1.0f));
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_STENCIL) {
        SetupOutlineSmoothStencilStates(*(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get(), primitive->Scale);
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutline : VertexShaderPalSkinOutline);
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_ALPHA_STENCIL_MASK) {
        SetupOutlineSmoothAlphaStencilMaskStates(*(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get());
        num_textures = 0;
        if (material->GetDiffuseMapBankIndex() != 0) {
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, false);
            num_textures = 1;
        }
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutline : VertexShaderPalSkinOutline);
        ApplyPixelShader(GetPixelShaderDiffuseColourTextureDiffuseAlpha());
        abort_render = failed;
    } else if (pass == RENDER_PASS_OUTLINE_SMOOTH_FILL) {
        num_textures = 0;
        const CEngineMeshEffectOutline& effect = *(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get();
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutlineSmoothFill : VertexShaderPalSkinOutlineSmoothFill);
        NEngineInternal::GPScreenEffectOutlineGlow->SetupDepthBufferTransform(primitive->PeekRenderInfo()->ObjectTransform);
        long num_stages = mesh_primitive->IsDP3Bump() ? 2 : 1;
        if (material->GetDiffuseMapBankIndex() != 0 && material->Transparent) {
            textures[0] = GetDiffuseTexture(material, composite_surfaces, mipmap_level, failed, graphic_bank, false);
            num_textures = 1;
            if (mesh_primitive->IsDP3Bump())
                ApplyPixelShader(GetPixelShaderOutlineSmoothFillDP3Alpha());
            else
                ApplyPixelShader(GetPixelShaderOutlineSmoothFillAlpha());
            SetupOutlineSmoothFillStates(effect, primitive->Scale, true, num_stages);
            abort_render = failed;
        } else {
            if (mesh_primitive->IsDP3Bump())
                ApplyPixelShader(GetPixelShaderOutlineSmoothFillDP3());
            else
                ApplyPixelShader(GetPixelShaderOutlineSmoothFill());
            SetupOutlineSmoothFillStates(effect, primitive->Scale, false, num_stages);
        }
    } else if (pass == RENDER_PASS_OUTLINE_STENCIL) {
        SetupOutlineStencilStates(*(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get(), primitive->Scale);
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutline : VertexShaderPalSkinOutline);
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
    } else if (pass == RENDER_PASS_OUTLINE_FILL) {
        SetupOutlineFillStates(*(const CEngineMeshEffectOutline*)primitive->MeshEffect.Get(), primitive->Scale);
        ApplyVertexShader(mesh_primitive->IsDP3Bump() ? VertexShaderPalSkinDP3BumpOutline : VertexShaderPalSkinOutline);
        ApplyPixelShader(GetPixelShaderDiffuseOnly());
    }
    if (abort_render) {
        NEngineInternal::GPRenderManager->FlushRender();
        NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
        return;
    }
    unsigned long occlusion_handle;
    PrepareOcclusionTest(*primitive, pass, occlusion_handle);
    if (occlusion_handle != (unsigned long)-1)
        NEngineInternal::GPOcclusionManager->StartOcclusionTest();
    CEnginePrimitiveRenderUtil::SetWorldTransform(sub_primitive.PeekObjectTransform());
    NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive->GetVertexBuffer());
    unsigned long shader_flags = 0x10b;
    if (environment_mapped)
        shader_flags = 0x12b;
    NEngineInternal::GPRenderManager->RealiseRenderState();
    NEngineInternal::GPShaderRenderManager->PrepareForRender(shader_flags);
    const CMatrix4x4* bone_transforms = sub_primitive.GetParent()->GetBoneTransformations();
    for (unsigned long i = 0; i < num_textures; ++i)
        NEngineInternal::GPRenderManager->AttachTextureToStage(i, textures[i]);
    RenderPrimitiveBlocks(*mesh_primitive, bone_transforms);
    if (occlusion_handle != (unsigned long)-1)
        NEngineInternal::GPOcclusionManager->EndOcclusionTest(occlusion_handle);
    if (decal) {
        long overlay_alpha = primitive->GetAlphaIncludingDistanceFade(primitive->PeekRenderInfo()->Distance * TextureOverlayFadeDistandeFactor);
        if (overlay_alpha > 0) {
            CArray<const CTexture*> overlaid_textures;
            if (GetOverlaidTextures(*decal, overlaid_textures, mipmap_level, graphic_bank)) {
                long num_overlays = decal->OverlaidBlends.size();
                for (long i = 0; i < num_overlays; ++i) {
                    SetupOverlayAlphaBlendingStates(overlay_alpha, decal->OverlaidBlends[i].DiffuseColour);
                    NEngineInternal::GPRenderManager->AttachTextureToStage(0, overlaid_textures[i]);
                    NEngineInternal::GPRenderManager->RealiseRenderState();
                    NEngineInternal::GPShaderRenderManager->PrepareForRender(shader_flags);
                    RenderPrimitiveBlocks(*mesh_primitive, bone_transforms);
                }
            }
        }
    }
    for (unsigned long i = 0; i < num_textures; ++i)
        NEngineInternal::GPRenderManager->AttachTextureToStage(i, 0);
    NEngineInternal::GPLightingManager->RestoreDefaultPrimitiveParameters();
    NEngineInternal::GPRenderManager->FlushRender();
}
