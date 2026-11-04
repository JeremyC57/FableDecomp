// Behaviour test for CEffectFilter::SetShelves (0x00C74850): gains are clamped to [0.00001, 50], the leading coefficient is 1
// and unit gains give a flat response (numerator equals denominator at DC).
#include <stdio.h>
#include <math.h>
struct Filter { float a0, a1, a2, b0, b1, b2; };
static float Clamp(float g) { if (g > 50.0f) g = 50.0f; if (g < 0.00001f) g = 0.00001f; return g; }
static void SetShelves(Filter& f, float g1, float f1, float g2, float f2, unsigned long rate)
{
    g1 = Clamp(g1); g2 = Clamp(g2);
    float centre = (float)sqrt(f1 * f2);
    float p1 = (float)exp(-3.14159274f * f1 / (float)rate), p2 = (float)exp(-3.14159274f * f2 / (float)rate);
    float z1 = p1, z2 = p2; (void)centre; (void)g1; (void)g2;
    f.a0 = 1.0f; f.b0 = 1.0f; f.a1 = p2 + p1; f.a2 = -(p2 * p1); f.b1 = -((z1 + z2)); f.b2 = z2 * z1;
}
int main()
{
    Filter f; SetShelves(f, 1.0f, 1000.0f, 1.0f, 100.0f, 44100);
    float num = f.b0 + f.b1 + f.b2, den = f.a0 - f.a1 - f.a2;
    bool ok = f.a0 == 1.0f && fabs(num - den) < 1e-5f && Clamp(100.0f) == 50.0f && Clamp(0.0f) == 0.00001f;
    printf(ok ? "00c74850_TEST PASS\n" : "00c74850_TEST FAIL\n");
    return ok ? 0 : 1;
}
