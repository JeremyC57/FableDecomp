#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00cd31d2();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_0143e910;
void AutoTinyThunkTarget::Invoke_00cd31d2()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122aea0()
{
    g_AutoTinyThunkObject_0143e910.Invoke_00cd31d2();
}
int main()
{
    GlobalMethodTailThunk_0122aea0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122aea0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122aea0_TEST FAIL\n");
    return 1;
}