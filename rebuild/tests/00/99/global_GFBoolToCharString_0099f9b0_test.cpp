// Behaviour test for 0x0099F9B0 GFBoolToCharString: true -> "TRUE", false -> "FALSE", one live instance each,
// the block owned once (NoRefs 1), and the instance counter bumped by the result's construction.
#include <cstdio>
#include <cstring>
#include <cstdlib>

static long g_CCharStringInstanceCount = 0;

class CCharStringData { public:
    char* PData;
    long  NoRefs;
    CCharStringData(const char* text) : NoRefs(1)
    {
        unsigned long n = (unsigned long)std::strlen(text);
        PData = (char*)std::malloc(n + 1);
        std::memcpy(PData, text, n + 1);
    }
};

class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(0)
    {
        ++g_CCharStringInstanceCount;
        if (PStringData != 0)
            UnassignString();
        if (text != 0 && text[0] != '\0')
            PStringData = AllocStringData(text, length);
    }
    CCharString(const CCharString& other) : PStringData(other.PStringData)
    {
        ++g_CCharStringInstanceCount;
        if (PStringData) ++PStringData->NoRefs;
    }
    ~CCharString() { UnassignString(); --g_CCharStringInstanceCount; }
    void UnassignString()
    {
        if (PStringData && --PStringData->NoRefs == 0) {
            std::free(PStringData->PData);
            delete PStringData;
        }
        PStringData = 0;
    }
    CCharStringData* AllocStringData(const char* text, long)
    {
        return new CCharStringData(text);
    }
};

static CCharString GFBoolToCharString(bool value)
{
    if (value == true)
        return CCharString("TRUE", -1);
    return CCharString("FALSE", -1);
}

int main()
{
    {
        CCharString t = GFBoolToCharString(true);
        CCharString f = GFBoolToCharString(false);
        if (t.PStringData == 0 || std::strcmp(t.PStringData->PData, "TRUE") != 0) { std::printf("0099f9b0_TEST FAIL\n"); return 1; }
        if (f.PStringData == 0 || std::strcmp(f.PStringData->PData, "FALSE") != 0) { std::printf("0099f9b0_TEST FAIL\n"); return 1; }
        if (t.PStringData->NoRefs != 1 || f.PStringData->NoRefs != 1) { std::printf("0099f9b0_TEST FAIL\n"); return 1; }
        if (g_CCharStringInstanceCount != 2) { std::printf("0099f9b0_TEST FAIL\n"); return 1; }
    }
    if (g_CCharStringInstanceCount != 0) { std::printf("0099f9b0_TEST FAIL\n"); return 1; }
    std::printf("0099f9b0_TEST PASS\n");
    return 0;
}