// NUISystem::CSprite::SetGraphicIndex(unsigned long index) -- store the graphic index; for a real graphic take the sprite size from the graphics bank's frame-0 width / height, start loading frame 0 of the graphic and fetch it (forced load)
// Leaf: ?SetGraphicIndex@CSprite@NUISystem@@UAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041B550, 680 bytes, `ret 4`; CSprite vtable slot 145 (engine/NUISystem_CSprite.h).
// Identity: Ego_r.pdb ?SetGraphicIndex@CSprite@NUISystem@@UAEXK@Z (virtual); the catalogue's vfunc145 placeholder.
// Callees: CManager::GetInstance 0x0041E5F2 and its inline GetGraphicsBank (CCountedPointer<CGraphicDataBank> copy of +0x10);
//   the bank's GetGraphicFrameWidth / GetGraphicFrameHeight (virtual +0x54 / +0x58); PeekGraphicInfoFrame (inline, PeekGraphic
//   twice over GraphicList +0x1E0, sizeof(CGraphicInfo) 0x2C, Frames +0x28); CGraphicInfoFrame::BeginLoading 0x009FE620 (const);
//   the inline GetFrame(frame, 0, 1): TouchFrame (CManagedTexture::TouchTexture, CResource::TouchResource -> ListRemove 0x009FBFB0 /
//   ListInsert 0x009FBF90), CFrameData::BeginLoading 0x009FDE80, operator new 0x00BFEA1A + CFrameData ctor 0x009FE380,
//   InternalGetFrameForcedLoading 0x009FF6B0; the local CGraphicFrame's release (CGraphicFrameData::Uninitialise 0x009FCBA0,
//   operator delete 0x00BFE9BC). Size is CComponent::Size (+0xCC / +0xD0).
// Headers: the landed NUISystem::CIconText::AddIcon 0x00542780 graphic-frame macro block (same PeekGraphicInfoFrame / BeginLoading /
//   GetFrame sequence) over engine/NUISystem_CSprite.h.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CGRAPHICFRAME_RELEASEDATA
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_BEGINLOADING
#define FABLE_RETAIL_CGRAPHICINFO_SPRITE
#define FABLE_RETAIL_CGRAPHICINFO_PASS22_SPRITE
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHICINFOFRAME
#define FABLE_RETAIL_CTEXTURE
#include "fable_string.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CGraphicInfo.h"

namespace NUISystem {
void CSprite::SetGraphicIndex(unsigned long index)
{
    GraphicIndex = index;
    if (GraphicIndex > 0) {
        Size.X = (float)CManager::GetInstance()->GetGraphicsBank()->GetGraphicFrameWidth(GraphicIndex, 0);
        Size.Y = (float)CManager::GetInstance()->GetGraphicsBank()->GetGraphicFrameHeight(GraphicIndex, 0);
        const CGraphicInfoFrame* frame = CManager::GetInstance()->GetGraphicsBank()->PeekGraphicInfoFrame(GraphicIndex, 0);
        frame->BeginLoading(0, 0x10);
        CGraphicFrame graphicFrame;
        frame->GetFrame(graphicFrame, 0, 1);
    }
}
}
