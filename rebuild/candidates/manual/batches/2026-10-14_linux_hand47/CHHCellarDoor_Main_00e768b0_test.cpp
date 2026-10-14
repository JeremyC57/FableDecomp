#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00E768B0: exercises the same logic through a copy of the model (compiled separately from the source)
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
struct CV_ExposeMayorScript { bool m_cellarDoorUsable; };
struct CHH_CellarDoor {
    CGameScriptInterface* PQuest; CScriptThing Thing; CV_ExposeMayorScript* ParentClass; bool m_aborted;
    bool IsAborted() { return m_aborted; }
    void Main()
    {
        if (!ParentClass->m_cellarDoorUsable) { if (!IsAborted()) PQuest->SetThingAsUsable(Thing, false); }
        else { if (!IsAborted()) PQuest->SetThingAsUsable(Thing, true); }
    }
};
static bool Run()
{
    CGameScriptInterface quest; CV_ExposeMayorScript parent; CHH_CellarDoor d; d.PQuest = &quest; d.ParentClass = &parent; d.m_aborted = false; d.Thing.m_id = 9;
    parent.m_cellarDoorUsable = false; d.Main();
    if (g_usableCalls != 1 || g_usable != false || g_usableThing != &d.Thing) return false;
    parent.m_cellarDoorUsable = true; d.Main();
    if (g_usableCalls != 2 || g_usable != true) return false;
    d.m_aborted = true; d.Main(); parent.m_cellarDoorUsable = false; d.Main();
    return g_usableCalls == 2;                     // aborted: no call either way
}
int main()
{
    if (!Run()) { std::printf("00e768b0_TEST FAIL\n"); return 1; }
    std::printf("00e768b0_TEST PASS\n");
    return 0;
}
