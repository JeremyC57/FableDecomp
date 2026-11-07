// Behaviour test for 0x008DBC80 NCreatureMode::CModeDragonFlying::FrameUpdate: models the spline progress ((loops + fraction) / loops
// to do, remapped past the first segment on the first frame, capped at 1), the angle wrap GET_ROUNDED_ANGLE into [0,1), the bank
// (signed angle difference * 3 clamped to +-0.125 turn), the leg-anim alpha (|turn| * 8, zeroed late in the last loop, eased by 0.1),
// the look-ahead spline position ((1 - p) * 0.5 + p) and the end test (fraction > 0.999 and ++loops >= loops to do).
#include <stdio.h>
#include <math.h>

static float Progress(long loops, float fraction, long todo, bool firstFrame, long points)
{
    float p = ((float)loops + fraction) / (float)todo;
    if (firstFrame) {
        float first = 1.0f / ((float)points - 1.0f);
        p = (1.0f - first) * p + first;
    }
    return p > 1.0f ? 1.0f : p;
}
static float Wrap(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)fmod(a, 1.0f); if (a < 0.0f) a += 1.0f; } return a; }
static float Bank(float diff) { float t = diff * 3.0f; if (t > 0.125f) t = 0.125f; else if (t < -0.125f) t = -0.125f; return t; }
static float LegAlpha(float turn, float fraction, bool lastLoop, float previous)
{
    float a = (float)fabs(turn) * 8.0f;
    if (fraction > 0.75f && lastLoop) a = 0.0f;
    return (a - previous) * 0.1f + previous;
}

int main()
{
    int ok = 1;
    ok &= fabs(Progress(1, 0.5f, 3, false, 5) - 0.5f) < 1e-6f;
    ok &= fabs(Progress(0, 0.0f, 4, true, 5) - 0.25f) < 1e-6f;
    ok &= Progress(5, 0.9f, 3, false, 5) == 1.0f;
    ok &= fabs(Wrap(-0.25f) - 0.75f) < 1e-6f && fabs(Wrap(1.5f) - 0.5f) < 1e-6f && Wrap(0.3f) == 0.3f;
    ok &= Bank(0.1f) == 0.125f && Bank(-0.1f) == -0.125f && fabs(Bank(0.01f) - 0.03f) < 1e-6f;
    ok &= fabs(LegAlpha(0.1f, 0.5f, true, 0.0f) - 0.08f) < 1e-6f;
    ok &= fabs(LegAlpha(0.1f, 0.8f, true, 0.5f) - 0.45f) < 1e-6f;
    float p = 0.6f;
    ok &= fabs(((1.0f - p) * 0.5f + p) - 0.8f) < 1e-6f;
    long loops = 2, todo = 3;
    ok &= (0.9995f > 0.999f && ++loops >= todo) && loops == 3;
    if (ok) printf("008dbc80_TEST PASS\n"); else printf("008dbc80_TEST FAIL\n");
    return ok ? 0 : 1;
}
