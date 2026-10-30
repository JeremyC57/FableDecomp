// Behaviour test for libvorbis psy.c seed_loop (0x00C92E20): per-octave-group peak seeds a curve when above the floor.
#include <cstdio>
struct look { int n; long* octave; long firstoc, shiftoc; int eighth_octave_lines, total_octave_lines; float max_curve_dB; };
static int g_calls; static float g_amp; static int g_oc;
static void seed_curve(float*, const float**, float amp, int oc, int, int, float) { g_calls++; g_amp = amp; g_oc = oc; }
static void seed_loop(look* p, const float*** curves, const float* f, const float* flr, float* seed, float specmax)
{
    long n = p->n, i; float dBoffset = p->max_curve_dB - specmax;
    for (i = 0; i < n; i++) {
        float mx = f[i]; long oc = p->octave[i];
        while (i + 1 < n && p->octave[i + 1] == oc) { i++; if (f[i] > mx) mx = f[i]; }
        if (mx + 6.f > flr[i]) {
            oc = oc >> p->shiftoc; if (oc >= 17) oc = 16; if (oc < 0) oc = 0;
            seed_curve(seed, curves[oc], mx, p->octave[i] - p->firstoc, p->total_octave_lines, p->eighth_octave_lines, dBoffset);
        }
    }
}
int main()
{
    long oct[4] = { 10, 10, 20, 20 }; float f[4] = { 1, 5, 2, 3 }, flr[4] = { 0, 0, 0, 100 };
    static const float** curves[17]; look p = { 4, oct, 2, 0, 8, 64, 0 };
    seed_loop(&p, curves, f, flr, 0, 0);
    if (g_calls != 1 || g_amp != 5.0f || g_oc != 8) { std::printf("00c92e20_TEST FAIL\n"); return 1; }
    std::printf("00c92e20_TEST PASS\n"); return 0;
}