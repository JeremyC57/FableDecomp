// Test: CAIStateGroup_PrepareMeal::IsStillRunnable -- home building set, meal time, cooking pot set and not dead
#include <cstdio>
struct CThing { unsigned int Dead : 1; };
static bool IsStillRunnable(const void* home, bool mealTime, const CThing* pot)
{
    return home && mealTime && pot && (pot->Dead == 0);
}
int main()
{
    int house = 0; CThing alive; alive.Dead = 0; CThing dead; dead.Dead = 1;
    if (!IsStillRunnable(&house, true, &alive) || IsStillRunnable(0, true, &alive) || IsStillRunnable(&house, false, &alive)
        || IsStillRunnable(&house, true, 0) || IsStillRunnable(&house, true, &dead)) { std::printf("00946330_TEST FAIL\n"); return 1; }
    std::printf("00946330_TEST PASS\n");
    return 0;
}
