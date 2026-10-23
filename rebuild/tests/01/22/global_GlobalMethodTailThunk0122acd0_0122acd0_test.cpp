#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_01436ec4;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122acd0()
{
    g_AutoTinyThunkObject_01436ec4.Invoke();
}
int main()
{
    GlobalMethodTailThunk_0122acd0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122acd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122acd0_TEST FAIL\n");
    return 1;
}