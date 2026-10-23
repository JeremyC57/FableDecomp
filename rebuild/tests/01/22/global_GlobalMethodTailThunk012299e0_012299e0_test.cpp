#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bad34;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_012299e0()
{
    g_AutoTinyThunkObject_013bad34.Invoke();
}
int main()
{
    GlobalMethodTailThunk_012299e0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_012299e0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_012299e0_TEST FAIL\n");
    return 1;
}