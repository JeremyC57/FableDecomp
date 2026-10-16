#include <cstdio>
// Behaviour model of CTCNymph::IsDecentAttackRange (0x0080F190).
static bool IsDecent(float px, float py, float pz, float tx, float ty, float tz, float minRange, float circleRange)
{
    float dx = px - tx, dy = py - ty, dz = pz - tz;
    if (dz > 0.5f) {
        float d = dx * dx + dy * dy;
        if (d > minRange * minRange)
            if (d < circleRange * circleRange)
                return true;
    }
    return false;
}
int main()
{
    if (!IsDecent(5, 0, 2, 0, 0, 0, 2, 10)) { std::printf("0080f190_TEST FAIL\n"); return 1; }
    if (IsDecent(5, 0, 0.5f, 0, 0, 0, 2, 10) || IsDecent(1, 0, 2, 0, 0, 0, 2, 10) || IsDecent(10, 0, 2, 0, 0, 0, 2, 10)) { std::printf("0080f190_TEST FAIL\n"); return 1; }
    std::printf("0080f190_TEST PASS\n");
    return 0;
}