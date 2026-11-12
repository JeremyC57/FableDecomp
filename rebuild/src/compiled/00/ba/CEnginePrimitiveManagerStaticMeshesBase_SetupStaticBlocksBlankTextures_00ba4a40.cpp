// CEnginePrimitiveManagerStaticMeshesBase::SetupStaticBlocksBlankTextures(C3DPrimitive2& mesh_prim, CEngineInternalPrimitiveStaticMesh& prim, C3DTextureStages& stages, const C3DMeshMaterial* material, bool* already_set_up) -- give every static block of a mesh primitive the texture bank's BLANK textures (used while the real ones are not wanted): if the primitive already recorded this texture-bank frame and these stages, report *already_set_up = true; otherwise pick block 0's blank diffuse (stage active; type of the material's diffuse graphic, opaque when it has none) and blank bump (type of its bump graphic, TEXTURE_TYPE_BUMP when none), copy both to every later block, record the frame and the stages, and return true
// Leaf: ?SetupStaticBlocksBlankTextures@CEnginePrimitiveManagerStaticMeshesBase@@AAE_NAAVC3DPrimitive2@@AAVCEngineInternalPrimitiveStaticMesh@@AAVC3DTextureStages@@PBVC3DMeshMaterial@@PA_N@Z
// Inline-budget: FABLE_FORCEINLINE on CGraphicDataBank::PeekGraphicInfoFrame -- retail expands it at both uses (0x00BA4AB6 / 0x00BA4B48); plain `inline` leaves two calls (267 bytes) once it is used twice.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BA4A40, 463 bytes (+1 int3 pad), private `__fastcall` whose `this` is never read
//   (ecx untouched), five stack arguments, `ret 14h`. Only caller: SetupStaticBlocks 0x00BA50A0.
// Name: Ego_r.pdb (0x00A5A8A0) and FableWin.pdb, same mangled name as the Leaf line.
// Callees: CGraphicDataBank::GetBlankTexture(ETextureType) const 0x009FD150 (the only call). Everything else is inline in retail:
//   C3DMeshInfo::PeekTextureBank (mesh info +4 -> bank +0x3D4), CResourceBank::GetCurrentFrame (+0x194 of the graphic bank),
//   C3DPrimitive2::GetRecordedFrame / PeekRecordedTextureStages / SetRecordedFrame / SetRecordedTextureStages (+0x7C / +0x6C),
//   C3DTextureStages::operator==, C3DMeshMaterial::GetDiffuseMapBankIndex / GetBumpMapBankIndex (+0x10 / +0x14),
//   CGraphicDataBank::PeekGraphicInfoFrame -> PeekGraphic twice (the branchy bounds test, then the cmp/sbb/and select),
//   CGraphicInfoFrame::GetTextureType -> CGraphicInfo::GetTextureType (+0xC, then +0x1C).
// Headers (overlay opt-ins, hdr/README.md): FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_BLANKTEXTURES (the private
//   declaration), FABLE_RETAIL_C3DTEXTURESTAGES_EQUALS, FABLE_RETAIL_C3DPRIMITIVE2_RECORDEDSTATE, FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE.
// Shape: the frame is homed in the dead `prim` argument slot ([esp+10h]) and the first block in the dead `already_set_up` slot
//   ([esp+24h]); `texture_type` is declared (with its default) BEFORE the material's bank index is read -- that order puts the
//   material in ecx / edx and the type in eax (`mov ecx,[esp+20h] ; mov edi,[ecx+10h] ; xor eax,eax`, 0x00BA4AA9). A graphic index
//   past the list still dereferences the null frame (0x00BA4B17 `xor eax,eax ; mov ecx,[eax+0Ch]`), as the original does. The
//   block loop reloads StaticBlocks each iteration (GetStaticBlock) and reads the first block's textures through the reference.
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
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
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_STATICBLOCKS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_BLANKTEXTURES
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DTEXTURESTAGES_EQUALS
#define FABLE_RETAIL_C3DPRIMITIVE2_RECORDEDSTATE
#define FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
#include "fable_compiler.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEngineInternalPrimitiveStaticMesh.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CEnginePrimitiveManagerStaticMeshesBase.h"
#define FABLE_RETAIL_CCHARSTRING
#include "engine/C3DPrimitive2.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/CGraphicInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"

// Bodies: PeekGraphic / PeekGraphicInfoFrame copied from landed row 0x00BA48F0 (SetupStaticBlocksNoMorph, rebuild/src/compiled/00/ba/
//   CEnginePrimitiveManagerStaticMeshesBase_SetupStaticBlocksNoMorph_00ba48f0.cpp, plain `inline` there); retail expands both at 0x00BA4AB6 / 0x00BA4B48.
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index<GraphicList.size() ? &GraphicList[index] : 0;
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