// Behaviour test for 0x0057A2D0 ceil(float): the smallest integral value not below x, through the double ceil.
#include <cstdio>
#include <math.h>

static float CeilF(float x) { return ((float)ceil((double)x)); }

int main()
{
    bool ok = CeilF(1.25f) == 2.0f && CeilF(-1.25f) == -1.0f && CeilF(3.0f) == 3.0f && CeilF(0.0f) == 0.0f &&
              CeilF(-0.5f) == 0.0f && CeilF(1e7f + 0.5f) >= 1e7f;
    std::printf(ok ? "0057a2d0_TEST PASS\n" : "0057a2d0_TEST FAIL\n");
    return ok ? 0 : 1;
}