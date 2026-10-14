#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00E95D80: exercises the same logic through a copy of the model (compiled separately from the source)
static bool near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }
struct CScriptThing { long m_id; };
static int g_targetCalls = 0; static const CScriptThing* g_targetThing = 0; static const CScriptThing* g_targetHero = 0;
static int g_usableCalls = 0; static const CScriptThing* g_usableThing = 0; static bool g_usable = false;
struct CGameScriptInterface {
    CScriptThing m_hero; float m_health;
    virtual CScriptThing& GetHero() { return m_hero; }
    virtual float GetHealth(const CScriptThing& thing) const { return m_health + (float)thing.m_id; }
    virtual void SetThingAsUsable(const CScriptThing& thing, bool usable) const { ++g_usableCalls; g_usableThing = &thing; g_usable = usable; }
    virtual void GiveThingBestEnemyTarget(const CScriptThing& thing, const CScriptThing& target) const { ++g_targetCalls; g_targetThing = &thing; g_targetHero = &target; }
};
struct CHHUndead {
    CGameScriptInterface* PQuest; CScriptThing Thing; float RaiseUndeadBarAmount;
    void Init() { PQuest->GiveThingBestEnemyTarget(Thing, PQuest->GetHero()); RaiseUndeadBarAmount = PQuest->GetHealth(Thing); }
};
static bool Run()
{
    CGameScriptInterface quest; quest.m_hero.m_id = 1; quest.m_health = 40.0f;
    CHHUndead u; u.PQuest = &quest; u.Thing.m_id = 2; u.RaiseUndeadBarAmount = 0.0f;
    u.Init();
    return g_targetCalls == 1 && g_targetThing == &u.Thing && g_targetHero == &quest.m_hero && near(u.RaiseUndeadBarAmount, 42.0f);
}
int main()
{
    if (!Run()) { std::printf("00e95d80_TEST FAIL\n"); return 1; }
    std::printf("00e95d80_TEST PASS\n");
    return 0;
}
