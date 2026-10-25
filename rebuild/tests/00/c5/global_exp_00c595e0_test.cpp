// Behaviour test for exp(float) (0x00C595E0): the float overload widens, takes e^x and narrows.
#include <cstdio>
#include <math.h>
static float ExpFloat(float value) { return (float)exp((double)value); }
static bool Near(float a, float b) { float d = a - b; if (d < 0) d = -d; float m = b < 0 ? -b : b; return d <= 1e-5f * (m > 1 ? m : 1); }
int main()
{
    int ok = Near(ExpFloat(0.0f), 1.0f) && Near(ExpFloat(1.0f), 2.7182817f) && Near(ExpFloat(-2.5f), 0.082085f)
          && Near(ExpFloat(10.0f), 22026.465f);
    if (!ok) { std::printf("00c595e0_TEST FAIL\n"); return 1; }
    std::printf("00c595e0_TEST PASS\n");
    return 0;
}