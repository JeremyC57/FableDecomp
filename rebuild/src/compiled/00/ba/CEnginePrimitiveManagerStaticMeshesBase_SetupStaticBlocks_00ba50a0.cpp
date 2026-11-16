// CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks(C3DPrimitive2& mesh_prim, CEngineInternalPrimitiveStaticMesh& prim,
//   C3DTextureStages& stages, const C3DMeshMaterial* material, CVectorMap<long,CCompositeSurfaceInfo,...>* composite_surfaces,
//   long mipmap_level, bool* already_set_up) -- give every static block of a mesh primitive its diffuse / bump textures for this pass:
//   blank textures when the stages ask for them, nothing when the bank frame and the recorded stages show they are already set, else the
//   first block's from the material and every later block whose change flags say so from its own material; false if a texture failed
// Leaf: ?SetupStaticBlocks@CEnginePrimitiveManagerStaticMeshesBase@@AAE_NAAVC3DPrimitive2@@AAVCEngineInternalPrimitiveStaticMesh@@AAVC3DTextureStages@@PBVC3DMeshMaterial@@PAV?$CVectorMap@JVCCompositeSurfaceInfo@@V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@@@JPA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA50A0, 441 bytes (`ret 1Ch`), private thiscall. Caller: RenderMeshPrimitive 0x00BA5260 (landed).
// Identity: Ego_r.pdb ?SetupStaticBlocks@CEnginePrimitiveManagerStaticMeshesBase@@AAE_N...@Z (0x00A5B8F0; SetupStaticBlocksBlankTextures is Ego_r
//   0x00A5A8A0, retail 0x00BA4A40); the body is acctC's (wfB3p), matched inside the 0x00BA5260 TU and never staged
//   as its own row; landed 2026-11-16 from that TU's text.
// Callees: SetupStaticBlocksBlankTextures 0x00BA4A40, CEnginePrimitiveManagerMeshBase::GetDiffuseTexture 0x00BA9360 (twice) / GetBumpTexture
//   0x00BA95D0 (twice) (fastcall statics, out of line; their bodies are kept visible here as in the retail TU); the rest is inline.
// Translation unit: the retail TU of 0x00BA2350..0x00BA5260 (see the landed 0x00BA5260 row): this source is that row's preamble (lines
//   31-195 and its NEngineInternal globals) plus the stretch from CGraphicDataBank::PeekGraphicInfoFrame to this function, verbatim;
//   the whole TU text grades the same (RELOCATION_MATCH).
#include "fable_compiler.h"
#define FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_SATRGBATEXTURE
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_C4DVECTOR_INITIALISEFROMCOLOUR
#define FABLE_RETAIL_CRGBFLOATCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS_BLOCKDRAW
#define FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_BLANKTEXTURES
#define FABLE_RETAIL_C3DPRIMITIVE2_RECORDEDSTATE
#define FABLE_RETAIL_C3DTEXTURESTAGES_EQUALS
#define FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_STATICBLOCKS
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_GETVERTEXBUFFER
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CENGINEMESHEFFECT
#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_RENDERMESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPAREOCCLUSIONTEST
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_ACCESSORS
#define FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_ACCESSORS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DTEXTURESTAGES_CTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_NAMED_SQUARED
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKMESH_RENDERINFO
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_MEMBERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_OCCLUSIONTEST
#define FABLE_RETAIL_C3DPRIMITIVE2_ISDP3BUMP
#define FABLE_RETAIL_C3DPRIMITIVE2_ISDP3BUMP_MASK
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKRENDERINFO
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_SETUPRENDERMODE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_REFLECTIONTEXTURE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_2DALPHAMASK
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETTEXTUREFACTOR
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTHSHADERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_STATICMESHSHADERS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SETUPBLANKTEXTURES
#define FABLE_RETAIL_CENGINESUBPRIMITIVESTATICMESH_PARENTBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKLIGHTS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SPOTLIGHT
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD_DIRECT
#define FABLE_RETAIL_CENGINECAMERA_PEEKWINDOW
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_CLOTH2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SETUPRENDERMODE
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#define FABLE_RETAIL_CSHADERRENDERMANAGER_PSCONST_INDEX
#define FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_MEMBER
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC
#define FABLE_RETAIL_CENGINESHADOWRENDERER_SHADOWFUNC_CTOR
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_DEPTHBUFFERTRANSFORM
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_ZERORGBATEXTURE
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVE2DVIEWPORTMANAGER_ALPHAMASK
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CEngineSubPrimitiveStaticMesh.h"
#include "engine/CEngineInternalPrimitiveMorphedStaticMesh.h"
#include "engine/CGraphicInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DTextureStages.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFLimit.h"
#include "engine/CEngineMeshEffect.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/CTexture.h"
#include "engine/CRGBColour.h"
#include "engine/CRGBFloatColour.h"
#include "engine/C4DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitive2DViewportManager.h"
#include "engine/C2DAlphaMaskInfo.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/GFDebugPDynamicCast.h"

