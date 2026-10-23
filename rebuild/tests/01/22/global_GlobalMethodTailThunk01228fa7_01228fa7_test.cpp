#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b94;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228fa7()
{
    g_AutoTinyThunkObject_013b8b94.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01228fa7();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228fa7_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228fa7_TEST FAIL\n");
    return 1;
}