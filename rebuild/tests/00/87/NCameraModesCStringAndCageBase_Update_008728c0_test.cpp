// Behaviour test for NCameraModes::CStringAndCageBase::Update (0x008728C0): the update order and the Z easing.
#include <cstdio>
struct V3 { float X, Y, Z; };
static int g_log[16]; static int g_n = 0;
struct Cam {
    V3 LookAt, GoTo; float CurrentHeightOffset; bool NoSmoothing; bool autoEnabled; float autoTime, idle;
    V3 focal, thingPos; bool velocityHandled;
    bool UpdatePositionWithVelocity(V3& p) { g_log[g_n++] = 1; if (velocityHandled) p.X += 1.0f; return velocityHandled; }
    void UpdatePosition(V3& p) { g_log[g_n++] = 2; p.X += 2.0f; }
    void UpdateAutoGoBehind(V3& p) { g_log[g_n++] = 3; p.Y += 5.0f; }
    void UpdateXYVars() { g_log[g_n++] = 4; }
    void UpdateZVars() { g_log[g_n++] = 5; }
    void SetGoToPosition(const V3& p) { GoTo = p; }
    void Update()
    {
        V3 goTo = GoTo;
        V3 lookAt = focal;
        if (!UpdatePositionWithVelocity(goTo)) UpdatePosition(goTo);
        if (autoEnabled) { float t = idle; if (autoTime < t) UpdateAutoGoBehind(goTo); }
        UpdateXYVars(); UpdateZVars();
        goTo.Z = thingPos.Z + CurrentHeightOffset;
        bool ns = NoSmoothing;
        float rate = ns ? 0.0f : 10.0f;
        float z = (GoTo.Z * rate + goTo.Z) / (rate + 1.0f);
        goTo.Z = z;
        rate = ns ? 0.0f : 2.0f;
        z = (LookAt.Z * rate + lookAt.Z) / (rate + 1.0f);
        lookAt.Z = z;
        SetGoToPosition(goTo);
        LookAt = lookAt;
    }
};
static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    Cam c;
    c.LookAt.X = 0; c.LookAt.Y = 0; c.LookAt.Z = 3.0f;
    c.GoTo.X = 1; c.GoTo.Y = 2; c.GoTo.Z = 0.0f;
    c.CurrentHeightOffset = 1.0f; c.NoSmoothing = false; c.autoEnabled = true; c.autoTime = 1.0f; c.idle = 2.0f;
    c.focal.X = 7; c.focal.Y = 8; c.focal.Z = 9.0f; c.thingPos.X = 0; c.thingPos.Y = 0; c.thingPos.Z = 10.0f;
    c.velocityHandled = false;
    c.Update();
    // goTo.Z target 11, eased (0*10 + 11)/11 = 1; lookAt Z (3*2 + 9)/3 = 5
    if (g_n != 5 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3 || !near(c.GoTo.X, 3.0f) || !near(c.GoTo.Y, 7.0f)
        || !near(c.GoTo.Z, 1.0f) || !near(c.LookAt.Z, 5.0f) || !near(c.LookAt.X, 7.0f)) { std::printf("008728c0_TEST FAIL\n"); return 1; }
    g_n = 0; c.NoSmoothing = true; c.velocityHandled = true; c.autoEnabled = false;
    c.Update();   // no smoothing: Z snaps to the targets
    if (g_n != 3 || !near(c.GoTo.Z, 11.0f) || !near(c.LookAt.Z, 9.0f) || !near(c.GoTo.X, 4.0f)) { std::printf("008728c0_TEST FAIL\n"); return 1; }
    std::printf("008728c0_TEST PASS\n");
    return 0;
}