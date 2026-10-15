// Behaviour test for 0x0097E200: DoNothing returns 1 when stopping, else ticks look + brain and returns 1/4.
#include <cstdio>
struct CTCLook { int tracked; bool TrackNearestInterestingThing(bool) { tracked++; return true; } };
struct CCreature { CTCLook* look; bool GetInterface(CTCLook** out, long t) { if (t == 0x43 && look) { *out = look; return true; } return false; } };
struct CAIBrain { int updates; CCreature* Creature; void Update() { updates++; } };
struct CGroup {
    CAIBrain* PBrain; int stopCalls; int stopAfter;
    bool IsStopping() { return ++stopCalls > stopAfter; }
    long DoNothing()
    {
        if (IsStopping()) return 1;
        CTCLook* look;
        if (PBrain->Creature->GetInterface(&look, 0x43)) look->TrackNearestInterestingThing(true);
        PBrain->Update();
        return IsStopping() ? 1 : 4;
    }
};
int main()
{
    CTCLook look = { 0 }; CCreature cr = { &look }; CAIBrain br = { 0, &cr };
    CGroup g = { &br, 0, 5 };
    int bad = 0;
    if (g.DoNothing() != 4 || look.tracked != 1 || br.updates != 1) bad = 1;
    g.stopCalls = 0; g.stopAfter = 1;
    if (g.DoNothing() != 1 || look.tracked != 2 || br.updates != 2) bad = 2;
    g.stopCalls = 0; g.stopAfter = 0;
    if (g.DoNothing() != 1 || look.tracked != 2) bad = 3;
    cr.look = 0; g.stopCalls = 0; g.stopAfter = 5;
    if (g.DoNothing() != 4 || br.updates != 3) bad = 4;
    if (bad) { std::printf("0097e200_TEST FAIL\n"); return 1; }
    std::printf("0097e200_TEST PASS\n");
    return 0;
}
