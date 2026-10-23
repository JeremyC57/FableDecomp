#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099b510();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bca28;
void AutoTinyThunkTarget::Invoke_0099b510()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a5a0()
{
    g_AutoTinyThunkObject_013bca28.Invoke_0099b510();
}
int main()
{
    GlobalMethodTailThunk_0122a5a0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a5a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a5a0_TEST FAIL\n");
    return 1;
}