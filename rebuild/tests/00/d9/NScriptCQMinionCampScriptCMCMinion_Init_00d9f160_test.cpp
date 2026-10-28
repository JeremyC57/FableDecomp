// Test: CMC_Minion::Init -- the camp's minion count goes up and Me is healed by (100 - current health), the third argument false.
#include <cstdio>
class CThing { public: float health; };
class CGame { public: int calls; bool flag;
    float GetHealth(const CThing& t) const { return t.health; }
    void ModifyThingHealth(const CThing& t, float amount, bool f) { ((CThing&)t).health += amount; flag = f; ++calls; } };
struct CCamp { int NumberOfMinions; };
struct CMinion { CGame* Game; CThing Me; CCamp* ParentClass;
    void Init() { ParentClass->NumberOfMinions++; Game->ModifyThingHealth(Me, 100.0f - Game->GetHealth(Me), false); } };
int main()
{
    CGame g; g.calls = 0; g.flag = true; CCamp c; c.NumberOfMinions = 2;
    CMinion m; m.Game = &g; m.Me.health = 35.0f; m.ParentClass = &c; m.Init();
    bool ok = c.NumberOfMinions == 3 && m.Me.health == 100.0f && g.calls == 1 && !g.flag;
    std::printf(ok ? "00d9f160_TEST PASS\n" : "00d9f160_TEST FAIL\n");
    return ok ? 0 : 1;
}