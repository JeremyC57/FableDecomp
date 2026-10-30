// Behaviour test for 0x00752120 CTCLook::ChangeHeadAngleXY (see src/00752120.cpp): the integrator receives the XY angle and
// velocity by reference, acceleration * time, (target-acquired ? fast : slow) speed * time, the target, and half the def's XY
// range in turns as both limits.
#include <cstdio>
#include <cmath>
struct Pair { float XY, YZ; };
static Pair fast = { 2.0f, 0.0f }, slow = { 1.0f, 0.0f }, accel = { 4.0f, 0.0f };
struct Call { float* angle; float* vel; float a, s, t, lo, hi; };
static Call last;
struct Look {
    float rangeDeg; float angleXY; float velXY; bool target;
    float Speed() { return target ? fast.XY : slow.XY; }
    float Range() const { return 0.5f * (rangeDeg * 0.0027777778f); }
    void Settle(float& an, float& v, float a, float s, float t, float lo, float hi) const { last.angle = &an; last.vel = &v; last.a = a; last.s = s; last.t = t; last.lo = lo; last.hi = hi; }
    void Change(float target_, float time) { Settle(angleXY, velXY, accel.XY * time, Speed() * time, target_, Range(), Range()); }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    Look l = { 180.0f, 0.0f, 0.0f, true };
    l.Change(0.1f, 0.5f);
    if (last.angle != &l.angleXY || last.vel != &l.velXY || !Near(last.a, 2.0f) || !Near(last.s, 1.0f) || !Near(last.t, 0.1f)
        || !Near(last.lo, 0.25f) || !Near(last.hi, 0.25f)) { std::printf("00752120_TEST FAIL\n"); return 1; }
    l.target = false;
    l.Change(-0.2f, 1.0f);
    if (!Near(last.s, 1.0f) || !Near(last.a, 4.0f) || !Near(last.t, -0.2f)) { std::printf("00752120_TEST FAIL\n"); return 1; }
    std::printf("00752120_TEST PASS\n");
    return 0;
}