#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c3d470();
};
AutoTinyThunkTarget g_AutoTinyThunkObject;
void AutoTinyThunkTarget::Invoke_00c3d470()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_00c336a0()
{
    g_AutoTinyThunkObject.Invoke_00c3d470();
}
int main()
{
    GlobalMethodTailThunk_00c336a0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_00c336a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00c336a0_TEST FAIL\n");
    return 1;
}