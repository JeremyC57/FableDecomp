#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8748;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228ab5()
{
    g_AutoTinyThunkObject_013b8748.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01228ab5();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228ab5_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228ab5_TEST FAIL\n");
    return 1;
}