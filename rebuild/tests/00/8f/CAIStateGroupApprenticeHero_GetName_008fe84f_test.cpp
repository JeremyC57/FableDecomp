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

class CAIStateGroup_ApprenticeHero
{
public:
    static CCharString __fastcall GetName();
};

int main()
{
    {
        CCharString name = CAIStateGroup_ApprenticeHero::GetName();
        if (g_ctorCount != 1 || name.PStringData != &name) { std::printf("BAD_IDENTITY\n"); return 1; }
        if (g_seenLength != -1) { std::printf("BAD_LEN\n"); return 1; }
        if (g_seenText != (const char*)0x0128AEAC) { std::printf("BAD_TEXT\n"); return 1; }
    }
    std::printf("GETACTIONNAME_008fe84f_OK\n");
    return 0;
}
