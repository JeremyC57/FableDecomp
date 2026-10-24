// Behaviour test for 0x007D7AE0 GetNextHaltonSequenceNumber(value, base): stepping from 0 reproduces the base-2 and
// base-3 Halton sequences.
#include <cstdio>

void GetNextHaltonSequenceNumber(float& value, int base)
{
    double h = 1.0;
    while (h >= 1 - value)
        h *= 1.0 / base;
    value += (base + 1.0) * h - 1.0;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }

int main()
{
    float want2[7] = { 0.5f, 0.25f, 0.75f, 0.125f, 0.625f, 0.375f, 0.875f };
    float want3[5] = { 1.0f / 3, 2.0f / 3, 1.0f / 9, 4.0f / 9, 7.0f / 9 };
    bool ok = true;
    float v = 0.0f;
    for (int i = 0; i < 7; ++i) { GetNextHaltonSequenceNumber(v, 2); ok = ok && Near(v, want2[i]); }
    v = 0.0f;
    for (int i = 0; i < 5; ++i) { GetNextHaltonSequenceNumber(v, 3); ok = ok && Near(v, want3[i]); }
    if (!ok) { std::printf("007d7ae0_TEST FAIL\n"); return 1; }
    std::printf("007d7ae0_TEST PASS\n");
    return 0;
}