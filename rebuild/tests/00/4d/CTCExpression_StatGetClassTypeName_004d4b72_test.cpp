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

class CTCExpression
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

int main()
{
    {
        CCharString name = CTCExpression::StatGetClassTypeName();
        if (g_ctorCount != 1 || name.PStringData != &name) { std::printf("BAD_IDENTITY\n"); return 1; }
        if (g_seenLength != -1) { std::printf("BAD_LEN\n"); return 1; }
        if (g_seenText != (const char*)0x0123C2D0) { std::printf("BAD_TEXT\n"); return 1; }
    }
    std::printf("GETACTIONNAME_004d4b72_OK\n");
    return 0;
}
