// Behaviour test for CAIStateGroup_MinionSleep::IsRunnable (0x0092E130): a model of the row.
#include <stdio.h>

#include <math.h>
static bool IsRunnable(bool hasSleep, bool fast, bool ready, bool bedRunnable, float dx, float dy, float fx, float fy)
{
    if (!hasSleep) return false;
    if (!fast) {
        if (!ready || !bedRunnable) return false;
        if (sqrtf(dx * dx + dy * dy) > 0.01f) return false;
        if (sqrtf(fx * fx + fy * fy) > 0.1f) return false;
    }
    return true;
}
static int run()
{
    return IsRunnable(true, true, false, false, 5, 5, 5, 5) && !IsRunnable(false, true, true, true, 0, 0, 0, 0)
        && IsRunnable(true, false, true, true, 0.001f, 0, 0.05f, 0) && !IsRunnable(true, false, true, true, 1, 0, 0, 0)
        && !IsRunnable(true, false, true, true, 0, 0, 1, 0) && !IsRunnable(true, false, false, true, 0, 0, 0, 0);
}
int main()
{
    int ok = run();
    printf(ok ? "0092e130_TEST PASS\n" : "0092e130_TEST FAIL\n");
    return ok ? 0 : 1;
}
