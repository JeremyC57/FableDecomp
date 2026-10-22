#include "engine/CStaticFontBank.h"

float __fastcall GetFontMaxHeight(CStaticFontBank* bank)
{
    return (float)(unsigned long)bank->m_maxHeight;
}
