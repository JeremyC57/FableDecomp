#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0098e690();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bc870;
void AutoTinyThunkTarget::Invoke_0098e690()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a4d0()
{
    g_AutoTinyThunkObject_013bc870.Invoke_0098e690();
}
int main()
{
    GlobalMethodTailThunk_0122a4d0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a4d0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a4d0_TEST FAIL\n");
    return 1;
}