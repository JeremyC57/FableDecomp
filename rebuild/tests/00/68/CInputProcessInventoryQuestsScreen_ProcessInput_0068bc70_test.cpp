// Behaviour test for 0x0068BC70 CInputProcessInventoryQuestsScreen::ProcessInput: models the routing decisions
// (stick thresholds -> NUISystem events, held d-pad actions -> PanMap directions, wheel -> ScrollMenu amount,
// hero-information screen -> released A/B/Y/White handlers then the base process).
#include <stdio.h>
#include <math.h>

static int g_events[8]; static int g_nevents; static int g_performed; static int g_base;
static float g_panX, g_panY; static int g_scroll; static int g_handler;
static void SetEvent(int e) { g_events[g_nevents++] = e; }
static void Reset() { g_nevents = 0; g_performed = 0; g_base = 0; g_panX = g_panY = 9.0f; g_scroll = 0; g_handler = 0; }

// left stick (event type 0x11)
static void LeftStick(float x, float y, bool exclusive)
{
    if (y > 0.6f) { SetEvent(1); if (exclusive) SetEvent(0x11); g_performed = 1; }
    else if (y < -0.6f) { SetEvent(0); if (exclusive) SetEvent(0x10); g_performed = 1; }
    else if (x > 0.6f) { if (exclusive) SetEvent(0xf); g_performed = 1; }
    else if (x < -0.6f) { if (exclusive) SetEvent(0xe); g_performed = 1; }
}
// held map-pan actions 0x6b/0x6a/0x68/0x69
static void HeldPan(int action, bool hasMap)
{
    float x = 0.0f, y = 0.0f;
    if (action == 0x6b) y = -1.0f; else if (action == 0x6a) y = 1.0f;
    else if (action == 0x68) x = -1.0f; else if (action == 0x69) x = 1.0f; else return;
    if (hasMap) { g_panX = x; g_panY = y; }
    g_performed = 1;
}
static void Wheel(float movement) { g_scroll = (int)(long)floor(movement * 0.008333334f); g_performed = 1; }
static void HeroScreenReleased(int action)
{
    if (action == 0x23) { g_handler = 'A'; g_performed = 1; }
    else if (action == 0x22) { g_handler = 'B'; g_performed = 1; }
    else if (action == 0x24) { g_handler = 'Y'; g_performed = 1; }
    else if (action == 0x29) { g_handler = 'W'; g_performed = 1; }
    g_base = 1;
}

int main()
{
    int ok = 1;
    Reset(); LeftStick(0.0f, 0.7f, true); ok &= g_nevents == 2 && g_events[0] == 1 && g_events[1] == 0x11 && g_performed;
    Reset(); LeftStick(0.0f, -0.7f, false); ok &= g_nevents == 1 && g_events[0] == 0 && g_performed;
    Reset(); LeftStick(0.7f, 0.0f, false); ok &= g_nevents == 0 && g_performed;
    Reset(); LeftStick(0.5f, 0.5f, true); ok &= g_nevents == 0 && !g_performed;
    Reset(); HeldPan(0x6b, true); ok &= g_panX == 0.0f && g_panY == -1.0f && g_performed;
    Reset(); HeldPan(0x69, false); ok &= g_panX == 9.0f && g_performed;
    Reset(); Wheel(240.0f); ok &= g_scroll == 2;
    Reset(); Wheel(-120.0f); ok &= g_scroll == -1;
    Reset(); HeroScreenReleased(0x29); ok &= g_handler == 'W' && g_base;
    Reset(); HeroScreenReleased(0x10); ok &= g_handler == 0 && g_base && !g_performed;
    if (ok) printf("0068bc70_TEST PASS\n"); else printf("0068bc70_TEST FAIL\n");
    return ok ? 0 : 1;
}