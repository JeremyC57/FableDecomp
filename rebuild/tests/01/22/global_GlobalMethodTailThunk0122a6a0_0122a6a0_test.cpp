#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_009a7b70();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013ca79c;
void AutoTinyThunkTarget::Invoke_009a7b70()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a6a0()
{
    g_AutoTinyThunkObject_013ca79c.Invoke_009a7b70();
}
int main()
{
    GlobalMethodTailThunk_0122a6a0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a6a0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a6a0_TEST FAIL\n");
    return 1;
}