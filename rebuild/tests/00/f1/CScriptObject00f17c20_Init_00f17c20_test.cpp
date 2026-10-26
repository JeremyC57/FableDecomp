#include <cstdio>
static int g_calls = 0; static long g_slots[4]; static long g_args[4]; static const void* g_things[4];
class CScriptThing { public: void* __vftable; void* PImp_Data; void* PImp_Info; };
class CGameScriptInterface { public: virtual void F591(const CScriptThing& t, long a) { g_slots[g_calls] = 591; g_args[g_calls] = a; g_things[g_calls] = &t; ++g_calls; } virtual void F592(const CScriptThing& t, long a) { g_slots[g_calls] = 592; g_args[g_calls] = a; g_things[g_calls] = &t; ++g_calls; } };
struct CScriptObject_00f17c20 { virtual void v00() {} CGameScriptInterface* m_pInterface; CScriptThing m_thing; unsigned char _pad[8]; bool m_bState1C; bool m_bState1D; void Init() { m_bState1C = false; m_bState1D = false; m_pInterface->F591(m_thing, 0); m_pInterface->F592(m_thing, 0); } };

int main()
{
    CGameScriptInterface iface; CScriptObject_00f17c20 obj; obj.m_pInterface = &iface; obj.m_bState1C = true; obj.m_bState1D = true;
    obj.Init();
    if (g_calls != 2 || g_slots[0] != 591 || g_slots[1] != 592 || obj.m_bState1C || obj.m_bState1D || g_things[1] != &obj.m_thing) { std::printf("00f17c20_TEST FAIL\n"); return 1; }
    std::printf("00f17c20_TEST PASS\n");
    return 0;
}