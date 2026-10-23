#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099a410();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013cb2f8;
void AutoTinyThunkTarget::Invoke_0099a410()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a9e0()
{
    g_AutoTinyThunkObject_013cb2f8.Invoke_0099a410();
}
int main()
{
    GlobalMethodTailThunk_0122a9e0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a9e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a9e0_TEST FAIL\n");
    return 1;
}