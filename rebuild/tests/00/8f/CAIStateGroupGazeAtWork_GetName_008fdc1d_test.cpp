#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

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

class CAIStateGroup_GazeAtWork
{
public:
    static CCharString __fastcall GetName();
};

int main()
{
    {
        CCharString name = CAIStateGroup_GazeAtWork::GetName();
        if (g_ctorCount != 1 || name.PStringData != &name) { std::printf("BAD_IDENTITY\n"); return 1; }
        if (g_seenLength != -1) { std::printf("BAD_LEN\n"); return 1; }
        if (!RetailTextEquals(g_seenText, "SG_GAZE_AT_WORK")) { std::printf("BAD_TEXT\n"); return 1; }
    }
    std::printf("GETACTIONNAME_008fdc1d_OK\n");
    return 0;
}
