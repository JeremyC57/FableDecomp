// Behaviour test for 0x0088B6F0 CAIBrain::OnKill
#include <cstdio>
class CBrainOutput { public: int value; };
class CAIStateGroupBase { public:
    long ScriptAIPriority; bool Terminated; bool Enabled; bool runnable; int cleaned, activated, started;
    CAIStateGroupBase(long p, bool e) : ScriptAIPriority(p), Terminated(false), Enabled(e), runnable(true), cleaned(0), activated(0), started(0) {}
    void Terminate() { Terminated = true; }
    void CleanupMain() { ++cleaned; }
    bool IsEnabledAndRunnable() { return Enabled && runnable; }
    void Activate() { ++activated; }
    void OnStart() { ++started; }
};
struct CStateGroupEntry { long Key; CAIStateGroupBase* Group; };
class CBrainUpdateZone { public: unsigned long UpdatePeriod; };
class CAIBrain { public:
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
    void OnKill()
    {
        if (UpdatingStateGroup) {
            if (PActiveStateGroup && !PActiveStateGroup->Terminated) PActiveStateGroup->Terminate();
            while (UpdatingStateGroup && PActiveStateGroup) UpdateStateGroup();
        }
        if (PActiveStateGroup) { PActiveStateGroup->CleanupMain(); PActiveStateGroup = 0; }
        OnKillCalled = true;
    }
};
int main()
{
    CAIStateGroupBase g(5, true);
    CAIBrain b; b.PActiveStateGroup = &g; b.UpdatingStateGroup = true; b.updatesUntilIdle = 3;
    b.OnKill();
    bool ok = g.Terminated && b.updates == 3 && g.cleaned == 1 && b.PActiveStateGroup == 0 && b.OnKillCalled;
    CAIBrain c; c.OnKill();
    ok = ok && c.OnKillCalled && c.updates == 0;
    if (!ok) { std::printf("0088b6f0_TEST FAIL\n"); return 1; }
    std::printf("0088b6f0_TEST PASS\n");
    return 0;
}