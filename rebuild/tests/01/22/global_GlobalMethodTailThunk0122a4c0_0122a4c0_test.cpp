#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0098abf0();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bc470;
void AutoTinyThunkTarget::Invoke_0098abf0()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a4c0()
{
    g_AutoTinyThunkObject_013bc470.Invoke_0098abf0();
}
int main()
{
    GlobalMethodTailThunk_0122a4c0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a4c0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a4c0_TEST FAIL\n");
    return 1;
}