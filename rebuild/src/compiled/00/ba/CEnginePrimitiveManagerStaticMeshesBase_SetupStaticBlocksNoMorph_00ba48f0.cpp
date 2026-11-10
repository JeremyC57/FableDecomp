// CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(C3DPrimitive2& primitive, const C3DMesh2& mesh, C3DTextureStages& stages, long level, const CGraphicDataBank* bank, bool load) -- resolve the diffuse / bump textures of a static mesh primitive's blocks: block 0 always (per active texture stage), later blocks only where their change flags say the texture changes (a changed diffuse with no diffuse stage marks failure); then invalidate the recorded frame and return true when nothing failed
// Flags:
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_C3DPRIMITIVE2_HIERARCHY
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
// Leaf: ?SetupStaticBlocksNoMorph@CEnginePrimitiveManagerStaticMeshesBase@@SI_NAAVC3DPrimitive2@@ABVC3DMesh2@@AAVC3DTextureStages@@JPBVCGraphicDataBank@@_N@Z
// Inline-budget: FABLE_FORCEINLINE on original CGraphicInfoFrame::GetTexture -- retail 613 / 597-byte helpers expand it;052 outlined it and inlined the smaller texture wrappers into Setup (608 bytes).
// Retail: 0x00ba48f0, 324 bytes, static fast call.
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_STATICBLOCKS
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#define FABLE_RETAIL_CCHARSTRING
#include "engine/C3DPrimitive2.h"
#include "engine/C3DMesh2.h"

#include "engine/CGraphicInfo.h"
#include "engine/CEnginePrimitiveRenderer.h"

// Complete behavior reconstructed from 0x00BA9360..00ba95c5 and 0x00BA95D0..00ba9825.
// Supporting inline phases retain the native owning types and all load paths.
extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer;

inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index<GraphicList.size() ? &GraphicList[index] : 0;
}
inline const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long graphic,unsigned long frame) const
{
    return PeekGraphic(graphic) ? PeekGraphic(graphic)->PeekInfoFrame(frame) : 0;
}
inline void CManagedTexture::TouchTexture()
{
    if(!Data.IsNull() && Data->ResourceList && Data->LastUsedFrame!=Data->ResourceList->GetCurrentFrame()) {
        Data->SetAsMRU();
        Data->LastUsedFrame=Data->ResourceList->GetCurrentFrame();
    }
}
inline void CGraphicInfoFrame::TouchFrame(unsigned long mipmap_level) const
{
    const unsigned long frame=GraphicInfo->GetBank()->GetCurrentFrame();
    if(Data->TouchedFrame!=frame || mipmap_level<=Data->TouchedMipmapLevel) {
        Data->TouchedFrame=frame;
        Data->ManagedTexture.TouchTexture();
        if(mipmap_level<=Data->CurrentMipmapLevel && Data->ResourceList && Data->LastUsedFrame!=Data->ResourceList->GetCurrentFrame()) {
            Data->SetAsMRU();
            Data->LastUsedFrame=Data->ResourceList->GetCurrentFrame();
        }
        if(Data->TouchedMipmapLevel>mipmap_level)
            Data->TouchedMipmapLevel=(signed char)mipmap_level;
    }
}
FABLE_FORCEINLINE bool CGraphicInfoFrame::GetTexture(const CTexture** texture,unsigned long mipmap_level,unsigned long flags) const
{
    if(!Data.IsNull() && Data->Frame.FrameData) {
        if(!(flags&8)) TouchFrame(mipmap_level);
        if(mipmap_level<Data->CurrentMipmapLevel) {
            Data->BeginLoading(mipmap_level,flags);
            if((flags&16) && Data->FileData.Data) goto load_frame;
        }
        *texture=Data->Frame.FrameData->GetTexture();
        return true;
    }
load_frame:
    if(Data.IsNull()) {
        if(flags&4) return false;
        Data.Reset(new CFrameData(this));
    }
    Data->BeginLoading(mipmap_level,flags);
    if((flags&1) && InternalGetFrameForcedLoading(flags)) {
        *texture=Data->Frame.FrameData->GetTexture();
        return true;
    }
    return false;
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

bool FABLE_FASTCALL CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksNoMorph(C3DPrimitive2& mesh_prim, const C3DMesh2& mesh,
    C3DTextureStages& stages, long mipmap_level, const CGraphicDataBank* graphic_bank, bool block_on_load)
{
    N3DPrimitive::CStaticBlock& first_block=*mesh_prim.GetStaticBlock(0);
    const C3DMeshMaterial* material=mesh.GetMaterial(first_block.GetMaterialIndex());
    bool failed=false;
    if(stages.DiffuseTextureStage!=-1)
        first_block.DiffuseTexture=CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(material,0,mipmap_level,failed,graphic_bank,block_on_load);
    if(stages.BumpTextureStage!=-1)
        first_block.BumpTexture=CEnginePrimitiveManagerMeshBase::GetBumpTexture(material,0,mipmap_level,failed,graphic_bank,block_on_load);
    unsigned long no_blocks=mesh_prim.GetStaticBlockCount();
    for(unsigned long c_block=1;c_block<no_blocks;++c_block) {
        N3DPrimitive::CStaticBlock& block_info=*mesh_prim.GetStaticBlock(c_block);
        const C3DMeshMaterial* current_material=mesh.GetMaterial(block_info.GetMaterialIndex());
        if(block_info.GetChangeFlags()&1) {
            if(stages.DiffuseTextureStage!=-1)
                block_info.DiffuseTexture=CEnginePrimitiveManagerMeshBase::GetDiffuseTexture(current_material,0,mipmap_level,failed,graphic_bank,block_on_load);
            else failed=true;
        }
        if((block_info.GetChangeFlags()&2)&&stages.BumpTextureStage!=-1)
            block_info.BumpTexture=CEnginePrimitiveManagerMeshBase::GetBumpTexture(current_material,0,mipmap_level,failed,graphic_bank,block_on_load);
    }
    mesh_prim.SetRecordedFrame(-1);
    return !failed;
}