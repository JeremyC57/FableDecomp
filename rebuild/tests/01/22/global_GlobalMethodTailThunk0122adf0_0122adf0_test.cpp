#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c16620();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_0143bb34;
void AutoTinyThunkTarget::Invoke_00c16620()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122adf0()
{
    g_AutoTinyThunkObject_0143bb34.Invoke_00c16620();
}
int main()
{
    GlobalMethodTailThunk_0122adf0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122adf0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122adf0_TEST FAIL\n");
    return 1;
}