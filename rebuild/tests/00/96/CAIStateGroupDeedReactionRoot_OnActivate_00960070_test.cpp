// behaviour test for CAIStateGroup_DeedReactionRoot::OnActivate (0x00960070)
#include <cstdio>
struct CThing { bool dead; unsigned long long UniqueID; };
struct CLog { int applied; void ApplyPendingDeedLog() { ++applied; } };
struct Root {
    CThing* ThingReactingTo; bool DeedIsVirtual; unsigned long long LastUID;
    bool DeliveredReaction, DeliveredSpeech, Terminated; CLog* log;
    void OnActivate()
    {
        if (!(ThingReactingTo && !ThingReactingTo->dead)) { Terminated = true; return; }
        DeliveredReaction = false; DeliveredSpeech = false;
        if (DeedIsVirtual) { LastUID = ThingReactingTo->UniqueID; return; }
        log->ApplyPendingDeedLog();
    }
};
int main()
{
    CThing t = { false, 0x123456789ULL }; CLog l = { 0 };
    Root r = { &t, true, 0, true, true, false, &l };
    r.OnActivate();
    if (r.LastUID != 0x123456789ULL || r.DeliveredReaction || r.DeliveredSpeech || l.applied) { std::printf("00960070_TEST FAIL\n"); return 1; }
    r.DeedIsVirtual = false; r.OnActivate();
    if (l.applied != 1 || r.Terminated) { std::printf("00960070_TEST FAIL\n"); return 1; }
    t.dead = true; r.OnActivate();
    if (!r.Terminated || l.applied != 1) { std::printf("00960070_TEST FAIL\n"); return 1; }
    std::printf("00960070_TEST PASS\n");
    return 0;
}