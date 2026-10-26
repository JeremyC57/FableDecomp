#include <cstdio>
// Model of CAIStateGroup_VersusEnemyBase::OnActivate @ 0x00962A90: register creature vs live target unless already registered.
class CThing { public: unsigned char m_flags; };
struct Creature { bool registered; bool IsRegisteredCombatant() { return registered; } };
class CCombatManager { public: Creature* a; CThing* b; bool flag; int calls; void RegisterCombatant(Creature* x, CThing* y, bool f) { a = x; b = y; flag = f; ++calls; } };
class CWorld { public: CCombatManager* m_pCombatManager; };
struct Ptr { CThing* item; CThing* GetPItem() { return item; } bool IsAlive() { if (GetPItem() != 0) { if (!(GetPItem()->m_flags & 1)) return true; } return false; } };
static CCombatManager g_cm; static CWorld g_world = { &g_cm };
struct Group {
    Creature* creature; Ptr PTargetThing;
    CWorld* GetPWorld() { return &g_world; }
    void OnActivate()
    {
        if (!creature->IsRegisteredCombatant()) {
            if (PTargetThing.IsAlive()) { Creature* c = creature; CCombatManager* cm = GetPWorld()->m_pCombatManager; cm->RegisterCombatant(c, PTargetThing.GetPItem(), true); }
        }
    }
};
int main()
{
    Creature c = { false }; CThing t = { 0 }; Group g; g.creature = &c; g.PTargetThing.item = &t; g_cm.calls = 0;
    g.OnActivate();
    if (g_cm.calls != 1 || g_cm.a != &c || g_cm.b != &t || !g_cm.flag) { std::printf("00962a90_TEST FAIL\n"); return 1; }
    c.registered = true; g.OnActivate(); if (g_cm.calls != 1) { std::printf("00962a90_TEST FAIL\n"); return 1; }
    c.registered = false; t.m_flags = 1; g.OnActivate(); if (g_cm.calls != 1) { std::printf("00962a90_TEST FAIL\n"); return 1; }
    std::printf("00962a90_TEST PASS\n"); return 0;
}