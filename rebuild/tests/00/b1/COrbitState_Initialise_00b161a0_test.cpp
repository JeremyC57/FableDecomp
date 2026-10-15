// behaviour test for 0x00B161A0 COrbitState::Initialise
#include <cstdio>
struct C3DVector { float X, Y, Z; void Zero() { X = Y = Z = 0.0f; } };
struct COrbitSettings { float RotateSpeed, RotateStart, RotateSpeedRandom, RotateStartRandom; };
static float g_rand[2] = { 1.5f, 0.5f }; static int g_n = 0;
float GFFloatRandom(float range, unsigned int* seed) { ++*seed; return g_rand[g_n++] * range / 2.0f; }
struct COrbitState {
    float NextAngle, RotateSpeedRandomValue; C3DVector FromPosition, ToPosition, NextPosition;
    void Initialise(const COrbitSettings& settings, unsigned int* seed)
    {
        NextAngle = settings.RotateStart;
        NextAngle += (GFFloatRandom(2.0f, seed) - 1.0f) * settings.RotateStartRandom;
        RotateSpeedRandomValue = (GFFloatRandom(2.0f, seed) - 1.0f) * settings.RotateSpeedRandom;
        ToPosition.Zero();
        NextPosition.Zero();
        NextAngle -= (settings.RotateSpeed + RotateSpeedRandomValue) * 2.0f;
    }
};
int main()
{
    COrbitSettings s = { 3.0f, 10.0f, 2.0f, 4.0f };
    COrbitState o; o.ToPosition.X = o.NextPosition.Z = 9.0f; o.FromPosition.X = 7.0f;
    unsigned int seed = 0;
    o.Initialise(s, &seed);
    // angle = 10 + 0.5*4 = 12; rsrv = -0.5*2 = -1; angle -= (3 - 1) * 2 -> 8
    if (seed != 2 || o.RotateSpeedRandomValue != -1.0f || o.NextAngle != 8.0f || o.ToPosition.X != 0.0f
        || o.NextPosition.Z != 0.0f || o.FromPosition.X != 7.0f)
    { std::printf("00b161a0_TEST FAIL\n"); return 1; }
    std::printf("00b161a0_TEST PASS\n");
    return 0;
}