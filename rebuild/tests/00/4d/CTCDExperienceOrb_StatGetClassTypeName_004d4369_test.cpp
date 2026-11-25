// Behaviour test for 0x004D4369 CTCDExperienceOrb::StatGetClassTypeName (NF414): links the row object (the static is declared here, not defined) and
// asserts what it hands to CCharString(const char*, long): retail's text "CTCDExperienceOrb", length -1, one constructor call, constructed into the
// caller's own hidden result slot.
#include <cstdio>
#include <string.h>

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

class CTCDExperienceOrb
{
public:
    static CCharString __fastcall StatGetClassTypeName();
};

int main()
{
    {
        CCharString name = CTCDExperienceOrb::StatGetClassTypeName();
        if (g_ctorCount != 1 || name.PStringData != &name) { std::printf("BAD_IDENTITY\n"); return 1; }
        if (g_seenLength != -1) { std::printf("BAD_LEN\n"); return 1; }
        if (g_seenText == 0 || strcmp(g_seenText, "CTCDExperienceOrb") != 0) { std::printf("BAD_TEXT\n"); return 1; }
    }
    std::printf("OK_004d4369\n");
    return 0;
}
