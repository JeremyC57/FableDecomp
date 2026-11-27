// Behaviour test for CAIStateGroup_GoToPub::GetObjectWithScriptName (0x00947EB0):
// pick the item at a random index, or null when the list is empty.
#include <cstdio>
static int pick(int count, int index)
{
    if (count == 0) return -1;
    for (int i = 0; i < count; ++i) {
        if (index == 0) return i;
        --index;
    }
    return -1;
}
int main()
{
    if (pick(0, 0) != -1) { std::printf("00947eb0_TEST FAIL\n"); return 1; }
    if (pick(4, 0) != 0 || pick(4, 2) != 2 || pick(4, 3) != 3) { std::printf("00947eb0_TEST FAIL\n"); return 1; }
    std::printf("00947eb0_TEST PASS\n");
    return 0;
}
