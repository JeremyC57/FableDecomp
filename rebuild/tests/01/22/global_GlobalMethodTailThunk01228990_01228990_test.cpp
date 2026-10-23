#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8670;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228990()
{
    g_AutoTinyThunkObject_013b8670.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01228990();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228990_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228990_TEST FAIL\n");
    return 1;
}