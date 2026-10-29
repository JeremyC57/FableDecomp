// Behaviour test for 0x00E5E240 NScript::CV_BodyGuardScript::GetHenchmanTypeFromString: "_EVIL1" 0, "_EVIL2" 1, "_GOOD1" 2,
// "_GOOD2" 3 and every other text (including the empty string) 2; the null-data arm compares against "".
#include <cstdio>
#include <cstring>

class CCharString { public:
    const char* PStringData;                            // null = the empty string
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return std::strcmp(text, "") == 0;
        return std::strcmp(PStringData, text) == 0;
    }
};
struct CV_BodyGuardScript {
    long GetHenchmanTypeFromString(const CCharString& text)
    {
        if (text == "_EVIL1")
            return 0;
        if (text == "_EVIL2")
            return 1;
        if (text == "_GOOD1")
            return 2;
        if (text == "_GOOD2")
            return 3;
        return 2;
    }
};

static long T(const char* s) { CV_BodyGuardScript b; CCharString c; c.PStringData = s; return b.GetHenchmanTypeFromString(c); }

int main()
{
    int ok = T("_EVIL1") == 0 && T("_EVIL2") == 1 && T("_GOOD1") == 2 && T("_GOOD2") == 3 && T("_EVIL3") == 2
             && T("") == 2 && T(0) == 2;
    if (!ok) { std::printf("00e5e240_TEST FAIL\n"); return 1; }
    std::printf("00e5e240_TEST PASS\n");
    return 0;
}