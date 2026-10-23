#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099b510();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b89dc;
void AutoTinyThunkTarget::Invoke_0099b510()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228c50()
{
    g_AutoTinyThunkObject_013b89dc.Invoke_0099b510();
}
int main()
{
    GlobalMethodTailThunk_01228c50();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228c50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228c50_TEST FAIL\n");
    return 1;
}