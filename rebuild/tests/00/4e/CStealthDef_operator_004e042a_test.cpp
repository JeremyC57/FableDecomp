// behaviour test for 0x004E042A CStealthDef::operator=: the base part is assigned first (once), then every member (DistanceMovementCanBeHeardFrom, RunningNoiseMultiplier, JoggingNoiseMultiplier, WalkingNoiseMultiplier, DayTimeLampVisibilityMultiplier, NightTimeLampVisibilityMultiplier, DangerFeedbackHangtimeSeconds, RunningAwayFromCombatHangtimeSeconds)
// is copied, and *this is returned; the base's own bytes are left to the base operator=
#include <cstdio>
#include <cstring>
static int gBaseCalls = 0;
struct Vec2 {
    float X, Y;
    Vec2() : X(0.0f), Y(0.0f) { }
    Vec2(float x, float y) : X(x), Y(y) { }
    bool operator==(const Vec2& o) const { return X == o.X && Y == o.Y; }
};
struct Base {
    int Tag;
    Base() : Tag(0) { }
    Base& operator=(const Base&) { ++gBaseCalls; return *this; }
};
struct Def : public Base {
    float DistanceMovementCanBeHeardFrom;
    float RunningNoiseMultiplier;
    float JoggingNoiseMultiplier;
    float WalkingNoiseMultiplier;
    float DayTimeLampVisibilityMultiplier;
    float NightTimeLampVisibilityMultiplier;
    float DangerFeedbackHangtimeSeconds;
    float RunningAwayFromCombatHangtimeSeconds;
};
int main()
{
    int fail = 0;
    Def a, b;
    std::memset(&a, 0, sizeof(a));
    std::memset(&b, 0, sizeof(b));
    a.Tag = 11; b.Tag = 22;
    b.DistanceMovementCanBeHeardFrom = 1.5f + 0;
    b.RunningNoiseMultiplier = 1.5f + 1;
    b.JoggingNoiseMultiplier = 1.5f + 2;
    b.WalkingNoiseMultiplier = 1.5f + 3;
    b.DayTimeLampVisibilityMultiplier = 1.5f + 4;
    b.NightTimeLampVisibilityMultiplier = 1.5f + 5;
    b.DangerFeedbackHangtimeSeconds = 1.5f + 6;
    b.RunningAwayFromCombatHangtimeSeconds = 1.5f + 7;
    Def& r = (a = b);
    if (&r != &a) fail |= 1;
    if (gBaseCalls != 1) fail |= 2;
    if (!(a.DistanceMovementCanBeHeardFrom == b.DistanceMovementCanBeHeardFrom) || !(a.RunningNoiseMultiplier == b.RunningNoiseMultiplier) || !(a.JoggingNoiseMultiplier == b.JoggingNoiseMultiplier) || !(a.WalkingNoiseMultiplier == b.WalkingNoiseMultiplier) || !(a.DayTimeLampVisibilityMultiplier == b.DayTimeLampVisibilityMultiplier) || !(a.NightTimeLampVisibilityMultiplier == b.NightTimeLampVisibilityMultiplier) || !(a.DangerFeedbackHangtimeSeconds == b.DangerFeedbackHangtimeSeconds) || !(a.RunningAwayFromCombatHangtimeSeconds == b.RunningAwayFromCombatHangtimeSeconds)) fail |= 4;
    if (a.Tag != 11) fail |= 8;
    if (fail) { std::printf("004e042a_TEST FAIL %d\n", fail); return 1; }
    std::printf("004e042a_TEST PASS\n");
    return 0;
}