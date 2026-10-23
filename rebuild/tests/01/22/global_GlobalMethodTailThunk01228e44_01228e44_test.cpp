#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0053c9bc();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8a98;
void AutoTinyThunkTarget::Invoke_0053c9bc()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228e44()
{
    g_AutoTinyThunkObject_013b8a98.Invoke_0053c9bc();
}
int main()
{
    GlobalMethodTailThunk_01228e44();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228e44_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228e44_TEST FAIL\n");
    return 1;
}