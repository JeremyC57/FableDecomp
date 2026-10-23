#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_0143e91c;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122ae74()
{
    g_AutoTinyThunkObject_0143e91c.Invoke();
}
int main()
{
    GlobalMethodTailThunk_0122ae74();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122ae74_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ae74_TEST FAIL\n");
    return 1;
}