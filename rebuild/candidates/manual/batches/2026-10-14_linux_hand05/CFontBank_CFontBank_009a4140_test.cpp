#include <cstdio>
static int g_strings = 0;
struct CCharString { void* m_pData; CCharString() : m_pData(0) { ++g_strings; } };
struct CFontManager;
struct CBaseClassNonCopyable { virtual void v00() {} CBaseClassNonCopyable() {} };
struct CFontBank : CBaseClassNonCopyable { unsigned long FontHeight; CCharString Name; CFontManager* PManager; virtual void v00() {} CFontBank(); };
CFontBank::CFontBank() : CBaseClassNonCopyable(), Name() { }
int main()
{
    CFontBank b;
    if (b.Name.m_pData != 0 || g_strings != 1) { std::printf("009a4140_TEST FAIL\n"); return 1; }
    std::printf("009a4140_TEST PASS\n");
    return 0;
}
