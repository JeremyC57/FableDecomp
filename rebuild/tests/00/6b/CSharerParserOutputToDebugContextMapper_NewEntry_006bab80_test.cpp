#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
static int g_defaults = 0, g_stringCopies = 0, g_setCopies = 0, g_destroyed = 0;
struct CCharString {
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString(const CCharString& o) : m_pData(o.m_pData) { ++g_stringCopies; }
};
struct SStringSet {
    void* m_pHeader; unsigned long m_count; unsigned long m_compare;
    SStringSet() : m_pHeader(0), m_count(0), m_compare(0) { }
    SStringSet(const SStringSet& o) : m_pHeader(o.m_pHeader), m_count(o.m_count), m_compare(o.m_compare) { ++g_setCopies; }
};
struct CEntry {
    CCharString Name; SStringSet Sharers; SStringSet Contexts;
    CEntry() { ++g_defaults; Name.m_pData = "default"; Sharers.m_count = 3; Contexts.m_count = 4; }
    ~CEntry() { ++g_destroyed; }
};
static CEntry __fastcall NewEntry()
{
    CEntry entry;
    return entry;
}
int main()
{
    {
        CEntry e = NewEntry();
        if (g_defaults != 1 || e.Name.m_pData[0] != 'd' || e.Sharers.m_count != 3 || e.Contexts.m_count != 4) { std::printf("006bab80_TEST FAIL\n"); return 1; }
        if (g_setCopies != 2 * g_stringCopies) { std::printf("006bab80_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != g_defaults + g_stringCopies) { std::printf("006bab80_TEST FAIL\n"); return 1; }
    std::printf("006bab80_TEST PASS\n");
    return 0;
}