// CManagedTexture::operator=(const CManagedTexture&) -- when the two hold different texture data: Release() (0x00A69AD0) then the counted Data assigned (old released, new add-ref'd); returns *this
// Leaf: ??4CManagedTexture@@QAEABV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A69B20.
// Naming (agentNF31, 2026-11-12): was CIVCountedHandle_00a69b20::operator= over local CIVCountedPointer / CManagedTexture models.
//   Ego_r.pdb 0x00B7D230 + FableWin.pdb 0x017AC0C9 ??4CManagedTexture@@QAEABV0@ABV0@@Z (const reference return); the landed
//   caller UploadFrame 0x009FF220 binds this symbol (ecx = &ManagedTexture, arg = &LoadingManagedTexture at 0x009FF292) through
//   engine/CManagedTexture.h FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME, which also declares Release 0x00A69AD0.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CFRAMEDATA_UPLOADFRAME
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CManagedTexture.h"

const CManagedTexture& CManagedTexture::operator=(const CManagedTexture& other)
{
    if (Data.InternalGetObject() != other.Data.InternalGetObject()) {
        Release();
        Data = other.Data;
    }
    return *this;
}
