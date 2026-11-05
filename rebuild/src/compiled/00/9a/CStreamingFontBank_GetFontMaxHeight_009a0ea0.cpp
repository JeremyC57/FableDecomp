// Leaf: ?CStreamingFontBank_GetFontMaxHeight@@YIMPAUCStreamingFontBank@@@Z
#include "engine/CStreamingFontBank.h"

float __fastcall CStreamingFontBank_GetFontMaxHeight(CStreamingFontBank* bank)
{
    return (float)(unsigned long)bank->m_maxHeight;
}
