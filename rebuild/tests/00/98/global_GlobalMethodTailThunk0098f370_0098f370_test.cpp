#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0098f1e0();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bc870;
void AutoTinyThunkTarget::Invoke_0098f1e0()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0098f370()
{
    g_AutoTinyThunkObject_013bc870.Invoke_0098f1e0();
}
int main()
{
    GlobalMethodTailThunk_0098f370();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0098f370_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0098f370_TEST FAIL\n");
    return 1;
}