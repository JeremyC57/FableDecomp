#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099b510();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8724;
void AutoTinyThunkTarget::Invoke_0099b510()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228a5b()
{
    g_AutoTinyThunkObject_013b8724.Invoke_0099b510();
}
int main()
{
    GlobalMethodTailThunk_01228a5b();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228a5b_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228a5b_TEST FAIL\n");
    return 1;
}