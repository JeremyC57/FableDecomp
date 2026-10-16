#include <cstdio>
#include <cmath>
// Behaviour model of CTCSleep::GetFastTrack (0x0092C3B0): the flag survives only while the thing stays put.
struct Sleep { bool fast; float px, py, pz;
    bool Get(float x, float y, float z) {
        if (fast)
            fast = ((float)std::fabs(px - x) <= 0.0001f && (float)std::fabs(py - y) <= 0.0001f && (float)std::fabs(pz - z) <= 0.0001f) ? true : false;
        return fast;
    } };
int main()
{
    Sleep s = { true, 1, 2, 3 };
    if (!s.Get(1, 2, 3.00005f)) { std::printf("0092c3b0_TEST FAIL\n"); return 1; }
    if (s.Get(1, 2.5f, 3) || s.fast || s.Get(1, 2, 3)) { std::printf("0092c3b0_TEST FAIL\n"); return 1; }
    std::printf("0092c3b0_TEST PASS\n");
    return 0;
}