namespace NEngineInternal { extern CRenderManager* GPRenderManager; }                  // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineCamera* GPCamera; }                                                // 0x01436EA0
namespace NEngineInternal { extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; }   // 0x01436E78
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; }                       // 0x01436E80
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }                              // 0x01436E9C
namespace NEngineInternal { extern CEngineOcclusionManager* GPOcclusionManager; }                                  // 0x01436E5C
extern unsigned char GTopBitInByteTable[256];                                       // 0x0143B990
extern const float DELTA_NOTIONAL_ZERO;                                               // 0x0129BA3C

namespace NEngineInternal { extern CEngineScreenEffectOutlineGlow* GPScreenEffectOutlineGlow; } // 0x01436E3C
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }                                        // 0x01436E60
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }                                            // 0x01436E98
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }                                  // 0x01436E84
FABLE_FORCEINLINE const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic,unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}
bool CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures(C3DPrimitive2& mesh_prim, CEngineInternalPrimitiveStaticMesh& prim,
    C3DTextureStages& stages, const C3DMeshMaterial* material, bool* already_set_up)
{
    const CGraphicDataBank* graphic_bank=prim.MeshInfo->PeekTextureBank();
    long frame=graphic_bank->GetCurrentFrame();
    if(frame==mesh_prim.GetRecordedFrame() && stages==mesh_prim.PeekRecordedTextureStages()) {
        *already_set_up=true;
        return true;
    }
    *already_set_up=false;
    N3DPrimitive::CStaticBlock& first_block=*mesh_prim.GetStaticBlock(0);
    if(stages.DiffuseTextureStage!=-1) {
        CGraphicDataBank::ETextureType texture_type=CGraphicDataBank::TEXTURE_TYPE_OPAQUE;
        unsigned long graphic_index=material->GetDiffuseMapBankIndex();
        if(graphic_index!=0) texture_type=graphic_bank->PeekGraphicInfoFrame(graphic_index,0)->GetTextureType();
        first_block.DiffuseTexture=graphic_bank->GetBlankTexture(texture_type);
    }
    if(stages.BumpTextureStage!=-1) {
        CGraphicDataBank::ETextureType texture_type=CGraphicDataBank::TEXTURE_TYPE_BUMP;
        unsigned long graphic_index=material->GetBumpMapBankIndex();
        if(graphic_index!=0) texture_type=graphic_bank->PeekGraphicInfoFrame(graphic_index,0)->GetTextureType();
        first_block.BumpTexture=graphic_bank->GetBlankTexture(texture_type);
    }
    unsigned long no_blocks=mesh_prim.GetStaticBlockCount();
    for(unsigned long c_block=1;c_block<no_blocks;++c_block) {
        N3DPrimitive::CStaticBlock& block_info=*mesh_prim.GetStaticBlock(c_block);
        block_info.DiffuseTexture=first_block.DiffuseTexture;
        block_info.BumpTexture=first_block.BumpTexture;
    }
    mesh_prim.SetRecordedFrame(frame);
    mesh_prim.SetRecordedTextureStages(stages);
    return true;
}
FABLE_FORCEINLINE bool CGraphicInfoFrame::GetTexture(const CTexture** texture,unsigned long mipmap_level,unsigned long flags) const
{
    if(!Data.IsNull()) {
        if(Data->Frame.FrameData) {
            if(!(flags&8)) TouchFrame(mipmap_level);
            if(mipmap_level<Data->CurrentMipmapLevel) {
                Data->BeginLoading(mipmap_level,flags);
                if(!(flags&16) || !Data->FileData.Data) {
                    *texture=Data->Frame.FrameData->GetTexture();
                    return true;
                }
            } else {
                *texture=Data->Frame.FrameData->GetTexture();
                return true;
            }
        }
        Data->BeginLoading(mipmap_level,flags);
    } else {
        if(flags&4) return false;
        Data.Reset(new CFrameData(this));
        Data->BeginLoading(mipmap_level,flags);
    }
    if(!(flags&1) || !InternalGetFrameForcedLoading(flags)) return false;
    *texture=Data->Frame.FrameData->GetTexture();
    return true;
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
    if(NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=17;
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
    if(NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=17;
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
    if(NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad() || block_on_load) get_flags=1;
    const CTexture* texture;
    if(frame_info->GetTexture(&texture,mipmap_level,get_flags)) return texture;
    return frame_info->GetBlankTexture();
}
const CTexture* FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::Get2DAlphaMaskTexture(unsigned long bank_index, unsigned long mipmap, bool& preloading,
    const CGraphicDataBank* bank, bool force)
{
    if(bank_index==0) return NEngineInternal::GPResourceManager->GetSatRGBATexture();
    const CGraphicInfoFrame* frame_info=bank->PeekGraphicInfoFrame(bank_index,0);
    unsigned long get_flags=(NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad() || force) ? 0x11 : 0;
    const CTexture* texture;
    if(frame_info->GetTexture(&texture,mipmap,get_flags)) return texture;
    return NEngineInternal::GPResourceManager->GetSatRGBATexture();
}

bool CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocks(C3DPrimitive2& mesh_prim, CEngineInternalPrimitiveStaticMesh& prim,
    C3DTextureStages& stages, const C3DMeshMaterial* material,
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >* composite_surfaces,
    long mipmap_level, bool* already_set_up)
{
    if(stages.UseBlankTextures) {
        SetupStaticBlocksBlankTextures(mesh_prim,prim,stages,material,already_set_up);
        return true;
    }
    bool failed=false;
    const CGraphicDataBank* graphic_bank=prim.MeshInfo->PeekTextureBank();
    const C3DMesh2& mesh=prim.PeekMesh();
    const bool block_on_load=(prim.EnginePrimitiveFlags>>7)&1;
    if(composite_surfaces==0 && graphic_bank->GetCurrentFrame()==mesh_prim.GetRecordedFrame() && stages==mesh_prim.PeekRecordedTextureStages()) {
        *already_set_up=true;
        return true;
    }
    *already_set_up=false;
    N3DPrimitive::CStaticBlock& first_block=*mesh_prim.GetStaticBlock(0);
    if(stages.DiffuseTextureStage!=-1)
        first_block.DiffuseTexture=CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(material,composite_surfaces,mipmap_level,failed,graphic_bank,block_on_load);
    if(stages.BumpTextureStage!=-1)
        first_block.BumpTexture=CEnginePrimitiveManagerMeshBase::GetBumpTexture(material,composite_surfaces,mipmap_level,failed,graphic_bank,block_on_load);
    unsigned long no_blocks=mesh_prim.GetStaticBlockCount();
    for(unsigned long c_block=1;c_block<no_blocks;++c_block) {
        N3DPrimitive::CStaticBlock& block_info=*mesh_prim.GetStaticBlock(c_block);
        const C3DMeshMaterial* current_material=mesh.GetMaterial(block_info.GetMaterialIndex());
        if(block_info.GetChangeFlags()&1) {
            if(stages.DiffuseTextureStage!=-1)
                block_info.DiffuseTexture=CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(current_material,composite_surfaces,mipmap_level,failed,graphic_bank,block_on_load);
            else failed=true;
        }
        if((block_info.GetChangeFlags()&2)&&stages.BumpTextureStage!=-1)
            block_info.BumpTexture=CEnginePrimitiveManagerMeshBase::GetBumpTexture(current_material,composite_surfaces,mipmap_level,failed,graphic_bank,block_on_load);
    }
    return !failed;
}
