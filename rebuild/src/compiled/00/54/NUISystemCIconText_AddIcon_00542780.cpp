// NUISystem::CIconText::AddIcon(long graphic, long charIndex, EIconFlags flags) -- build an embedded icon; a set graphic takes its frame size and starts loading/fetching its frame (else the size is zero), then append it to Icons
// Leaf: ?AddIcon@CIconText@NUISystem@@UAEXJJW4EIconFlags@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00542780, 682 bytes, `ret 0xc`, thiscall, virtual (Ego_r.pdb
//   ?AddIcon@CIconText@NUISystem@@UAEXJJW4EIconFlags@2@@Z; EIconFlags is NUISystem::EIconFlags).
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 (GetGraphicsBank inline), CGraphicInfoFrame::BeginLoading 0x009FE620, CResource::ListRemove 0x009FBFB0 /
//   ListInsert 0x009FBF90, CFrameData::BeginLoading 0x009FDE80, CFrameData::CFrameData 0x009FE380, InternalGetFrameForcedLoading
//   0x009FF6B0, CGraphicFrameData::Uninitialise 0x009FCBA0, operator new 0x00BFEA1A, operator delete 0x00BFE9BC,
//   vector<CEmbeddedIcon>::push_back 0x00543C90.
// Lever: retail's TU holds the out-of-line STLport push_back (0x00543C90) and its _M_insert_overflow; with both bodies visible
//   (auto_inline off) c2 knows the icon's address is not kept, which fixes the FrameWidth/FrameHeight load pair (edx, eax) before
//   the fild. push_back alone was not enough; the whole chain must be visible.
// Headers: all shared -- engine/NUISystem_CIconText.h (retail layout, Icons +0x18c), stlport/fable_containers.h vector with the
//   visible out-of-line push_back (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE), NUISystem::CManager::GetGraphicsBank, and the
//   graphics-bank stack of the landed CEngineSkyRenderer rows: CGraphicDataBank::PeekGraphicInfoFrame (inline, FableWin.pdb
//   ?PeekGraphicInfoFrame@CGraphicDataBank@@QBEPBVCGraphicInfoFrame@@KK@Z; new opt-in), CGraphicInfoFrame::GetWidth / GetHeight
//   (the frame size words +6 / +8 of the graphic header, `movzx` then `fild dword`), BeginLoading 0x009FE620 and the inline
//   CGraphicInfoFrame::GetFrame (?GetFrame@CGraphicInfoFrame@@QBE_NAAVCGraphicFrame@@KK@Z) with its TouchFrame / TouchTexture /
//   TouchResource and CGraphicFrame release bodies.
//   The (graphic, charIndex, flags) CEmbeddedIcon constructor is an inline opt-in (FABLE_RETAIL_CICONTEXT_EMBEDDEDICON_ARGS_CTOR,
//   descriptive: the PDBs only show the out-of-line default and copy constructors; the default-ctor + assignment form gives a
//   different prologue).
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CICONTEXT_VSLOTS
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CICONTEXT_EMBEDDEDICON_ARGS_CTOR
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
// The graphics-bank helper set of the landed CEngineSkyRenderer rows (0x00B64550 RenderSunFlare): one body per helper.
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
#include "engine/NUISystem_CIconText.h"
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CGraphicInfo.h"

namespace NUISystem {
void CIconText::AddIcon(long graphic, long charIndex, EIconFlags flags)
{
    CEmbeddedIcon icon(graphic, charIndex, flags);
    CCountedPointer<CGraphicDataBank> bank = CManager::GetInstance()->GetGraphicsBank();
    if (graphic > 0) {
        const CGraphicInfoFrame* frame = bank->PeekGraphicInfoFrame(graphic, 0);
        icon.Size.X = (float)frame->GetWidth();
        icon.Size.Y = (float)frame->GetHeight();
        frame->BeginLoading(0, 0x10);
        CGraphicFrame graphicFrame;
        frame->GetFrame(graphicFrame, 0, 0x11);
    } else {
        icon.Size.X = 0.0f;
        icon.Size.Y = 0.0f;
    }
    Icons.push_back(icon);
}
}