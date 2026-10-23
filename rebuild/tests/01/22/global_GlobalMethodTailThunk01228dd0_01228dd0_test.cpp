#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_004d1d50();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8a54;
void AutoTinyThunkTarget::Invoke_004d1d50()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228dd0()
{
    g_AutoTinyThunkObject_013b8a54.Invoke_004d1d50();
}
int main()
{
    GlobalMethodTailThunk_01228dd0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228dd0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228dd0_TEST FAIL\n");
    return 1;
}