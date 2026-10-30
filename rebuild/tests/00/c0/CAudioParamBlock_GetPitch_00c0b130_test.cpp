// Behaviour test for 0x00C0B130 CAudioParamBlock::GetPitch (see src/00c0b130.cpp): value times the masked pitch factors, clamped to [0.25, 4].
#include <cstdio>
static float Multiply(float value, const float* f, unsigned long mask, unsigned long count)
{
    while (value != 0.0f && mask != 0 && count != 0) { if (mask & 1) value *= *f; --count; mask >>= 1; ++f; }
    return value;
}
static float GetPitch(const float* f, float value, unsigned short mask)
{
    float p = Multiply(value, f, mask, 8);
    if (p < 0.25f) return 0.25f;
    if (p > 4.0f) return 4.0f;
    return p;
}
int main()
{
    float f[8] = { 2.0f, 3.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 10.0f };
    if (GetPitch(f, 1.0f, 0x0001) != 2.0f || GetPitch(f, 1.0f, 0x0003) != 4.0f || GetPitch(f, 1.0f, 0x0004) != 0.5f
        || GetPitch(f, 0.1f, 0x0000) != 0.25f || GetPitch(f, 1.0f, 0x0180) != 4.0f) { std::printf("00c0b130_TEST FAIL\n"); return 1; }
    std::printf("00c0b130_TEST PASS\n"); return 0;
}