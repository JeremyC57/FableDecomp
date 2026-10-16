// behaviour test for CThingAICreature::SetHasInformation (0x00830EB0)
#include <cstdio>
static int g_refreshes = 0;
struct CThingAICreature {
    bool PerformingVillageTaskFlag : 1; bool AISuspended : 1; bool HasInformation : 1; bool WanderWithInformation : 1;
    bool WaveWithInformation : 1; bool ContinueAIWithInformation : 1; bool ImmuneToAttack : 1; bool EnableCreatureAutoPlacing : 1;
    void SetHasInformation(bool hasInformation, bool wander, bool wave, bool continueAI)
    {
        bool changed = HasInformation != hasInformation;
        HasInformation = hasInformation; WanderWithInformation = wander; WaveWithInformation = wave; ContinueAIWithInformation = continueAI;
        if (changed) ++g_refreshes;
    }
};
int main()
{
    CThingAICreature c; c.PerformingVillageTaskFlag = true; c.AISuspended = false; c.HasInformation = false; c.WanderWithInformation = false;
    c.WaveWithInformation = false; c.ContinueAIWithInformation = false; c.ImmuneToAttack = true; c.EnableCreatureAutoPlacing = true;
    c.SetHasInformation(true, true, false, true);
    int bad = g_refreshes != 1 || !c.HasInformation || !c.WanderWithInformation || c.WaveWithInformation || !c.ContinueAIWithInformation || !c.PerformingVillageTaskFlag || !c.ImmuneToAttack || !c.EnableCreatureAutoPlacing || c.AISuspended;
    c.SetHasInformation(true, false, true, false);
    if (g_refreshes != 1 || c.WanderWithInformation || !c.WaveWithInformation) bad = 1;
    c.SetHasInformation(false, false, false, false);
    if (g_refreshes != 2 || c.HasInformation) bad = 1;
    if (bad) { std::printf("00830eb0_TEST FAIL\n"); return 1; }
    std::printf("00830eb0_TEST PASS\n");
    return 0;
}