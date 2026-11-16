#include <cstdio>
static bool IsCurrentTrapToCauseKnockdown()   // NF37: CTCTrapBase::IsCurrentTrapToCauseKnockdown (0x00787C18)
{
    return true;
}
int main()
{
    if (IsCurrentTrapToCauseKnockdown() == true) { std::printf("AUTO_TINY_00787c18_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00787c18_TEST FAIL\n");
    return 1;
}