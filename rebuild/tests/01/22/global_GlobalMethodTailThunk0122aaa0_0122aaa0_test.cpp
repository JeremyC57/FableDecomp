#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00a47ef0();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013d2834;
void AutoTinyThunkTarget::Invoke_00a47ef0()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122aaa0()
{
    g_AutoTinyThunkObject_013d2834.Invoke_00a47ef0();
}
int main()
{
    GlobalMethodTailThunk_0122aaa0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122aaa0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122aaa0_TEST FAIL\n");
    return 1;
}