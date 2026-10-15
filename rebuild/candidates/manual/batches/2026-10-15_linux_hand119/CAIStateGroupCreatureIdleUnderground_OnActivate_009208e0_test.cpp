// Behaviour test for 0x009208E0: go underground unless already in mode 0x1e (or no mode manager).
#include <cstdio>
static int g_actionsBuilt = 0, g_actionsDestroyed = 0;
struct CCreature;
struct CAction { CCreature* who; CAction(CCreature* c) : who(c) { ++g_actionsBuilt; } ~CAction() { ++g_actionsDestroyed; } };
struct CMode { long type; long GetType() { return type; } };
struct CModes { CMode* current; CMode* GetCurrentMode() { return current; } };
struct CCreature { CModes* modes; int set; CCreature* actor;
    bool GetInterface(CModes** out, long t) { if (t == 0x31 && modes) { *out = modes; return true; } return false; }
    bool SetCurrentAction(CAction* a) { ++set; actor = a->who; return true; } };
static void OnActivate(CCreature* c)
{
    CModes* modes;
    if (c->GetInterface(&modes, 0x31)) {
        if (modes->GetCurrentMode()->GetType() != 0x1e) {
            CCreature* creature = c;
            CAction action(creature);
            creature->SetCurrentAction(&action);
        }
    }
}
int main()
{
    CMode m = { 3 }; CModes mm = { &m }; CCreature c = { &mm, 0, 0 };
    OnActivate(&c);
    int bad = (c.set != 1 || c.actor != &c || g_actionsBuilt != 1 || g_actionsDestroyed != 1);
    m.type = 0x1e; OnActivate(&c); if (c.set != 1) bad = 2;
    c.modes = 0; OnActivate(&c); if (c.set != 1) bad = 3;
    if (bad) { std::printf("009208e0_TEST FAIL\n"); return 1; }
    std::printf("009208e0_TEST PASS\n");
    return 0;
}
