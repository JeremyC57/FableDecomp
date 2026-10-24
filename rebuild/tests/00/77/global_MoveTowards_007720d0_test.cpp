// Behaviour test for 0x007720D0 MoveTowards(current, target, step)
#include <cstdio>
#include <math.h>

static float MoveTowards(float current, float target, float step)
{
    float diff = current - target;
    if (fabs(diff) < step)
        return target;
    if (diff > 0.0f)
        return current - step;
    return current + step;
}

int main()
{
    if (MoveTowards(10.0f, 4.0f, 2.0f) != 8.0f) { std::printf("007720d0_TEST FAIL\n"); return 1; }    // down one step
    if (MoveTowards(1.0f, 4.0f, 2.0f) != 3.0f) { std::printf("007720d0_TEST FAIL\n"); return 1; }     // up one step
    if (MoveTowards(4.5f, 4.0f, 2.0f) != 4.0f) { std::printf("007720d0_TEST FAIL\n"); return 1; }     // snap
    if (MoveTowards(6.0f, 4.0f, 2.0f) != 4.0f) { std::printf("007720d0_TEST FAIL\n"); return 1; }     // |diff| == step: step down
    if (MoveTowards(4.0f, 4.0f, 0.0f) != 4.0f) { std::printf("007720d0_TEST FAIL\n"); return 1; }     // diff 0, step 0: +0
    std::printf("007720d0_TEST PASS\n");
    return 0;
}