// Test for MixChannels_00c77d90 0x00C77D90: stereo gives the (1-x)/2, (1+x)/2 pair; four channels give inverse-distance
// weights to the four 0.707 corner speakers, normalised to sum 1, with a coincident speaker taking the whole gain.
#include <cstdio>
#include <cmath>
static void MixChannels(const float* position, float* gains, unsigned long count, unsigned long channels)
{
    if (count < channels) channels = count;
    float x = position[0], z = position[2];
    if (channels == 2) {
        float left = (1.0f - x) * 0.5f, right = (x + 1.0f) * 0.5f;
        for (unsigned long i = 0; i < count; i += 2) { gains[i] = left; gains[i + 1] = right; }
        return;
    }
    float xm = 0.707f - x, xp = x + 0.707f, zm = 0.707f - z, zp = z + 0.707f;
    float d0 = zm * zm + xp * xp, d1 = xm * xm + zm * zm, d2 = zp * zp + xp * xp, d3 = zp * zp + xm * xm;
    float g0 = 0.0f, g1 = 0.0f, g2 = 0.0f, g3 = 0.0f;
    if (d0 == 0.0f) g0 = 1.0f; else if (d1 == 0.0f) g1 = 1.0f; else if (d2 == 0.0f) g2 = 1.0f; else if (d3 == 0.0f) g3 = 1.0f;
    else {
        float w0 = 1.0f / d0, w1 = 1.0f / d1, w2 = 1.0f / d2, w3 = 1.0f / d3;
        float norm = 1.0f / (w3 + w2 + w1 + w0);
        g0 = norm * w0; g1 = norm * w1; g2 = norm * w2; g3 = norm * w3;
    }
    for (unsigned long i = 0; i < count; i += channels) { gains[i] = g0; gains[i + 1] = g1; gains[i + 2] = g2; gains[i + 3] = g3; }
}
static bool near(float a, float b) { return fabs(a - b) < 1e-5f; }
int main() {
    float pos[3] = { 0.5f, 0.0f, 0.0f }; float g[8];
    MixChannels(pos, g, 4, 2);
    bool ok = near(g[0], 0.25f) && near(g[1], 0.75f) && near(g[2], 0.25f) && near(g[3], 0.75f);
    float centre[3] = { 0.0f, 0.0f, 0.0f };
    MixChannels(centre, g, 4, 4);
    ok = ok && near(g[0], 0.25f) && near(g[1], 0.25f) && near(g[2], 0.25f) && near(g[3], 0.25f);
    float corner[3] = { -0.707f, 0.0f, 0.707f };   // on speaker 0 (xp == 0, zm == 0)
    MixChannels(corner, g, 8, 4);
    ok = ok && g[0] == 1.0f && g[1] == 0.0f && g[2] == 0.0f && g[3] == 0.0f && g[4] == 1.0f && g[7] == 0.0f;
    if (ok) printf("00c77d90_TEST PASS\n"); else printf("00c77d90_TEST FAIL\n");
    return ok ? 0 : 1;
}