// CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive) -- draw a static mesh's shadow mesh into the shadow buffer: skip disabled managers, primitives flagged 0x2000 and primitives without a shadow mesh; with shaders on and the render state captured, clip the shadow mesh's world-space sphere against the shadow caster volume, pick the LOD, fetch the mesh, then for every visible primitive set the colour-buffer depth-pack stage (technique 2), the shadow vertex / pixel shaders and the alpha test (transparent materials also get their diffuse texture at the mipmap they need) and draw its static blocks; finally release the stream, indices and textures
// Leaf: ?RenderPrimitiveToShadowBuffer@CEnginePrimitiveManagerStaticMeshesBase@@UAE?AW4ERendered@@AAVCEngineInternalPrimitiveBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA6240, 2937 bytes, thiscall `ret 4`, ebp frame (sub esp,2BCh), no EH frame.
// Identity: vtable 0x012A50DC slot 8; Ego_r.pdb 0x00A5D4E0 / FableWin.pdb 0x02E9FE30 (same mangled name).
// Retail-asm: 0x00BA6A6F..0x00BA6A89 `fld [ebp-1Ch] ; fsub [ebp-68h] ; fistp dword ptr [ebp-3Ch] ; fild [ebp-3Ch] ; fadd [ebp-44h] ; fstp [ebp-54h] ;
//   mov eax,[ebp-1Ch] ; cmp eax,[ebp-54h] ; jne ; inc dword ptr [ebp-3Ch]` = the shared engine/GFFloatToLongFloor.h helper (the mipmap size
//   floor inside the expanded GetAppropriateMipmap, as in the same-TU row 0x00BA5260); this row carries no asm of its own.
// Globals: NEngineInternal::GPRenderManager 0x01436E18, GPShaderRenderManager 0x01436E14, GPPrimitiveRendererSharedData 0x01436E78,
//   GPPrimitiveRenderer 0x01436E80, GPShadowRenderer 0x01436E60, GPCamera 0x01436EA0 (names: WellingtonGame Xbox PDB globals + the
//   same-TU row 0x00BA5260's source), GTopBitInByteTable 0x0143B990, DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Same TU (EVIDENCED): retail compiled this row with the bodies of the three callees that receive &texture_stages visible above it
//   (SetupFirstTextures 0x00B9D670 landed, RenderMeshPrimitiveBlocksIndexBuffer 0x00B9E5B0 staged acctC_c19a, SetupStaticBlocks
//   0x00BA50A0 + its callee SetupStaticBlocksBlankTextures 0x00BA4A40 landed), so c2 knew the stages object is not captured: the
//   technique / ctor stores schedule around pointer loads (0x00BA66B8..0x00BA66D0 `xor edx,edx ; cmp esi,2 ; mov [ebp-8],edx ; ...
//   mov [ebp-0Ch],dl ; jne ; add ecx,32Ch`) and the UseBlankTextures stores move into the expanded mipmap / alpha-test code
//   (0x00BA696F, 0x00BA6B3A). With declarations only: DIFFER(2937v2937), 45 masked lines. Their text below is the same text as the
//   wfB3 TU file rebuild/candidates/manual/pending/2026-11-14_acctC_wfB3open/tu_00ba2350_00ba5260.cpp (with GetDiffuseTexture /
//   GetBumpTexture / CGraphicInfoFrame::GetTexture as there); each grades RELOCATION_MATCH in this file (q.py at its own address).
// Spellings: technique read before the C3DTextureStages declaration (EVIDENCED: retail drops the ctor's dead Diffuse = -1 store, an
//   address-taken local's store cannot pass a later pointer load); `mipmap = 0` before the stages (FITTED: store order mipmap, Bump,
//   UseBlank with the shared zero register dl); UseBlankTextures = false before the mipmap call (FITTED: store position 0x00BA696F);
//   the depth-pack constant as a C4DVector temporary built from a named const float (FITTED: retail loads MinWClamp first, copies it
//   with integer moves and stores X, Y, Z, W after the pushes; a named C4DVector gives the X store before the load); the
//   used_blank_textures flag in its own { } block round the SetupStaticBlocks call (FITTED: c2 frame packer puts it at ebp-2Dh as
//   retail; at function or loop scope it sorts first, ebp-1).
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS_BLOCKDRAW
#define FABLE_RETAIL_C3DPRIMITIVE2_RECORDEDSTATE
#define FABLE_RETAIL_C3DTEXTURESTAGES_EQUALS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_BLANKTEXTURES
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_STATICBLOCKS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
#define FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
#define FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW
#define FABLE_RETAIL_C3DPRIMITIVE2_ANIMATEDBLOCKS
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_PEEKMESH
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_APPROPRIATELOD
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH_GETRENDERINFO
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_HIDDENMATERIALS
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_RENDERMESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SILHOUETTES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_SHADOWBUFFERSHADERS
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_C3DMESHINFO_BEGINLOADINGTEXTURES
#define FABLE_RETAIL_C3DMESHINFO_LODERROR
#define FABLE_RETAIL_C3DMESH2_PRIMITIVES
#define FABLE_RETAIL_C3DPRIMITIVE2_RENDER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DTEXTURESTAGES_CTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CGRAPHICINFO_CONSTRUCT
#define FABLE_RETAIL_CGRAPHICINFO_GETWIDTH
#define FABLE_RETAIL_C3DPRIMITIVE2_GETVERTEXBUFFER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_SHADOWBUFFERSHADERS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKMINWCLAMP
#define FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY
#define FABLE_RETAIL_CSHADOWCLIPVOLUME_CLIPSPHERE
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CSCOPEDSHADERSENABLE_FORCEINLINE
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#include "fable_compiler.h"
#include "engine/EPrimitiveType.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CMatrix3x4.h"
#include "engine/C4DVector.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CEngineInternalPrimitiveMorphedStaticMesh.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CShadowClipVolume.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/C3DPrimitive2.h"
#include "engine/C3DTextureStages.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CMeshDataBank.h"
#include "engine/CCountedPointer.h"
#include "engine/CScopedShadersEnable.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CVertexBufferWin32.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CTexture.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFLimit.h"
#include "engine/CEnginePrimitiveRenderer.h"

