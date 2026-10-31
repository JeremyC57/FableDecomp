// behaviour test for 0x006AFF40 CTCPhysicsBase::GetTurnSpeedAngleXY() const
#include <cstdio>
#include <cmath>
static float Bearing(float x, float y) {
    float turns = (float)atan2((double)x, (double)y) * 0.15915494f;
    if (turns < 0.0f || turns >= 1.0f) {
        turns = (float)fmod((double)turns, 1.0);
        if (turns < 0.0f) turns += 1.0f;
    }
    return turns;
}
static float GetTurnSpeedAngleXY(float lx, float ly, float ox, float oy) {
    float last_bearing = Bearing(lx, ly);
    float old_bearing = Bearing(ox, oy);
    float result = last_bearing - old_bearing;
    return result;
}
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main() {
    // +Y is bearing 0, +X is a quarter turn, -X three quarters
    bool ok = Near(GetTurnSpeedAngleXY(1, 0, 0, 1), 0.25f) && Near(GetTurnSpeedAngleXY(0, 1, 1, 0), -0.25f)
        && Near(GetTurnSpeedAngleXY(-1, 0, 0, 1), 0.75f) && Near(GetTurnSpeedAngleXY(0, 1, 0, 1), 0.0f);
    if (!ok) { std::printf("006aff40_TEST FAIL\n"); return 1; }
    std::printf("006aff40_TEST PASS\n");
    return 0;
}