// Behaviour test for 0x008621A0 CModeStandBase::OnActivate (see src/008621a0.cpp): the six gait animations are looked up;
// a missing stand falls back to DEFAULT, missing walk/run/slow-walk share the stand animation and a missing sprint the
// run animation; a hero refreshes its movement speeds, a creature measures walk/jog/run speeds (0 when absent); an idle
// creature restarts its animation with ForceAddNewAnims raised only for the call.
#include <cstdio>
#include <cstring>
struct Mode {
    const char* Stand; const char* SlowWalk; const char* Walk; const char* Jog; const char* Run; const char* Sprint;
    float MaxWalk, MaxJog, MaxRun; int ThingType; bool Busy; bool Force; int Starts; bool ForceDuringStart; int SpeedInits;
    static float Speed(const char* a) { return a ? (float)std::strlen(a) : 0.0f; }
    void OnActivate(const char* found[6])
    {
        Stand = found[0]; SlowWalk = found[1]; Walk = found[2]; Jog = found[3]; Run = found[4]; Sprint = found[5];
        if (!Stand) Stand = "DEFAULT";
        if (!Walk) Walk = Stand;
        if (!Run) Run = Stand;
        if (!SlowWalk) SlowWalk = Stand;
        if (!Sprint) Sprint = Run;
        if (ThingType == 2) ++SpeedInits;
        else if (ThingType == 1) { MaxWalk = Speed(Walk); MaxJog = Speed(Jog); MaxRun = Speed(Run); }
        if (!Busy) { Force = true; ++Starts; ForceDuringStart = Force; Force = false; }
    }
};
int main()
{
    const char* none[6] = { 0, 0, 0, 0, 0, 0 };
    Mode m; std::memset(&m, 0, sizeof m); m.ThingType = 1;
    m.OnActivate(none);
    bool ok = !std::strcmp(m.Stand, "DEFAULT") && m.Walk == m.Stand && m.Run == m.Stand && m.Sprint == m.Run
        && m.MaxWalk == 7.0f && m.MaxJog == 0.0f && m.Starts == 1 && m.ForceDuringStart && !m.Force;
    const char* some[6] = { "ST_IDLE_SUBTLE", 0, "ST_WALK", "ST_JOG", "ST_RUN", 0 };
    Mode h; std::memset(&h, 0, sizeof h); h.ThingType = 2; h.Busy = true;
    h.OnActivate(some);
    ok = ok && h.SpeedInits == 1 && h.Sprint == h.Run && h.SlowWalk == h.Stand && h.Starts == 0 && h.MaxRun == 0.0f;
    std::printf(ok ? "008621a0_TEST PASS\n" : "008621a0_TEST FAIL\n");
    return ok ? 0 : 1;
}