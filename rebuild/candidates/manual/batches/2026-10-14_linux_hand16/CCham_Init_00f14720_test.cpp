#include <cstdio>
static int g_calls = 0; static long g_slots[4]; static long g_args[4]; static const void* g_things[4];
struct CScriptThing { void* __vftable; void* PImp_Data; void* PImp_Info; };
struct CGameScriptInterface { virtual void F594(const CScriptThing& t, long a) { g_slots[g_calls] = 594; g_args[g_calls] = a; g_things[g_calls] = &t; ++g_calls; } virtual void F595(const CScriptThing& t, long a) { g_slots[g_calls] = 595; g_args[g_calls] = a; g_things[g_calls] = &t; ++g_calls; } };
struct CCham { virtual void v00() {} CGameScriptInterface* m_pInterface; CScriptThing m_thing;  void Init() { m_pInterface->F594(m_thing, 0); m_pInterface->F595(m_thing, 0); } };

int main()
{
    CGameScriptInterface iface; CCham obj; obj.m_pInterface = &iface;
    obj.Init();
    if (g_calls != 2 || g_slots[0] != 594 || g_args[0] != 0 || g_slots[1] != 595 || g_args[1] != 0 || g_things[0] != &obj.m_thing) { std::printf("00f14720_TEST FAIL\n"); return 1; }
    std::printf("00f14720_TEST PASS\n");
    return 0;
}
