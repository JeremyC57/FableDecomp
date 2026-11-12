// CGraphicInfoFrame::CFrameData::OnResourceLoaded() -- once an async read is pending (+0x30): try to upload the frame (0x009FF220, 0); when that fails and no graphic frame data exists (+0x1c), drop the owner's counted data pointer (owner at +0x40, pointer at its +8)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FF860, length 80 bytes.
// Callees: UploadFrame @ 0x009FF220 (on `this`, 0; a bool in al), CIVCountedPointeeBase virtual slot 1 (Delete) when the count reaches zero. Shape: the inlined
// `CIVCountedPointer::operator=(0)` (GOTCHAS wave 24h: the release through a local pointee copy, its zero store inside the null test, then the assignment's
// unconditional zero store); the pointer address `owner+8` formed with `add esi,8`.
// Leaf: ?OnResourceLoaded@CFrameData@CGraphicInfoFrame@@UAEXXZ
// Naming (agentNF31, 2026-11-12): moved off the local top-level CFrameData stand-in (UploadFrame(long)) onto engine/CGraphicInfoFrame.h,
//   whose nested CGraphicInfoFrame::CFrameData declares ?UploadFrame@CFrameData@CGraphicInfoFrame@@QAE_N_N@Z (Ego_r.pdb 0x00B5B6D0 +
//   FableWin.pdb 0x031285A0: the parameter is bool); +0x1c = Frame (CGraphicFrame::FrameData), +0x30 = FileData, +0x40 =
//   GraphicInfoFrame (its Data +8, a mutable CIVCountedPointer<CFrameData>), Ego_r layout.
#define FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICINFO_TEXTURETYPE
#define FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/CGraphicInfo.h"
#include "engine/GFSizeof.h"
#include "engine/CGraphicInfoFrame.h"

void CGraphicInfoFrame::CFrameData::OnResourceLoaded()
{
    if (!FileData.IsNull()) {
        if (!UploadFrame(false)) {
            if (!Frame.FrameData)
                GraphicInfoFrame->Data.Reset(0);
        }
    }
}
