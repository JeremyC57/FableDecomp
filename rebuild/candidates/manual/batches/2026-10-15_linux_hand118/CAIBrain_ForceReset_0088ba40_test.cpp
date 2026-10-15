// Behaviour test for 0x0088BA40 CAIBrain::ForceReset
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
    bool ForceReset(CBrainOutput* output)
    {
        bool copied = false;
        if (PActiveStateGroup) {
            if (!PActiveStateGroup->Terminated) PActiveStateGroup->Terminate();
            while (UpdatingStateGroup) UpdateStateGroup();
            OutputValid = false;
            PActiveStateGroup->CleanupMain();
            if (outputOnCleanup) OutputValid = true;     // the cleanup produced an output
            if (OutputValid) { copied = OutputValid; *output = Output; }
        }
        PPendingStateGroup = 0;
        PActiveStateGroup = 0;
        return copied;
    }
};
int main()
{
    CAIStateGroupBase g(5, true), p(1, true);
    CAIBrain b; b.PActiveStateGroup = &g; b.PPendingStateGroup = &p; b.UpdatingStateGroup = true; b.updatesUntilIdle = 2;
    b.Output.value = 77; b.outputOnCleanup = true;
    CBrainOutput out; out.value = 0;
    bool r = b.ForceReset(&out);
    bool ok = r && out.value == 77 && g.Terminated && g.cleaned == 1 && b.updates == 2 && b.PActiveStateGroup == 0 && b.PPendingStateGroup == 0;
    CAIBrain c; c.PPendingStateGroup = &p; out.value = 1;
    ok = ok && !c.ForceReset(&out) && out.value == 1 && c.PPendingStateGroup == 0;
    if (!ok) { std::printf("0088ba40_TEST FAIL\n"); return 1; }
    std::printf("0088ba40_TEST PASS\n");
    return 0;
}
