#include <cstdio>
struct CScriptThing { unsigned char data[12]; };
static const CScriptThing* g_thing[2] = { 0, 0 }; static bool g_enabled[2] = { true, true }; static int g_calls = 0;
struct CGameScriptInterface_Calls {
    virtual void EntitySetAllStategroupsEnabled(const CScriptThing& thing, bool enabled) { g_thing[0] = &thing; g_enabled[0] = enabled; ++g_calls; }
    virtual void EntitySetCombatEnabled(const CScriptThing& thing, bool enabled) { g_thing[1] = &thing; g_enabled[1] = enabled; ++g_calls; }
};
struct CCham {
    virtual void Init() { m_active = false; m_triggered = false; m_pInterface->EntitySetAllStategroupsEnabled(m_cham, false); m_pInterface->EntitySetCombatEnabled(m_cham, false); }
    CGameScriptInterface_Calls* m_pInterface; CScriptThing m_cham; void* ParentClass; void* PMasterData; bool m_active; bool m_triggered;
};
int main()
{
    CGameScriptInterface_Calls iface; CCham cham; cham.m_pInterface = &iface; cham.m_active = true; cham.m_triggered = true;
    cham.Init();
    if (cham.m_active || cham.m_triggered || g_calls != 2 || g_thing[0] != &cham.m_cham || g_thing[1] != &cham.m_cham || g_enabled[0] || g_enabled[1]) { std::printf("00f19b60_TEST FAIL\n"); return 1; }
    std::printf("00f19b60_TEST PASS\n"); return 0;
}