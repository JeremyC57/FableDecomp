// Behaviour test for 0x007D7A90 GetHaltonSequenceNumber(index, base): base-2 and base-3 radical inverses.
#include <cstdio>

float GetHaltonSequenceNumber(int index, int base)
{
    float result = 0.0f;
    float invBase = 1.0f / base;
    float frac = invBase;
    while (index > 0) {
        int digit = index % base;
        index /= base;
        result += digit * frac;
        frac *= invBase;
    }
    return result;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

int main()
{
    float want2[8] = { 0.0f, 0.5f, 0.25f, 0.75f, 0.125f, 0.625f, 0.375f, 0.875f };
    float want3[6] = { 0.0f, 1.0f / 3, 2.0f / 3, 1.0f / 9, 4.0f / 9, 7.0f / 9 };
    bool ok = true;
    for (int i = 0; i < 8; ++i) ok = ok && Near(GetHaltonSequenceNumber(i, 2), want2[i]);
    for (int i = 0; i < 6; ++i) ok = ok && Near(GetHaltonSequenceNumber(i, 3), want3[i]);
    ok = ok && Near(GetHaltonSequenceNumber(-4, 2), 0.0f);
    if (!ok) { std::printf("007d7a90_TEST FAIL\n"); return 1; }
    std::printf("007d7a90_TEST PASS\n");
    return 0;
}