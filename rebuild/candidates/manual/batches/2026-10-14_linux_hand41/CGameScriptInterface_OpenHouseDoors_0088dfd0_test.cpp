#include <cstdio>
#include <cstring>
#include <new>

static int g_calls; static bool g_lock = !false; static bool g_silent = true;
struct CThingBuilding { unsigned char m_typeFlags; void LockAllDoors(bool lock, bool silent) { ++g_calls; g_lock = lock; g_silent = silent; } };
struct CScriptThing { virtual CThingBuilding* GetThing() const { return m_thing; } virtual bool IsValid() const { return m_valid; } CThingBuilding* m_thing; bool m_valid; };
struct CGameScriptInterface {
    void OpenHouseDoors(const CScriptThing& thing) const {
        if (thing.IsValid()) { CThingBuilding* building = thing.GetThing(); if (building->m_typeFlags & 0x40) building->LockAllDoors(false, false); } }
};
int main()
{
    CThingBuilding b; b.m_typeFlags = 0x40; CScriptThing st; st.m_thing = &b; st.m_valid = true; CGameScriptInterface si;
    si.OpenHouseDoors(st); if (g_calls != 1 || g_lock != false || g_silent) { std::printf("0088dfd0_TEST FAIL\n"); return 1; }
    b.m_typeFlags = 0x3f; si.OpenHouseDoors(st); if (g_calls != 1) { std::printf("0088dfd0_TEST FAIL\n"); return 1; }
    b.m_typeFlags = 0x40; st.m_valid = false; si.OpenHouseDoors(st); if (g_calls != 1) { std::printf("0088dfd0_TEST FAIL\n"); return 1; }
    std::printf("0088dfd0_TEST PASS\n");
    return 0;
}
