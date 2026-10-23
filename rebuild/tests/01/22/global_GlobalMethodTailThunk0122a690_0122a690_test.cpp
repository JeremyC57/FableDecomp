#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099b510();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013ca7d4;
void AutoTinyThunkTarget::Invoke_0099b510()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a690()
{
    g_AutoTinyThunkObject_013ca7d4.Invoke_0099b510();
}
int main()
{
    GlobalMethodTailThunk_0122a690();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a690_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a690_TEST FAIL\n");
    return 1;
}