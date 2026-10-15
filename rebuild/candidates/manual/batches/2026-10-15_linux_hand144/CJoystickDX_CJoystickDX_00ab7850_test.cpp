// Behaviour test for 0x00ab7850 (CJoystickDX::CJoystickDX): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CInputManagerDX { int id; };
struct CJoystick { void* mgr; bool excl; CJoystick(void* m, bool e) : mgr(m), excl(e) {} };
struct CJoystickDX : CJoystick {
    float XAxis, YAxis, XAxis2, YAxis2; long XAxisMin, XAxisMax, YAxisMin, YAxisMax; void* DJoystick; bool Acquired; CInputManagerDX* InputManagerDX;
    int exclusiveCalls; bool lastExclusive;
    bool SetExclusiveMode(bool e) { ++exclusiveCalls; lastExclusive = e; return true; }
    CJoystickDX(CInputManagerDX* m, bool e) : CJoystick(m, e), exclusiveCalls(0), lastExclusive(false)
    {
        InputManagerDX = m; DJoystick = 0;
        XAxisMin = 0x5556; XAxisMax = 0xaaaa; YAxisMin = 0x5556; YAxisMax = 0xaaaa;
        XAxis = 0.0f; YAxis = 0.0f; XAxis2 = 0.0f; YAxis2 = 0.0f;
        SetExclusiveMode(e);
    }
};
static void Run()
{
    CInputManagerDX m; m.id = 2;
    CJoystickDX j(&m, true);
    CHECK(j.mgr == &m && j.excl && j.InputManagerDX == &m && j.DJoystick == 0);
    CHECK(j.XAxisMin == 0x5556 && j.XAxisMax == 0xaaaa && j.YAxisMin == 0x5556 && j.YAxisMax == 0xaaaa);
    CHECK(j.XAxis == 0.0f && j.YAxis == 0.0f && j.XAxis2 == 0.0f && j.YAxis2 == 0.0f);
    CHECK(j.exclusiveCalls == 1 && j.lastExclusive);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00ab7850_TEST FAIL\n"); return 1; }
    std::printf("00ab7850_TEST PASS\n");
    return 0;
}
