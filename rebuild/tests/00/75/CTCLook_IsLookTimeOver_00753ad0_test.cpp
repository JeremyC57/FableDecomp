#include <cstdio>
// Behaviour model of CTCLook::IsLookTimeOver (0x00753AD0).
static bool IsLookTimeOver(bool hasOwner, bool keepLooking, long frame, long lookTimer)
{
    if (hasOwner && !keepLooking)
        return false;
    return frame >= lookTimer;
}
int main()
{
    if (IsLookTimeOver(true, false, 100, 10) || !IsLookTimeOver(true, true, 100, 10) || !IsLookTimeOver(false, false, 10, 10) || IsLookTimeOver(false, false, 9, 10)) { std::printf("00753ad0_TEST FAIL\n"); return 1; }
    std::printf("00753ad0_TEST PASS\n");
    return 0;
}