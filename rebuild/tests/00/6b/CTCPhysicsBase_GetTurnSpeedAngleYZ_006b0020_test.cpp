// behaviour test for 0x006B0020 CTCPhysicsBase::GetTurnSpeedAngleYZ() const
#include <cstdio>
#include <cmath>
static float Wrap(float t) { if (t < 0.0f || t >= 1.0f) { t = (float)fmod((double)t, 1.0); if (t < 0.0f) t += 1.0f; } return t; }
static float AngleYZ(float x, float y, float z) { return Wrap((float)atan2((double)z, std::sqrt((double)(x * x + y * y))) * 0.15915494f); }
static float GetTurnSpeedAngleYZ(const float* cur, const float* last) {
    float result = AngleYZ(cur[0], cur[1], cur[2]) - AngleYZ(last[0], last[1], last[2]);
    return result;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main() {
    float flat[3] = {0, 1, 0}, up45[3] = {0, 1, 1}, down45[3] = {0, 1, -1};
    bool ok = Near(GetTurnSpeedAngleYZ(up45, flat), 0.125f) && Near(GetTurnSpeedAngleYZ(flat, up45), -0.125f)
        && Near(GetTurnSpeedAngleYZ(down45, flat), 0.875f) && Near(GetTurnSpeedAngleYZ(flat, flat), 0.0f);
    if (!ok) { std::printf("006b0020_TEST FAIL\n"); return 1; }
    std::printf("006b0020_TEST PASS\n");
    return 0;
}