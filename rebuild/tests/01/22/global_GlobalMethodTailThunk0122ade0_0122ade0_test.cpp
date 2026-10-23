#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c1c820();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_0143bb14;
void AutoTinyThunkTarget::Invoke_00c1c820()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122ade0()
{
    g_AutoTinyThunkObject_0143bb14.Invoke_00c1c820();
}
int main()
{
    GlobalMethodTailThunk_0122ade0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122ade0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ade0_TEST FAIL\n");
    return 1;
}