namespace NEngineInternal {
extern CRenderManager* GPRenderManager;                                  // 0x01436E18
extern CShaderRenderManager* GPShaderRenderManager;                      // 0x01436E14
extern CEnginePrimitiveRendererSharedData* GPPrimitiveRendererSharedData; // 0x01436E78
extern CEnginePrimitiveRenderer* GPPrimitiveRenderer;                     // 0x01436E80
extern CEngineShadowRenderer* GPShadowRenderer;                           // 0x01436E60
extern CEngineCamera* GPCamera;                                           // 0x01436EA0
}
extern unsigned char GTopBitInByteTable[256];                              // 0x0143B990
extern const float DELTA_NOTIONAL_ZERO;                                    // 0x0129BA3C

inline unsigned long FABLE_FASTCALL GFGetTopBitInByte(unsigned char value)
{
    return GTopBitInByteTable[value];
}

inline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera() { return *NEngineInternal::GPCamera; }
FABLE_FORCEINLINE float __fastcall CEnginePrimitiveRenderUtil::CalcDistanceFromCamera(const C3DVector& position)
{
    return (position - GetCurrentCamera().PeekCameraPos()).GetMagnitude();
}
inline float __fastcall CEnginePrimitiveRenderUtil::GetLODErrorTolerance() { return NEngineInternal::GPPrimitiveRendererSharedData->LODErrorTolerance; }
inline float __fastcall CEnginePrimitiveRenderUtil::GetLODVirtualScreenSizeFOVAdjusted() { return NEngineInternal::GPPrimitiveRendererSharedData->LODVirtualScreenSizeFOVAdjusted; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderShadowBufferRender() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderShadowBufferRender; }
inline const CPixelShader& __fastcall CEnginePrimitiveRenderUtil::GetPixelShaderShadowBufferRenderAlpha() { return *NEngineInternal::GPPrimitiveRendererSharedData->PixelShaderShadowBufferRenderAlpha; }
inline float __fastcall CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() { return NEngineInternal::GPPrimitiveRendererSharedData->MipmapSizeMultiplier; }

FABLE_FORCEINLINE unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateLOD(const C3DMeshInfo* mesh_info, float distance, float radius, float tolerance)
{
    float nearest = distance - radius > DELTA_NOTIONAL_ZERO ? distance - radius : DELTA_NOTIONAL_ZERO;
    float lod_error = nearest / (radius * CEnginePrimitiveRenderUtil::GetLODVirtualScreenSizeFOVAdjusted()) * tolerance;
    unsigned long lod = 0;
    while (mesh_info->HasLOD(lod + 1) && !(mesh_info->GetLODError(lod + 1) > lod_error))
        ++lod;
    return lod;
}

