// Test: CAIStateGroup_ReactToDeadLeader::IsStillRunnable -- not stopping, enemy alive, and FramesRunning below the scaled frame budget
#include <cstdio>
static long g_scaled_013bb738 = 100;
static bool IsStillRunnable(bool stopping, bool enemyAlive, long framesRunning)
{
    if (!stopping && enemyAlive) {
        if (framesRunning >= g_scaled_013bb738) return false;
        return true;
    }
    return false;
}
int main()
{
    if (!IsStillRunnable(false, true, 99) || IsStillRunnable(false, true, 100) || IsStillRunnable(true, true, 1) || IsStillRunnable(false, false, 1)) { std::printf("0095e430_TEST FAIL\n"); return 1; }
    std::printf("0095e430_TEST PASS\n");
    return 0;
}
