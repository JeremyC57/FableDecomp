// Behaviour test for 00702c60: body copied from src/00702c60.cpp (CThingObject::DrawIsDrawable), base test / engine / DivideByGameSpeed stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static float g_gameSpeed; static int g_divCalls; static float g_divArg;
struct CDisplayEngine {
    static float __stdcall DivideByGameSpeed(float value) { ++g_divCalls; g_divArg = value; return value / g_gameSpeed; }
};
static bool g_baseDrawable; static int g_engineCalls; static CDisplayEngine g_engine;
struct CThing {
    virtual void v00() {}
    CDisplayEngine* GetDisplayEngine() { ++g_engineCalls; return &g_engine; }
    virtual bool DrawIsDrawable() { return g_baseDrawable; }
};
struct CThingObject : CThing {
    unsigned char _pad04[0xd0];
    long          DieOffTimer;
    bool          DiesOff;
    virtual bool DrawIsDrawable();
};
bool CThingObject::DrawIsDrawable()
{
    if (CThing::DrawIsDrawable()) {
        if (DiesOff && GetDisplayEngine()->DivideByGameSpeed((float)DieOffTimer) < 5.0f)
            return (DieOffTimer & 1) ? true : false;
        return true;
    }
    return false;
}
int main()
{
    CThingObject o; memset(&o, 0, sizeof(o)); new (&o) CThingObject;
    g_gameSpeed = 1.0f;
    // 1. base says not drawable: false, nothing else consulted
    g_baseDrawable = false; o.DiesOff = true; o.DieOffTimer = 1; g_divCalls = 0; g_engineCalls = 0;
    CHECK(o.DrawIsDrawable() == false); CHECK(g_divCalls == 0);
    g_baseDrawable = true;
    // 2. not dying off: drawable, no speed query
    o.DiesOff = false; CHECK(o.DrawIsDrawable() == true); CHECK(g_divCalls == 0);
    o.DiesOff = true;
    // 3. dying off but plenty of time (10 / 1.0 >= 5): drawable
    o.DieOffTimer = 10; CHECK(o.DrawIsDrawable() == true); CHECK(g_divCalls == 1); CHECK(g_divArg == 10.0f); CHECK(g_engineCalls == 1);
    // 4. under 5 ticks: blink -- odd ticks drawn, even ticks hidden
    o.DieOffTimer = 3; CHECK(o.DrawIsDrawable() == true);
    o.DieOffTimer = 4; CHECK(o.DrawIsDrawable() == false);
    o.DieOffTimer = 1; CHECK(o.DrawIsDrawable() == true);
    o.DieOffTimer = 0; CHECK(o.DrawIsDrawable() == false);
    // 5. game speed scales the remaining time: 10 ticks at speed 4 -> 2.5 < 5: blink (even -> hidden)
    g_gameSpeed = 4.0f; o.DieOffTimer = 10; CHECK(o.DrawIsDrawable() == false);
    o.DieOffTimer = 11; CHECK(o.DrawIsDrawable() == true);
    // 6. exactly 5 is not under 5
    g_gameSpeed = 1.0f; o.DieOffTimer = 5; CHECK(o.DrawIsDrawable() == true);
    if (fails == 0) printf("00702c60_TEST PASS\n");
    else printf("00702c60_TEST FAIL (%d)\n", fails);
    return fails;
}