// Behaviour test for 0x00689370 float ceil(float): rounds towards +infinity through the double CRT ceil, for
// positive, negative, integral and fractional inputs.
#include <cstdio>
extern "C" double ceil(double value);

static float ceil_float(float value)
{
    return (float)ceil((double)value);
}

int main()
{
    if (ceil_float(1.25f) != 2.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (ceil_float(-1.75f) != -1.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (ceil_float(3.0f) != 3.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (ceil_float(0.0001f) != 1.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (ceil_float(-0.5f) != 0.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (ceil_float(100.01f) != 101.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    std::printf("00689370_TEST PASS\n");
    return 0;
}