#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; ~CBaseIntelligentPointer() { char b[16]; std::sprintf(b, "ptr%d", (int)(long)m_pItem); Log(b); } };
struct CIntelligentPointer : CBaseIntelligentPointer { };
struct CCharString { char* m_pData; ~CCharString() { Log("str"); } };
struct CAnimFilterBase { virtual void v00() {} ~CAnimFilterBase() { Log("filterbase"); } };
struct CAnimFilterOfGroup : CAnimFilterBase { CCharString m_group; long m_groupIndex; };
struct CModeStandBase { virtual void v00() {} ~CModeStandBase() { Log("stand"); } unsigned char _pad[0x64]; };
struct CModeStandNamed_008cc280 : CModeStandBase { CAnimFilterOfGroup m_filter; CIntelligentPointer m_pThing0, m_pThing1, m_pThing2; ~CModeStandNamed_008cc280() { } };
int main()
{
    { CModeStandNamed_008cc280 m; m.m_pThing0.m_pItem = (void*)0; m.m_pThing1.m_pItem = (void*)1; m.m_pThing2.m_pItem = (void*)2; }
    return Check("008cc280", "ptr2 ptr1 ptr0 str filterbase stand ");
}
