#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CBaseObject { virtual void v00() {} long m_refCount; CBaseObject() : m_refCount(0) { Log("obj"); } };
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { Log("ptr"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() { } };
struct CMessageEvent : CBaseObject { unsigned char _pad[0x40]; CIntelligentPointer m_pSender; CIntelligentPointer m_pTarget; CMessageEvent() : m_pSender(), m_pTarget() { } };
int main()
{
    CMessageEvent e;
    if (e.m_pSender.m_pItem != 0 || e.m_pTarget.m_pItem != 0 || e.m_refCount != 0) return Check("00687270", "x");
    return Check("00687270", "obj ptr ptr ");
}