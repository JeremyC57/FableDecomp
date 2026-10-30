// Behaviour test for 0x00689370 float floor(float): rounds towards -infinity through the double CRT floor, for
// positive, negative, integral and fractional inputs.
#include <cstdio>
extern "C" double floor(double value);

static float floor_float(float value)
{
    return (float)floor((double)value);
}

int main()
{
    if (floor_float(1.25f) != 1.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (floor_float(-1.75f) != -2.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (floor_float(3.0f) != 3.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (floor_float(0.9999f) != 0.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (floor_float(-0.5f) != -1.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    if (floor_float(100.99f) != 100.0f) { std::printf("00689370_TEST FAIL\n"); return 1; }
    std::printf("00689370_TEST PASS\n");
    return 0;
}
