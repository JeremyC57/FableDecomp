#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bad8c;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01229a80()
{
    g_AutoTinyThunkObject_013bad8c.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01229a80();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01229a80_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229a80_TEST FAIL\n");
    return 1;
}