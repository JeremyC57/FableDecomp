// Behaviour test for 0x0088BAC0 CAIBrain::IsReadyToRunStateGroupWithHigherScriptPriorityThan
#include <cstdio>
struct CBrainOutput { int value; };
struct CAIStateGroupBase {
    long ScriptAIPriority; bool Terminated; bool Enabled; bool runnable; int cleaned, activated, started;
    CAIStateGroupBase(long p, bool e) : ScriptAIPriority(p), Terminated(false), Enabled(e), runnable(true), cleaned(0), activated(0), started(0) {}
    void Terminate() { Terminated = true; }
    void CleanupMain() { ++cleaned; }
    bool IsEnabledAndRunnable() { return Enabled && runnable; }
    void Activate() { ++activated; }
    void OnStart() { ++started; }
};
struct CStateGroupEntry { long Key; CAIStateGroupBase* Group; };
struct CBrainUpdateZone { unsigned long UpdatePeriod; };
struct CAIBrain {
    unsigned long frame; unsigned long long offset; CBrainUpdateZone* zones;
    CStateGroupEntry* StateGroupBegin; CStateGroupEntry* StateGroupEnd;
    CAIStateGroupBase* PActiveStateGroup; CAIStateGroupBase* PPendingStateGroup;
    CBrainOutput Output; bool OutputValid; bool UpdatingStateGroup; CBrainUpdateZone* PCurrentUpdateZone; bool OnKillCalled;
    int updates; int updatesUntilIdle; bool outputOnCleanup;
    CAIBrain() : frame(0), offset(0), zones(0), StateGroupBegin(0), StateGroupEnd(0), PActiveStateGroup(0), PPendingStateGroup(0),
        OutputValid(false), UpdatingStateGroup(false), PCurrentUpdateZone(0), OnKillCalled(false), updates(0), updatesUntilIdle(0), outputOnCleanup(false)
    { Output.value = 0; }
    void UpdateStateGroup() { ++updates; if (--updatesUntilIdle <= 0) UpdatingStateGroup = false; }
    CBrainUpdateZone* GetUpdateZone(long zone) { return zone < 0 ? 0 : &zones[zone]; }
    bool IsReady(long priority, long zone)
    {
        CBrainUpdateZone* updateZone = GetUpdateZone(zone);
        PCurrentUpdateZone = updateZone;
        unsigned long period = 1;
        if (updateZone) period = updateZone->UpdatePeriod;
        unsigned long long o = offset;
        if ((o + frame) % period != 0) return false;
        for (CStateGroupEntry* it = StateGroupBegin; it != StateGroupEnd; ++it) {
            CAIStateGroupBase* group = it->Group;
            if (group->ScriptAIPriority > priority && group->IsEnabledAndRunnable()) {
                if (PActiveStateGroup == 0) { PActiveStateGroup = group; group->Activate(); PActiveStateGroup->OnStart(); }
                return true;
            }
        }
        return false;
    }
};
int main()
{
    CBrainUpdateZone zones[1] = { { 4 } };
    CAIStateGroupBase g1(10, false), g2(8, true), g3(2, true);
    CStateGroupEntry list[3] = { {1, &g1}, {2, &g2}, {3, &g3} };
    CAIBrain b; b.zones = zones; b.StateGroupBegin = list; b.StateGroupEnd = list + 3; b.offset = 1; b.frame = 2;
    bool ok = !b.IsReady(5, 0) && b.PCurrentUpdateZone == &zones[0];           // 3 % 4 != 0
    b.frame = 3;
    ok = ok && b.IsReady(5, 0) && b.PActiveStateGroup == &g2 && g2.activated == 1 && g2.started == 1;
    ok = ok && !b.IsReady(9, 0) && b.IsReady(1, -1) && g2.activated == 1;       // already active: no re-activation
    if (!ok) { std::printf("0088bac0_TEST FAIL\n"); return 1; }
    std::printf("0088bac0_TEST PASS\n");
    return 0;
}
