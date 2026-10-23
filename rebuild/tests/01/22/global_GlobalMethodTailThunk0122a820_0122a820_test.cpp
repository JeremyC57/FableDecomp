#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0098d710();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013ca8a8;
void AutoTinyThunkTarget::Invoke_0098d710()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a820()
{
    g_AutoTinyThunkObject_013ca8a8.Invoke_0098d710();
}
int main()
{
    GlobalMethodTailThunk_0122a820();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a820_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a820_TEST FAIL\n");
    return 1;
}