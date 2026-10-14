#include <cstdio>
static int g_stringDtors = 0;
struct CCharString { void* PStringData; ~CCharString() { ++g_stringDtors; } };
struct CAnimFilterBase { virtual void v00() { } ~CAnimFilterBase() { } };
struct __declspec(novtable) CAnimFilterNamed : CAnimFilterBase { CCharString m_name; ~CAnimFilterNamed() { } };
int main()
{
    { CAnimFilterNamed f; f.m_name.PStringData = 0; if ((char*)&f.m_name != (char*)&f + 4) { std::printf("00739db0_TEST FAIL\n"); return 1; } }
    if (g_stringDtors != 1) { std::printf("00739db0_TEST FAIL\n"); return 1; }
    std::printf("00739db0_TEST PASS\n"); return 0;
}