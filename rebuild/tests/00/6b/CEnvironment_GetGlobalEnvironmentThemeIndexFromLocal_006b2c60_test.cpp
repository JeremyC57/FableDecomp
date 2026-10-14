#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

static int g_live = 0; static const char* g_name = 0; static long g_local = 0;
struct CCharString { const char* m_p; long m_len; CCharString(const char* p, long len) : m_p(p), m_len(len) { ++g_live; } ~CCharString() { --g_live; } };
struct CGameDefinitionManager { long GetGlobalIndexFromLocal(const CCharString& className, long localIndex) { g_name = className.m_p; g_local = localIndex; return localIndex + 1000; } };
struct CEnvironment { void* __vftable; CGameDefinitionManager* DefinitionManager;
    long GetGlobalEnvironmentThemeIndexFromLocal(long localIndex) { CCharString className("ENVIRONMENT_THEME_DAY", -1); return DefinitionManager->GetGlobalIndexFromLocal(className, localIndex); } };
int main()
{
    CGameDefinitionManager m; CEnvironment e; e.DefinitionManager = &m;
    long r = e.GetGlobalEnvironmentThemeIndexFromLocal(7);
    bool ok = r == 1007 && g_local == 7 && std::strcmp(g_name, "ENVIRONMENT_THEME_DAY") == 0 && g_live == 0;
    if (!ok) { std::printf("006b2c60_TEST FAIL\n"); return 1; }
    std::printf("006b2c60_TEST PASS\n"); return 0;
}