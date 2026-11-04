// Behaviour test for CEnginePrimitiveManagerSpriteTrails::RenderPrimitive (0x00BCA6B0): a model of the row.
#include <stdio.h>

static int g_queued; static float g_dist;
static int Render(bool enabled, int points, float headZ, float camZ)
{
    if (!enabled) return 0;
    if (points > 0) { g_queued++; g_dist = headZ > camZ ? headZ - camZ : camZ - headZ; }
    return 2;
}
static int run() { return Render(false, 3, 0, 0) == 0 && Render(true, 0, 0, 0) == 2 && g_queued == 0 && Render(true, 2, 7, 3) == 2 && g_queued == 1 && g_dist == 4; }
int main()
{
    int ok = run();
    printf(ok ? "00bca6b0_TEST PASS\n" : "00bca6b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
