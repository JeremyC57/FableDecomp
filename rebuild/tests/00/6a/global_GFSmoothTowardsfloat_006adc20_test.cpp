// Behaviour test for 006adc20: GFSmoothTowards<float> -- step clamped to +-max_step, never overshooting the target (model copy).
#include <cstdio>
template <class T>
void GFSmoothTowards(const T& target, const T& max_step, T& current)
{
    T step = target - current;
    if (step < -max_step) step = -max_step;
    else if (step > max_step) step = max_step;
    current += step;
    if (step > 0) { if (current > target) current = target; }
    else { if (current < target) current = target; }
}
int main()
{
    int fail = 0;
    float c = 0.0f, t = 10.0f, m = 3.0f;
    GFSmoothTowards(t, m, c); if (c != 3.0f) fail = 1;       // clamped up
    GFSmoothTowards(t, m, c); GFSmoothTowards(t, m, c); if (c != 9.0f) fail = 1;
    GFSmoothTowards(t, m, c); if (c != 10.0f) fail = 1;      // reaches, no overshoot
    t = -2.0f; m = 5.0f;
    GFSmoothTowards(t, m, c); if (c != 5.0f) fail = 1;       // clamped down
    GFSmoothTowards(t, m, c); if (c != 0.0f) fail = 1;
    GFSmoothTowards(t, m, c); if (c != -2.0f) fail = 1;      // within reach: lands exactly
    GFSmoothTowards(t, m, c); if (c != -2.0f) fail = 1;      // at target: stays
    if (fail) { std::printf("006adc20_TEST FAIL\n"); return 1; }
    std::printf("006adc20_TEST PASS\n");
    return 0;
}