#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099b510();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013d27e8;
void AutoTinyThunkTarget::Invoke_0099b510()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122aa70()
{
    g_AutoTinyThunkObject_013d27e8.Invoke_0099b510();
}
int main()
{
    GlobalMethodTailThunk_0122aa70();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122aa70_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122aa70_TEST FAIL\n");
    return 1;
}