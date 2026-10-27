#include <cstdio>
// Behaviour model: the task is returned to the village once and then cleared.
static int removed;
int main()
{
    int task = 1; int* pTask = &task;
    if (pTask) { ++removed; pTask = 0; }
    if (pTask) { ++removed; }
    if (removed != 1 || pTask) { std::printf("0093ee40_TEST FAIL\n"); return 1; }
    std::printf("0093ee40_TEST PASS\n");
    return 0;
}