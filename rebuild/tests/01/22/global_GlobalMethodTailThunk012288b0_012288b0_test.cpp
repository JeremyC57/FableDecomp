#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_004051c0();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b7cd8;
void AutoTinyThunkTarget::Invoke_004051c0()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_012288b0()
{
    g_AutoTinyThunkObject_013b7cd8.Invoke_004051c0();
}
int main()
{
    GlobalMethodTailThunk_012288b0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_012288b0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_012288b0_TEST FAIL\n");
    return 1;
}