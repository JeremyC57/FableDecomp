#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bade8;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01229b60()
{
    g_AutoTinyThunkObject_013bade8.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01229b60();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01229b60_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229b60_TEST FAIL\n");
    return 1;
}