FABLE_FORCEINLINE unsigned long FABLE_FASTCALL CEnginePrimitiveManagerMeshBase::GetAppropriateMipmap(const C3DVector& camera_position,
    const CMatrix3x4& transform, const C3DPrimitive2& mesh_primitive, float scale, const C3DMeshMaterial* material,
    const CGraphicDataBank* graphic_bank)
{
    C3DBoundingSphere sphere = mesh_primitive.GetBoundingSphere();
    transform.TransformPoint(sphere.Center);
    unsigned long mipmap = 0;
    float radius = sphere.GetRadius() * scale;
    float distance = (camera_position - sphere.GetCenter()).GetMagnitude();
    distance = (distance - radius) * mesh_primitive.GetAverageTextureStretch();
    if (distance > DELTA_NOTIONAL_ZERO) {
        long graphic_index = material->GetDiffuseMapBankIndex();
        if (graphic_index > 0) {
            const CGraphicInfo* graphic = graphic_bank->PeekGraphic(graphic_index);
            float ratio = (float)graphic->GetWidth() / (CEnginePrimitiveRenderUtil::GetMipmapSizeMultiplier() / distance * scale);
            long size = GFFloatToLongFloor(ratio);
            GFLimit(size, 1L, 255L);
            mipmap = GFGetTopBitInByte((unsigned char)size);
        }
    }
    return mipmap;
}

FABLE_FORCEINLINE const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}
FABLE_FORCEINLINE void CShaderRenderManager::SelectVertexBuffer(CVertexBufferWin32& buffer)
{
    D3DDevice->SetStreamSource(0, buffer.PVertexBuffer, 0, buffer.Stride);
}
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

void FABLE_FASTCALL CEnginePrimitiveManagerStaticMeshesBase::SetupFirstTextures(C3DPrimitive2& primitive, C3DTextureStages& textureStages)
{
    N3DPrimitive::CStaticBlock* block = primitive.StaticBlocks;
    if (textureStages.DiffuseTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.DiffuseTextureStage, block->DiffuseTexture);
    if (textureStages.BumpTextureStage != -1)
        NEngineInternal::GPRenderManager->AttachTextureToStage(textureStages.BumpTextureStage, block->BumpTexture);
}
void FABLE_FASTCALL CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer(C3DPrimitive2& mesh_primitive, const C3DTextureStages& texture_stages)
{
    unsigned long no_blocks = mesh_primitive.GetStaticBlockCount();
    for (unsigned long c_block = 0; c_block < no_blocks; ++c_block) {
        N3DPrimitive::CStaticBlock& block_info = *mesh_primitive.GetStaticBlock(c_block);
        if (!block_info.GetDegenerateTriangles()) {
            if (block_info.GetChangeFlags() & 1)
                NEngineInternal::GPRenderManager->AttachTextureToStage(texture_stages.DiffuseTextureStage, block_info.DiffuseTexture);
            if ((block_info.GetChangeFlags() & 2) && texture_stages.BumpTextureStage != -1)
                NEngineInternal::GPRenderManager->AttachTextureToStage(texture_stages.BumpTextureStage, block_info.BumpTexture);
            NEngineInternal::GPShaderRenderManager->DrawBuffersIndexed(0, mesh_primitive.GetVertexCount(), *mesh_primitive.IndexBuffer,
                block_info.GetStartIndex(), block_info.GetPrimitiveCount(), block_info.GetPrimitiveType());
        }
    }
}

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

