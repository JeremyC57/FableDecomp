// Behaviour test for 0x0075f1f0 (CKeyCamera::CKeyCamera(const CKeyCamera&)): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CCharString { char text[16]; CCharString() { text[0] = 0; } CCharString& operator=(const CCharString& o) { std::strcpy(text, o.text); return *this; } };
struct C3DVector { float X, Y, Z; };
struct CKeyCamera {
    float DeltaIndex; C3DVector Pos; C3DVector LookDirection; float FOV, Roll, ShuttleSpeed, Duration, PauseTime; CCharString EventName; float SpeedFraction;
    CKeyCamera() {}
    CKeyCamera(const CKeyCamera& o)
    {
        Pos = o.Pos; LookDirection = o.LookDirection; FOV = o.FOV; ShuttleSpeed = o.ShuttleSpeed; Duration = o.Duration;
        PauseTime = o.PauseTime; EventName = o.EventName; Roll = o.Roll; SpeedFraction = o.SpeedFraction; DeltaIndex = o.DeltaIndex;
    }
};
static void Run()
{
    CKeyCamera a; a.DeltaIndex = 1; a.Pos.X = 2; a.Pos.Y = 3; a.Pos.Z = 4; a.LookDirection.X = 5; a.LookDirection.Y = 6; a.LookDirection.Z = 7;
    a.FOV = 8; a.Roll = 9; a.ShuttleSpeed = 10; a.Duration = 11; a.PauseTime = 12; std::strcpy(a.EventName.text, "EVT"); a.SpeedFraction = 13;
    CKeyCamera b(a);
    CHECK(b.DeltaIndex == 1 && b.Pos.Z == 4 && b.LookDirection.X == 5 && b.FOV == 8 && b.Roll == 9 && b.ShuttleSpeed == 10);
    CHECK(b.Duration == 11 && b.PauseTime == 12 && b.SpeedFraction == 13 && std::strcmp(b.EventName.text, "EVT") == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0075f1f0_TEST FAIL\n"); return 1; }
    std::printf("0075f1f0_TEST PASS\n");
    return 0;
}
