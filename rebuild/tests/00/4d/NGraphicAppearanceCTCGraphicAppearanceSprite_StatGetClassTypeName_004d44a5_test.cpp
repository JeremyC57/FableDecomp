#include <cstdio>

class CCharString
{
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);       // 0x0099EBF0
    ~CCharString();
};

static const char* g_seenText = 0;
static long g_seenLength = 0;
static int g_ctorCount = 0;
CCharString::CCharString(const char* text, long length) { PStringData = this; g_seenText = text; g_seenLength = length; ++g_ctorCount; }
CCharString::~CCharString() {}

namespace NGraphicAppearance {
class CTCGraphicAppearanceSprite
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};
}

int main()
{
    {
        CCharString name = NGraphicAppearance::CTCGraphicAppearanceSprite::StatGetClassTypeName();
        if (g_ctorCount != 1 || name.PStringData != &name) { std::printf("BAD_IDENTITY\n"); return 1; }
        if (g_seenLength != -1) { std::printf("BAD_LEN\n"); return 1; }
        if (g_seenText != (const char*)0x0123BCA0) { std::printf("BAD_TEXT\n"); return 1; }
    }
    std::printf("GETACTIONNAME_004d44a5_OK\n");
    return 0;
}
