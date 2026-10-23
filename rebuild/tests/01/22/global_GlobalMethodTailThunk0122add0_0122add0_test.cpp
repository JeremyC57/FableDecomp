#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c1c820();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_0143bafc;
void AutoTinyThunkTarget::Invoke_00c1c820()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122add0()
{
    g_AutoTinyThunkObject_0143bafc.Invoke_00c1c820();
}
int main()
{
    GlobalMethodTailThunk_0122add0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122add0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122add0_TEST FAIL\n");
    return 1;
}