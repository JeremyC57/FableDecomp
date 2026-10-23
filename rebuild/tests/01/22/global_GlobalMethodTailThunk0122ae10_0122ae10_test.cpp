#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c3caa0();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_0143bc00;
void AutoTinyThunkTarget::Invoke_00c3caa0()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122ae10()
{
    g_AutoTinyThunkObject_0143bc00.Invoke_00c3caa0();
}
int main()
{
    GlobalMethodTailThunk_0122ae10();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122ae10_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ae10_TEST FAIL\n");
    return 1;
}