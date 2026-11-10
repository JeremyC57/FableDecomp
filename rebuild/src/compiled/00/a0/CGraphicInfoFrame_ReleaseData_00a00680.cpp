// CGraphicInfoFrame::ReleaseData() const -- drop the frame's shared data block: release the counted pointer Data (+0x08) and clear it
// Leaf: ?ReleaseData@CGraphicInfoFrame@@ABEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A00680, 36 bytes: the release block of CIVCountedPointer::Reset(0) (count decremented, slot-1
//   Delete at zero, `mov [esi+8],0` on the release path) followed by Reset's unconditional store of the null pointer.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?ReleaseData@CGraphicInfoFrame@@ABEXXZ (private const; Data is `mutable`); the landed
//   caller CGraphicInfoFrame::CFrameData::FreeTextureMemory 0x009FE460 binds this spelling at 0x00A00680 (`GraphicInfoFrame->ReleaseData()`).
//   Layout engine/CGraphicInfoFrame.h (retail): CBaseClass vptr +0, FrameNum +4, SmallMipmapLevel +6, Data +8.
// Name fix (agentNF5 2026-11-10): was `CPrimitiveBlock_00a00680::DiscardDeviceIndependentResources` over a local CIVCountedPointer model
//   (the manifest label was propagated from the +0x2C body 0x00A8EE90); now the shared headers.
#define FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/CGraphicInfoFrame.h"
void CGraphicInfoFrame::ReleaseData() const
{
    Data.Reset(0);
}
