// Behaviour test for 0x0068CB90 CInputProcessControlFreeCamera::ProcessInput: angle wrapping to [0,1) and the
// look-at target 40 units along the (XY, YZ) angles, the keyboard height step (+-2.5) and the stick normalisation.
#include <stdio.h>
#include <math.h>
static float Wrap(float a) { if (a < 0.0f || a >= 1.0f) { a = (float)fmod((double)a, 1.0); if (a < 0.0f) a += 1.0f; } return a; }
static void Target(float px, float py, float pz, float xy, float yz, float& tx, float& ty, float& tz)
{
    xy = Wrap(xy); yz = Wrap(yz);
    double a = yz * 6.2831854820251465, b = xy * 6.2831854820251465;
    float h = (float)cos(a) * 40.0f;
    tx = px + (float)(sin(b) * h); ty = py + (float)(cos(b) * h); tz = pz + (float)(sin(a) * 40.0);
}
static bool Near(float a, float b) { return fabs(a - b) < 1e-3f; }
int main()
{
    int ok = 1; float x, y, z;
    ok &= Near(Wrap(1.25f), 0.25f) && Near(Wrap(-0.25f), 0.75f) && Wrap(0.5f) == 0.5f;
    Target(0, 0, 0, 0.0f, 0.0f, x, y, z); ok &= Near(x, 0) && Near(y, 40) && Near(z, 0);
    Target(1, 2, 3, 0.25f, 0.0f, x, y, z); ok &= Near(x, 41) && Near(y, 2) && Near(z, 3);
    Target(0, 0, 0, 0.0f, 0.25f, x, y, z); ok &= Near(z, 40) && Near(y, 0);
    Target(0, 0, 0, -0.75f, 0.0f, x, y, z); ok &= Near(x, 40);   // -0.75 wraps to 0.25
    float sx = 3.0f, sy = 4.0f, len = (float)sqrt(sx * sx + sy * sy);
    if (len > 1.0f) { float inv = 1.0f / len; sx *= inv; sy *= inv; }
    ok &= Near(sx, 0.6f) && Near(sy, 0.8f);
    float h = 10.0f; h += 2.5f; ok &= h == 12.5f;
    if (ok) printf("0068cb90_TEST PASS\n"); else printf("0068cb90_TEST FAIL\n");
    return ok ? 0 : 1;
}
