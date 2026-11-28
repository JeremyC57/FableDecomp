// Flags: 
// Leaf: ?SetTexturePointersValidForFrames@CGraphicDataBank@@QAEXK@Z
class CDataInputStream;
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#include "engine/CGraphicDataBank.h"

void CGraphicDataBank::SetTexturePointersValidForFrames(unsigned long frames)
{
    TexturePointersValidForFrames = frames;
}
