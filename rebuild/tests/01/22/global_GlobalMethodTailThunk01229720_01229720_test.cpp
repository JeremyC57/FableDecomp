#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bac64;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01229720()
{
    g_AutoTinyThunkObject_013bac64.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01229720();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01229720_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229720_TEST FAIL\n");
    return 1;
}