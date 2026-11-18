// CEnginePrimitiveManagerMeshBase::GetReflectionTexture(const C3DMeshMaterial* material, unsigned long mipmap_level, bool& failed, const CGraphicDataBank* graphic_bank, const bool block_on_load) -- the texture of a material's reflection map frame at the wanted mipmap: the bank's blank texture when the material has no reflection map, else the frame's GetTexture (touch, begin loading, forced load when the renderer blocks on loads or block_on_load is set), or the frame's blank texture while it is still loading
// Leaf: ?GetReflectionTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@PBVC3DMeshMaterial@@KAA_NPBVCGraphicDataBank@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA9830, 570 bytes, static __fastcall (material ecx, mipmap_level edx; failed, graphic_bank,
//   block_on_load on the stack; `ret 0Ch`); `failed` is not used. Speed TU (no pragma).
// Identity: Ego_r.pdb / FableWin.pdb / Ego_d.pdb ?GetReflectionTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@PBVC3DMeshMaterial@@KAA_NPBVCGraphicDataBank@@_N@Z
//   (Ego_r donor order GetDiffuseTexture / GetBumpTexture / GetReflectionTexture / Get2DAlphaMaskTexture = retail 0x00BA9360 /
//   0x00BA95D0 / 0x00BA9830 / 0x00BA9A70). Ego_d (the mesh-base object, after GetBumpTexture): parameters material, mipmap_level,
//   failed, graphic_bank, block_on_load; locals texture, frame_info, graphic_index, get_flags -- used as named here.
// Callees: CGraphicDataBank::GetBlankTexture(ETextureType) 0x009FD150 (TEXTURE_TYPE_OPAQUE), and inside the inline
//   ?GetTexture@CGraphicInfoFrame@@QBE_NPAPBVCTexture@@KK@Z (FableWin.pdb; Ego_d `GetTexture(dest, mipmap_level, get_flags)`):
//   CResource::ListRemove 0x009FBFB0 / ListInsert 0x009FBF90, CFrameData::BeginLoading 0x009FDE80, CFrameData::CFrameData
//   0x009FE380 (operator new 0x00BFEA1A, 0x44 bytes), CGraphicInfoFrame::InternalGetFrameForcedLoading 0x009FF6B0;
//   CGraphicInfoFrame::GetBlankTexture 0x009FD3F0; NEngineInternal::GPPrimitiveRenderer 0x01436E80 (inline GetBlockOnLoad, +4).
// Shape: shared headers with the existing opt-ins of the landed CEnginePrimitiveManagerRepeatedStaticMeshes::RenderSubPrimitive
//   0x00B9AC30 (GetTexture inline = the GetFrame sibling text; inline PeekGraphicInfoFrame = retail's twice-evaluated `index <
//   size ? &GraphicList[index] : 0` diamond). `if (frame_info->GetTexture(&texture, ...)) return texture; return
//   frame_info->GetBlankTexture();` sinks retail's `push ebx` (0x00BA98B6) past the diamond; matched at the first grade.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETTEXTURE_INLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CASTTOBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_ANIMATEDMESH
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINECOMPONENT
#include "fable_compiler.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/C3DMeshMaterial.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicInfoFrame.h"
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; }   // 0x01436E80

const CTexture* CEnginePrimitiveManagerMeshBase::GetReflectionTexture(const C3DMeshMaterial* material, unsigned long mipmap_level, bool& failed, const CGraphicDataBank* graphic_bank, const bool block_on_load)
{
    const CTexture* texture;
    long graphic_index = material->GetReflectionMapBankIndex();
    if (graphic_index == 0)
        return graphic_bank->GetBlankTexture(CGraphicDataBank::TEXTURE_TYPE_OPAQUE);
    const CGraphicInfoFrame* frame_info = graphic_bank->PeekGraphicInfoFrame(graphic_index, 0);
    unsigned long get_flags = 0;
    if (NEngineInternal::GPPrimitiveRenderer->GetBlockOnLoad() || block_on_load)
        get_flags = 1;
    if (frame_info->GetTexture(&texture, mipmap_level, get_flags))
        return texture;
    return frame_info->GetBlankTexture();
}