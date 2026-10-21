// Behaviour test for CAIStateGroup_SleepOnFloor::OnReplace (0x00969AE0): same logic over mocks.
#include <cstdio>
#include <cstring>
struct CThing { int id; };
struct CPoint { CThing* occupant; };
struct CState {
    CThing* creature; CPoint* point; CThing* PSleepPoint; bool Asleep; bool freeToAct;
    const char* playedAnim; int outputsSet; unsigned long lastFlags; int modeRemoved;
    void OnReplace()
    {
        if (point->occupant == creature) point->occupant = 0;
        PSleepPoint = 0;
        if (Asleep) {
            const char* anim = "ST_SLEEP_ON_FLOOR_OUTOF";
            unsigned long flags = 0;
            if (freeToAct) { flags |= 8; playedAnim = anim; lastFlags = flags; ++outputsSet; }
            modeRemoved = 0xd;
        }
    }
};
int main()
{
    CThing me = { 1 }, other = { 2 }, bed = { 3 };
    CPoint p; p.occupant = &me;
    CState s; s.creature = &me; s.point = &p; s.PSleepPoint = &bed; s.Asleep = true; s.freeToAct = true;
    s.playedAnim = 0; s.outputsSet = 0; s.lastFlags = 0; s.modeRemoved = 0;
    s.OnReplace();
    bool ok = p.occupant == 0 && s.PSleepPoint == 0 && s.outputsSet == 1 && s.lastFlags == 8 && s.modeRemoved == 0xd
        && std::strcmp(s.playedAnim, "ST_SLEEP_ON_FLOOR_OUTOF") == 0;
    p.occupant = &other; s.PSleepPoint = &bed; s.freeToAct = false; s.modeRemoved = 0; s.OnReplace();
    ok = ok && p.occupant == &other && s.PSleepPoint == 0 && s.outputsSet == 1 && s.modeRemoved == 0xd;
    s.Asleep = false; s.modeRemoved = 0; s.OnReplace();
    ok = ok && s.modeRemoved == 0;
    if (!ok) { std::printf("00969ae0_TEST FAIL\n"); return 1; }
    std::printf("00969ae0_TEST PASS\n");
    return 0;
}