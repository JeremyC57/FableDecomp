// Behaviour test for 006bab80: CPersistContext::GetDefaultVal<CFaction>() (NF382 name fix, was CSharerParserOutputToDebugContextMapper::NewEntry)
#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
static int g_defaults = 0, g_stringCopies = 0, g_setCopies = 0, g_destroyed = 0;
class CCharString { public:
    const char* m_pData;
    CCharString() : m_pData("") { }
    CCharString(const CCharString& o) : m_pData(o.m_pData) { ++g_stringCopies; }
};
struct SStringSet {
    void* m_pHeader; unsigned long m_count; unsigned long m_compare;
    SStringSet() : m_pHeader(0), m_count(0), m_compare(0) { }
    SStringSet(const SStringSet& o) : m_pHeader(o.m_pHeader), m_count(o.m_count), m_compare(o.m_compare) { ++g_setCopies; }
};
class CFaction { public:
    CCharString Name; SStringSet AlliedFactions; SStringSet EnemyFactions;
    CFaction() { ++g_defaults; Name.m_pData = "default"; AlliedFactions.m_count = 3; EnemyFactions.m_count = 4; }
    ~CFaction() { ++g_destroyed; }
};
class CPersistContext { public:
    template <class T> static T __fastcall GetDefaultVal() { T value; return value; }
};
int main()
{
    {
        CFaction e = CPersistContext::GetDefaultVal<CFaction>();
        if (g_defaults != 1 || e.Name.m_pData[0] != 'd' || e.AlliedFactions.m_count != 3 || e.EnemyFactions.m_count != 4) { std::printf("006bab80_TEST FAIL\n"); return 1; }
        if (g_setCopies != 2 * g_stringCopies) { std::printf("006bab80_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != g_defaults + g_stringCopies) { std::printf("006bab80_TEST FAIL\n"); return 1; }
    std::printf("006bab80_TEST PASS\n");
    return 0;
}