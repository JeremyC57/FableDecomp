#include <cstdio>
static int g_calls = 0; static long g_slots[4]; static long g_args[4]; static const void* g_things[4];
struct CScriptThing { void* __vftable; void* PImp_Data; void* PImp_Info; };
struct CGameScriptInterface { virtual void F477(const CScriptThing& t, long a) { g_slots[g_calls] = 477; g_args[g_calls] = a; g_things[g_calls] = &t; ++g_calls; } virtual void F511(const CScriptThing& t, long a) { g_slots[g_calls] = 511; g_args[g_calls] = a; g_things[g_calls] = &t; ++g_calls; } };
struct CScriptObject_00ee14b0 { virtual void v00() {} CGameScriptInterface* m_pInterface; CScriptThing m_thing;  void Init() { m_pInterface->F477(m_thing, 1); m_pInterface->F511(m_thing, 0); } };

int main()
{
    CGameScriptInterface iface; CScriptObject_00ee14b0 obj; obj.m_pInterface = &iface;
    obj.Init();
    if (g_calls != 2 || g_slots[0] != 477 || g_args[0] != 1 || g_slots[1] != 511 || g_args[1] != 0 || g_things[0] != &obj.m_thing || g_things[1] != &obj.m_thing) { std::printf("00ee14b0_TEST FAIL\n"); return 1; }
    std::printf("00ee14b0_TEST PASS\n");
    return 0;
}