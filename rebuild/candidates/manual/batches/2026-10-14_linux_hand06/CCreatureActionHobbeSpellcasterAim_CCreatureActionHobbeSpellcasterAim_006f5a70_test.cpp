#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
struct CCreatureAction_GenericAimProjectileWeapon { virtual void v00() {} CCreatureAction_GenericAimProjectileWeapon() { Log("aim"); } unsigned char _body[0x110]; };
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { Log("ptr"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() { } };
struct CCreatureAction_HobbeSpellcasterAim : CCreatureAction_GenericAimProjectileWeapon { CIntelligentPointer m_pTarget; CCreatureAction_HobbeSpellcasterAim() : m_pTarget() { } };
int main()
{
    CCreatureAction_HobbeSpellcasterAim a;
    if (a.m_pTarget.m_pItem != 0) return Check("006f5a70", "x");
    return Check("006f5a70", "aim ptr ");
}
