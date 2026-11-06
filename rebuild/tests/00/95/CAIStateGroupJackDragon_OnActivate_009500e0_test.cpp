// Behaviour test for CAIStateGroup_JackDragon::OnActivate (0x009500E0): a model of the activation -- gravity off, phase changes
// and targeting on, the state / timers reset (both "last" frames from one world-frame read), the dragon placed on the landed
// marker only while that marker is alive with physics, the into-ground offsets rotated by the marker's facing, the hover-relative
// out-of-ground Y values, and DragonPosIntoGround3 = rotated offset + marker position + rotated back offset + (0, 0, bodge).
#include <stdio.h>
#include <math.h>
struct V { float X, Y, Z; };
static V Add(V a, V b) { V r = { a.X + b.X, a.Y + b.Y, a.Z + b.Z }; return r; }
static V RotateXY(V v, float a) { V r = { v.X * cosf(a) - v.Y * sinf(a), v.X * sinf(a) + v.Y * cosf(a), v.Z }; return r; }
struct Dragon {
    bool Gravity, PhaseChange, Targetable; int State, GoLeft, FramesInSnapRange; bool BattleTriggered;
    long LastInRange, LastSlam, Landed; V Pos; float Facing; V IntoGround3, OutOfGround4Right, OutOfGround4Left;
};
static void OnActivate(Dragon& d, bool markerAlive, bool markerPhysics, V markerPos, float markerFacing, float hoverY,
                       float bodge, long frame)
{
    d.PhaseChange = true; d.Gravity = false;
    d.State = 1; d.GoLeft = 0; d.BattleTriggered = false; d.FramesInSnapRange = 0;
    d.LastInRange = d.LastSlam = frame; d.Landed = frame; d.Targetable = true;
    V offset = { 0.0f, -37.93019f, 30.148378f }, back = { 0.0f, -80.0f, 30.0f };
    if (markerAlive && markerPhysics) {
        offset = RotateXY(offset, markerFacing); back = RotateXY(back, markerFacing);
        d.Pos = markerPos; d.Facing = markerFacing;
    }
    V r4 = { 1394.0f, hoverY + 20.0f, 185.14838f }, l4 = { 1544.0f, hoverY + 40.0f, 185.14838f };
    d.OutOfGround4Right = r4; d.OutOfGround4Left = l4;
    V up = { 0.0f, 0.0f, bodge };
    d.IntoGround3 = Add(Add(Add(offset, markerPos), back), up);
}
static bool Near(float a, float b) { return fabsf(a - b) < 1e-3f; }
int main()
{
    int ok = 1;
    V m = { 1470.0f, 8000.0f, 100.0f };
    Dragon d = {}; d.Gravity = true; d.State = 7;
    OnActivate(d, true, true, m, 0.0f, 7900.0f, 5.0f, 42);
    ok &= !d.Gravity && d.PhaseChange && d.Targetable && d.State == 1 && d.LastInRange == 42 && d.LastSlam == 42 && d.Landed == 42;
    ok &= Near(d.IntoGround3.X, 1470.0f) && Near(d.IntoGround3.Y, 8000.0f - 37.93019f - 80.0f) && Near(d.IntoGround3.Z, 165.148378f);
    ok &= Near(d.Pos.X, 1470.0f) && Near(d.OutOfGround4Right.Y, 7920.0f) && Near(d.OutOfGround4Left.Y, 7940.0f);
    Dragon e = {};
    OnActivate(e, true, true, m, 1.5707963f, 7900.0f, 0.0f, 1);
    ok &= Near(e.IntoGround3.X, 1470.0f + 37.93019f + 80.0f) && Near(e.IntoGround3.Y, 8000.0f);
    Dragon f = {};
    OnActivate(f, false, true, m, 1.5707963f, 7900.0f, 0.0f, 1);
    ok &= Near(f.Pos.X, 0.0f) && Near(f.IntoGround3.Y, 8000.0f - 117.93019f);
    printf(ok ? "009500e0_TEST PASS\n" : "009500e0_TEST FAIL\n");
    return ok ? 0 : 1;
}