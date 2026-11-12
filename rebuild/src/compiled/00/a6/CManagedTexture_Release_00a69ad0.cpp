// CManagedTexture::Release() -- true when empty; otherwise, when the texture data is shared by exactly two holders (count == 2), release its texture (0x00A69A80) and report true; then release the counted Data
// Leaf: ?Release@CManagedTexture@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69AD0, length 73 bytes (+ int3 padding). Callers: CFrameData::StopLoading/BeginLoading
// (0x009FDC20, 0x009FDE80), ~CFrameData (0x00B2B910) and others (11 sites).
// Callees: CManagedTextureData::ReleaseTexture @ 0x00A69A80 (landed), the counted object's virtual slot 1 (Delete).
// Shape: early `mov al,1 ; ret` for the empty handle, the result flag in bl, the CIVCountedPointer release with its double clear;
// the inline GetRefCount() re-reads the member pointer (`mov eax,[esi+4]`).
// Naming (agentNF31, 2026-11-12): was the stand-in CFrameDataHandle::Clear() over local CFrameData / CIVCountedPointer models.
//   Ego_r.pdb 0x00B7D1E0 + FableWin.pdb 0x017B23E3 ?Release@CManagedTexture@@QAE_NXZ (Ego_r order ReleaseTexture 0x00B7D190 /
//   Release 0x00B7D1E0 / operator= 0x00B7D230 = retail 0x00A69A80 / 0x00A69AD0 / 0x00A69B20); the landed StopLoading 0x009FDC20 and
//   UploadFrame 0x009FF220 bind it through engine/CManagedTexture.h. Callee 0x00A69A80 now binds the landed row's own
//   ?ReleaseTexture@CManagedTextureData@@QAEXXZ (header overlay opt-in) in place of ?Close@CFrameData@@QAEXXZ.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
#define FABLE_RETAIL_CMANAGEDTEXTUREDATA_RELEASETEXTURE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CManagedTexture.h"

bool CManagedTexture::Release()
{
    if (Data.IsNull())
        return true;
    bool released = false;
    if (Data.GetRefCount() == 2) {
        Data->ReleaseTexture();
        released = true;
    }
    Data.Reset(0);   // engine/CIVCountedPointer.h: the release, then the null store (retail double clear)
    return released;
}
