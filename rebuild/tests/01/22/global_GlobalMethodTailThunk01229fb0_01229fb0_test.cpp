#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013baf40;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01229fb0()
{
    g_AutoTinyThunkObject_013baf40.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01229fb0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01229fb0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229fb0_TEST FAIL\n");
    return 1;
}