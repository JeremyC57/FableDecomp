#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099b510();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b865c;
void AutoTinyThunkTarget::Invoke_0099b510()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228940()
{
    g_AutoTinyThunkObject_013b865c.Invoke_0099b510();
}
int main()
{
    GlobalMethodTailThunk_01228940();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228940_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228940_TEST FAIL\n");
    return 1;
}