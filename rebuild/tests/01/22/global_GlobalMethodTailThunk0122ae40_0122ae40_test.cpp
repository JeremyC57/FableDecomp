#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c3e4c0();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_0143bf88;
void AutoTinyThunkTarget::Invoke_00c3e4c0()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122ae40()
{
    g_AutoTinyThunkObject_0143bf88.Invoke_00c3e4c0();
}
int main()
{
    GlobalMethodTailThunk_0122ae40();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122ae40_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ae40_TEST FAIL\n");
    return 1;
}