ERendered CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& base_primitive)
{
    if (!GetEnable())
        return RENDERED_NOTHING;
    CEngineInternalPrimitiveStaticMesh& primitive = static_cast<CEngineInternalPrimitiveStaticMesh&>(base_primitive);
    if (primitive.EnginePrimitiveFlags & 0x2000)
        return RENDERED_NOTHING;
    if (primitive.ShadowMeshInfo == 0)
        return RENDERED_NOTHING;

    CScopedShadersEnable shaders_enable(true, true);
    CRenderStateScopedCaptureBlock capture_block;

    float radius = primitive.ShadowMeshInfo->GetBoundingSphere().GetRadius() * primitive.Scale;
    C3DVector centre = primitive.ShadowMeshInfo->GetBoundingSphere().Center;
    primitive.ObjectTransform.TransformPoint(centre);
    if (!NEngineInternal::GPShadowRenderer->PeekSceneData().CasterClipVolume.ClipSphere(centre, radius))
        return RENDERED_NOTHING;

    float distance = CalcDistanceFromCamera(centre);
    unsigned long lod = GetAppropriateLOD(primitive.ShadowMeshInfo, distance, radius, GetLODErrorTolerance());
    CCountedPointer<const C3DMesh2> mesh;
    if (!primitive.ShadowMeshInfo->GetMesh(mesh, lod, 0))
        return RENDERED_NOTHING;
    CEngineRenderInfoStaticMesh* render_info = primitive.GetRenderInfo();
    render_info->Mesh = mesh.Get();
    render_info->LOD = lod;

    const unsigned char* hidden_materials = 0;
    if (primitive.GetType() == 10)
        hidden_materials = static_cast<CEngineInternalPrimitiveMorphedStaticMesh&>(primitive).Morph.GetHiddenMaterials(*mesh.Get());

    const CGraphicDataBank* texture_bank = primitive.ShadowMeshInfo->PeekTextureBank();
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(primitive.ObjectTransform);
    NEngineInternal::GPRenderManager->SetAlphaRef(128);
    NEngineInternal::GPRenderManager->SetAlphaTestFunc(ALPHA_TEST_GREATER_EQUAL);

    for (unsigned long i = 0; i < mesh->GetPrimitiveCount(); ++i) {
        C3DPrimitive2& mesh_primitive = *mesh->GetPrimitive(i);
        if (mesh_primitive.TriangleCount == 0)
            continue;
        const C3DMeshMaterial* material = mesh->GetMaterial(mesh_primitive.GetMaterialIndex());
        if (hidden_materials && hidden_materials[mesh_primitive.GetMaterialIndex()])
            continue;
        CEngineShadowRenderer::EShadowRenderingTechnique technique = NEngineInternal::GPShadowRenderer->PeekRenderingTechnique();
        unsigned long mipmap = 0;
        C3DTextureStages texture_stages;
        if (technique == CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER) {
            texture_stages.DiffuseTextureStage = 1;
            NEngineInternal::GPRenderManager->AttachTextureToStage(0, &NEngineInternal::GPShadowRenderer->GetDepthPackingTexture());
            NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_POINT);
            NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_POINT);
            NEngineInternal::GPRenderManager->SetTextureUVModeU(0, TEXTURE_UV_WRAP);
            NEngineInternal::GPRenderManager->SetTextureUVModeV(0, TEXTURE_UV_CLAMP);
            const float min_w_clamp = NEngineInternal::GPShadowRenderer->PeekColourBufferShadowMinWClamp();
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, C4DVector(128.0f, min_w_clamp, 0.0f, 0.0f));
        } else {
            texture_stages.DiffuseTextureStage = 0;
        }
        NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*VertexShaderShadowBufferRender, true);
        if (material->Transparent) {
            if (material->GetDiffuseMapBankIndex() == 0)
                continue;
            texture_stages.UseBlankTextures = false;
            mipmap = GetAppropriateMipmap(GetCurrentCamera().PeekCameraPos(), primitive.ObjectTransform, mesh_primitive, primitive.Scale, material, texture_bank);
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(true);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(GetPixelShaderShadowBufferRenderAlpha());
        } else {
            texture_stages.UseBlankTextures = true;
            NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
            NEngineInternal::GPShaderRenderManager->ApplyPixelShader(GetPixelShaderShadowBufferRender());
        }
        NEngineInternal::GPRenderManager->RealiseRenderState();
        NEngineInternal::GPShaderRenderManager->PrepareForRender(1);
        NEngineInternal::GPShaderRenderManager->SelectVertexBuffer(*mesh_primitive.GetVertexBuffer());
        {
            bool used_blank_textures;
            if (SetupStaticBlocks(mesh_primitive, primitive, texture_stages, material, 0, mipmap, &used_blank_textures)) {
                SetupFirstTextures(mesh_primitive, texture_stages);
                RenderMeshPrimitiveBlocksIndexBuffer(mesh_primitive, texture_stages);
            }
        }
    }
    NEngineInternal::GPRenderManager->FlushRender();
    return RENDERED_OBJECT;
}