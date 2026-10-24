// behaviour test for 0x00C0B4C0 WrapAngle: one-step wrap of an angle into [-pi, pi)
#include <cstdio>

static const float kPi = 3.14159265f;
static const float kTwoPi = 6.28318531f;
static const float kMinusPi = -3.14159265f;

float WrapAngle(float angle)
{
    if (angle >= kPi)
        return angle - kTwoPi;
    if (angle < kMinusPi)
        return angle + kTwoPi;
    return angle;
}

static bool Near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }

int main()
{
    if (!Near(WrapAngle(1.0f), 1.0f)) { std::printf("00c0b4c0_TEST FAIL\n"); return 1; }
    if (!Near(WrapAngle(4.0f), 4.0f - kTwoPi)) { std::printf("00c0b4c0_TEST FAIL\n"); return 1; }
    if (!Near(WrapAngle(-4.0f), -4.0f + kTwoPi)) { std::printf("00c0b4c0_TEST FAIL\n"); return 1; }
    if (!Near(WrapAngle(kPi), kPi - kTwoPi)) { std::printf("00c0b4c0_TEST FAIL\n"); return 1; }
    if (!Near(WrapAngle(kMinusPi), kMinusPi)) { std::printf("00c0b4c0_TEST FAIL\n"); return 1; }
    std::printf("00c0b4c0_TEST PASS\n");
    return 0;
}