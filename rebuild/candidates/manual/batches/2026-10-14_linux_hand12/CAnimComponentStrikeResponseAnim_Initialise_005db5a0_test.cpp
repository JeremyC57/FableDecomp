// Behaviour test for CAnimComponentStrikeResponseAnim::Initialise @ 0x005db5a0: defaults first; name longer than 2 -> member = name minus its first char, true; else false.
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct CPersistContext { int dummy; };
static int g_defaults = 0, g_live = 0;
struct CCharStringData { long m_refCount; long Length; char text[32]; };
struct CCharString {
    CCharStringData* PStringData;
    CCharString() : PStringData(0) { ++g_live; }
    CCharString(const char* s) { PStringData = (CCharStringData*)std::malloc(sizeof(CCharStringData)); PStringData->Length = (long)std::strlen(s); std::strcpy(PStringData->text, s); ++g_live; }
    CCharString(const CCharString& o) : PStringData(0) { ++g_live; *this = o; }
    ~CCharString() { std::free(PStringData); --g_live; }
    CCharString& operator=(const CCharString& o)
    { std::free(PStringData); PStringData = 0; if (o.PStringData) { PStringData = (CCharStringData*)std::malloc(sizeof(CCharStringData)); *PStringData = *o.PStringData; } return *this; }
    CCharString GetSubString(unsigned long start, unsigned long count) const { char buf[32]; std::memcpy(buf, PStringData->text + start, count); buf[count] = 0; return CCharString(buf); }
    long GetLength() const { return PStringData ? PStringData->Length : 0; }
};
struct CAnimComponentStrikeResponseAnim {
    virtual void SetDefaultValue() { ++g_defaults; }
    long m_refCount; CCharString ResponseAnimName;
    bool Initialise(CPersistContext& context, const CCharString& name)
    { SetDefaultValue(); long length = name.GetLength(); if (length > 2) { ResponseAnimName = name.GetSubString(1, length - 1); return true; } return false; }
};
int main()
{
    CPersistContext ctx; bool ok;
    {
        CAnimComponentStrikeResponseAnim c; CCharString name("_Strike");
        ok = c.Initialise(ctx, name) == true && g_defaults == 1 && std::strcmp(c.ResponseAnimName.PStringData->text, "Strike") == 0;
        CCharString shortName("ab"); if (c.Initialise(ctx, shortName) != false || g_defaults != 2) ok = false;
        CCharString empty; if (c.Initialise(ctx, empty) != false) ok = false;
        if (std::strcmp(c.ResponseAnimName.PStringData->text, "Strike") != 0) ok = false;
    }
    if (g_live != 0) ok = false;                                             // every temporary destroyed
    if (!ok) { std::printf("005db5a0_TEST FAIL\n"); return 1; }
    std::printf("005db5a0_TEST PASS\n");
    return 0;
}
