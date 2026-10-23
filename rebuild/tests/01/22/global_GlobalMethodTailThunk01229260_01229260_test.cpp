#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_004cc220();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013ba8ac;
void AutoTinyThunkTarget::Invoke_004cc220()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01229260()
{
    g_AutoTinyThunkObject_013ba8ac.Invoke_004cc220();
}
int main()
{
    GlobalMethodTailThunk_01229260();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01229260_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229260_TEST FAIL\n");
    return 1;
}