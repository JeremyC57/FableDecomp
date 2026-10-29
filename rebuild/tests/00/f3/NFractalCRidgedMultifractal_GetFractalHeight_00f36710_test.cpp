// Behaviour test for 0x00F36710 NFractal::CRidgedMultifractal::GetFractalHeight: ridged multifractal over a scripted noise
// source; checks the octave count (Octaves and the 0.001 weight cut-off), the lacunarity scaling of the sample point, the
// weight clamp and the final 1/3.4 scale clamped to [0, 1].
#include <stdio.h>
#include <math.h>

class C2DVector { public: float X, Y; };
static float g_noise[16]; static int g_calls; static float g_lastX[16];
struct CNoise { float PerlinNoise(const C2DVector& p) const { g_lastX[g_calls] = p.X; return g_noise[g_calls++]; } };
struct CRidged {
    double ExponentArray[128]; double Lacunarity; double Octaves; CNoise* Noise;
    double GetFractalHeight(const C2DVector& pos) const
    {
        const CNoise* noise = Noise;
        C2DVector point = pos;
        double signal = noise->PerlinNoise(point);
        signal = fabs(signal); signal = 1.0 - signal; signal *= signal;
        double result = signal, weight = 1.0;
        for (int i = 1; weight > 0.001 && i < Octaves; i++) {
            point.X *= (float)Lacunarity; point.Y *= (float)Lacunarity;
            weight = signal * 2.0;
            if (signal * 2.0 < 0.0) weight = 0.0; else if (signal * 2.0 > 1.0) weight = 1.0;
            signal = noise->PerlinNoise(point);
            signal = fabs(signal); signal = 1.0 - signal; signal *= signal; signal *= weight;
            result += signal * ExponentArray[i];
        }
        result *= 1.0f / 3.4f;
        if (result < 0.0) return 0.0;
        if (result > 1.0) return 1.0;
        return result;
    }
};
static int Near(double a, double b) { return fabs(a - b) < 1e-5; }

int main()
{
    CNoise n; CRidged f; f.Noise = &n; f.Lacunarity = 2.0; f.Octaves = 3.0;
    for (int i = 0; i < 128; ++i) f.ExponentArray[i] = 0.5;
    C2DVector p; p.X = 1.0f; p.Y = 1.0f;
    g_calls = 0; g_noise[0] = 0.5f; g_noise[1] = -0.25f; g_noise[2] = 0.0f;
    double h = f.GetFractalHeight(p);
    // s0 = 0.25; w1 = 0.5, s1 = 0.5625 * 0.5 = 0.28125; w2 = clamp(0.5625) , s2 = 1 * 0.5625
    double expect = (0.25 + 0.28125 * 0.5 + 0.5625 * 0.5) * (double)(1.0f / 3.4f);
    int ok = g_calls == 3 && Near(h, expect) && g_lastX[1] == 2.0f && g_lastX[2] == 4.0f;
    g_calls = 0; g_noise[0] = 1.0f;                      // signal 0 -> weight 0 stops after one more octave
    h = f.GetFractalHeight(p);
    ok = ok && g_calls == 2 && Near(h, 0.0);
    f.Octaves = 10.0; for (int i = 0; i < 128; ++i) f.ExponentArray[i] = 4.0;
    g_calls = 0; for (int i = 0; i < 16; ++i) g_noise[i] = 0.0f;
    h = f.GetFractalHeight(p);
    ok = ok && Near(h, 1.0);                             // clamped to 1
    if (!ok) { printf("00f36710_TEST FAIL\n"); return 1; }
    printf("00f36710_TEST PASS\n");
    return 0;
}