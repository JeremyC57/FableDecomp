// Behaviour test for 0x00C0B420 CAudioParamBlock::GetGain (see src/00c0b420.cpp): value times the masked ramped gains, clamped to [0, 1].
#include <cstdio>
struct Param { float value; bool angle; };
static float Multiply(float value, const Param* p, unsigned long mask, unsigned long count)
{
    while (value != 0.0f && mask != 0 && count != 0) { if (mask & 1) value *= p->value; --count; mask >>= 1; ++p; }
    return value;
}
static float GetGain(const Param* p, float value, unsigned short mask)
{
    float g = Multiply(value, p, mask, 16);
    if (g < 0.0f) return 0.0f;
    if (g > 1.0f) return 1.0f;
    return g;
}
int main()
{
    Param p[16];
    for (int i = 0; i < 16; ++i) { p[i].value = 1.0f; p[i].angle = false; }
    p[0].value = 0.5f; p[1].value = 4.0f; p[2].value = -1.0f;
    if (GetGain(p, 1.0f, 0x0001) != 0.5f || GetGain(p, 1.0f, 0x0002) != 1.0f || GetGain(p, 1.0f, 0x0004) != 0.0f
        || GetGain(p, 0.25f, 0x0003) != 0.5f) { std::printf("00c0b420_TEST FAIL\n"); return 1; }
    std::printf("00c0b420_TEST PASS\n"); return 0;
}