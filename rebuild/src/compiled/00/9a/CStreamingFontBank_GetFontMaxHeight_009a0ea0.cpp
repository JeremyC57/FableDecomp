#include "engine/CStreamingFontBank.h"

float __fastcall GetFontMaxHeight(CStreamingFontBank* bank)
{
    return (float)(unsigned long)bank->m_maxHeight;
}
