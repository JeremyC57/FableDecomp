#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099b510();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b7d5c;
void AutoTinyThunkTarget::Invoke_0099b510()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228910()
{
    g_AutoTinyThunkObject_013b7d5c.Invoke_0099b510();
}
int main()
{
    GlobalMethodTailThunk_01228910();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228910_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228910_TEST FAIL\n");
    return 1;
}