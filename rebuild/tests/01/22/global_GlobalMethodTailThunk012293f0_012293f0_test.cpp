#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bab0c;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_012293f0()
{
    g_AutoTinyThunkObject_013bab0c.Invoke();
}
int main()
{
    GlobalMethodTailThunk_012293f0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_012293f0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_012293f0_TEST FAIL\n");
    return 1;
}