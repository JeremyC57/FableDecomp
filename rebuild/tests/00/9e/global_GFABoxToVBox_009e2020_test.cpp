// Behaviour test for GFABoxToVBox (0x009E2020): each corner of an actual-screen box is scaled into virtual-screen units,
// X by VIRTUAL_SCREEN_WIDTH / render-target width and Y by VIRTUAL_SCREEN_HEIGHT / render-target height.
#include <stdio.h>
#include <math.h>

class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
static long g_targetX = 1280, g_targetY = 960;
static const double VIRTUAL_SCREEN_WIDTH = 640.0, VIRTUAL_SCREEN_HEIGHT = 480.0;
static float GFAXToVX(float x) { return ((float)VIRTUAL_SCREEN_WIDTH * x) / (float)g_targetX; }
static float GFAYToVY(float y) { return (float)((float)VIRTUAL_SCREEN_HEIGHT * y) / (float)g_targetY; }
static void GFABoxToVBox(const C2DBoxF& a, C2DBoxF& v)
{
    v.TLX = GFAXToVX(a.TLX);
    v.TLY = GFAYToVY(a.TLY);
    v.BRX = GFAXToVX(a.BRX);
    v.BRY = GFAYToVY(a.BRY);
}
static bool Near(float a, float b) { return fabs(a - b) < 0.001f; }
int main()
{
    int fails = 0;
    C2DBoxF a = { 100.0f, 200.0f, 1280.0f, 960.0f }, v;
    GFABoxToVBox(a, v);
    if (!Near(v.TLX, 50.0f) || !Near(v.TLY, 100.0f) || !Near(v.BRX, 640.0f) || !Near(v.BRY, 480.0f)) { printf("half scale FAIL\n"); ++fails; }
    g_targetX = 640; g_targetY = 240;
    GFABoxToVBox(a, v);
    if (!Near(v.TLX, 100.0f) || !Near(v.TLY, 400.0f) || !Near(v.BRX, 1280.0f) || !Near(v.BRY, 1920.0f)) { printf("aniso FAIL\n"); ++fails; }
    if (fails) { printf("009e2020_TEST FAIL\n"); return 1; }
    printf("009e2020_TEST PASS\n");
    return 0;
}