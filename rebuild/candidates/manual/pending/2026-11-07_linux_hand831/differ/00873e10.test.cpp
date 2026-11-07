// Behaviour test for 0x00873E10 NCameraModes::CPCMouseLook::Update: models the mouse smoothing (average of the last N samples,
// oldest dropped when the list is full), the sensitivity (profile value clamped to [0,1] plus 0.5), the dolly smoothing
// ((s * len + target) / (s + 1)), the recentre rule (inside +-0.375 turn the axis snaps to the facing rotated by +-0.375, sign from the
// angle), the auto-pitch blend ((pitch * 10 + target) / 11) and the pitch clamp to [max - 0.25, 0.25 - min] in turns.
#include <stdio.h>
#include <math.h>

static float Average(const float* in, int n, int samples, float next)
{
    float buf[16]; int count = n;
    for (int i = 0; i < n; ++i) buf[i] = in[i];
    if (count == samples) { for (int i = 1; i < count; ++i) buf[i - 1] = buf[i]; --count; }
    buf[count++] = next;
    float sum = 0.0f; for (int i = 0; i < count; ++i) sum += buf[i];
    return (1.0f / (float)samples) * sum;
}
static float Sensitivity(float profile) { if (profile < 0.0f) profile = 0.0f; else if (profile > 1.0f) profile = 1.0f; return profile + 0.5f; }
static float Dolly(float s, float len, float target) { return (s * len + target) / (s + 1.0f); }
static bool Recentre(float angle, float& offset)
{
    if (!(fabs(angle) < 0.375f)) return false;
    long sign = -1; if (!(angle < 0.0f)) sign = 1;
    offset = (float)sign * 0.375f; return true;
}
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }   // x87 /Od keeps extended precision: no exact float ==
static float ClampPitch(float pitch, float minPitch, float maxPitch)
{
    if (pitch < maxPitch - 0.25f) return maxPitch - 0.25f;
    if (pitch > 0.25f - minPitch) return 0.25f - minPitch;
    return pitch;
}

int main()
{
    int ok = 1;
    float in[3] = { 1.0f, 2.0f, 3.0f };
    ok &= fabs(Average(in, 3, 3, 6.0f) - (2.0f + 3.0f + 6.0f) / 3.0f) < 1e-5f;
    ok &= fabs(Average(in, 2, 4, 5.0f) - (1.0f + 2.0f + 5.0f) / 4.0f) < 1e-5f;
    ok &= Near(Sensitivity(-1.0f), 0.5f) && Near(Sensitivity(2.0f), 1.5f) && Near(Sensitivity(0.25f), 0.75f);
    ok &= fabs(Dolly(3.0f, 10.0f, 2.0f) - 8.0f) < 1e-5f;
    float off = 0.0f;
    ok &= Recentre(0.1f, off) && Near(off, 0.375f);
    ok &= Recentre(-0.2f, off) && Near(off, -0.375f);
    ok &= !Recentre(0.4f, off);
    ok &= fabs((0.0f * 10.0f + 0.125f) * (1.0f / 11.0f) - 0.0113636f) < 1e-5f;
    ok &= Near(ClampPitch(-0.3f, 0.083333336f, 0.22222222f), 0.22222222f - 0.25f);
    ok &= Near(ClampPitch(0.3f, 0.083333336f, 0.22222222f), 0.25f - 0.083333336f);
    ok &= Near(ClampPitch(0.0f, 0.083333336f, 0.22222222f), 0.0f);
    if (ok) printf("00873e10_TEST PASS\n"); else printf("00873e10_TEST FAIL\n");
    return ok ? 0 : 1;
}
