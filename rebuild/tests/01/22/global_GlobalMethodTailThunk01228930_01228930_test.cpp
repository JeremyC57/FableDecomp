#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099a410();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b83d0;
void AutoTinyThunkTarget::Invoke_0099a410()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228930()
{
    g_AutoTinyThunkObject_013b83d0.Invoke_0099a410();
}
int main()
{
    GlobalMethodTailThunk_01228930();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228930_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228930_TEST FAIL\n");
    return 1;
}