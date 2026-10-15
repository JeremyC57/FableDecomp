// Behaviour test for CKeyCamera::operator= @ 0x0075EF00.
#include <cstdio>
static int g_stringAssigns = 0;
struct CCharString { const char* p; CCharString& operator=(const CCharString& o) { p = o.p; ++g_stringAssigns; return *this; } };
struct C3DVector { float X, Y, Z; };
struct CKeyCamera {
    float DeltaIndex; C3DVector Pos, LookDirection; float FOV, Roll, ShuttleSpeed, Duration, PauseTime; CCharString EventName; float SpeedFraction;
    CKeyCamera& operator=(const CKeyCamera& o)
    {
        DeltaIndex = o.DeltaIndex; Pos = o.Pos; LookDirection = o.LookDirection; FOV = o.FOV; Roll = o.Roll;
        ShuttleSpeed = o.ShuttleSpeed; Duration = o.Duration; PauseTime = o.PauseTime; EventName = o.EventName;
        SpeedFraction = o.SpeedFraction; return *this;
    }
};
int main()
{
    CKeyCamera a, b;
    a.DeltaIndex = 1; a.Pos.X = 2; a.Pos.Y = 3; a.Pos.Z = 4; a.LookDirection.X = 5; a.LookDirection.Y = 6; a.LookDirection.Z = 7;
    a.FOV = 8; a.Roll = 9; a.ShuttleSpeed = 10; a.Duration = 11; a.PauseTime = 12; a.EventName.p = "EV"; a.SpeedFraction = 13;
    b.EventName.p = 0;
    CKeyCamera& r = (b = a);
    int ok = &r == &b && b.DeltaIndex == 1 && b.Pos.X == 2 && b.Pos.Z == 4 && b.LookDirection.Y == 6 && b.FOV == 8 && b.Roll == 9
          && b.ShuttleSpeed == 10 && b.Duration == 11 && b.PauseTime == 12 && b.EventName.p == a.EventName.p && b.SpeedFraction == 13
          && g_stringAssigns == 1;
    if (!ok) { std::printf("0075ef00_TEST FAIL\n"); return 1; }
    std::printf("0075ef00_TEST PASS\n");
    return 0;
}