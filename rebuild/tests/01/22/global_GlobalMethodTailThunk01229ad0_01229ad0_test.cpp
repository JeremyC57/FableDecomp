#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099b510();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bad98;
void AutoTinyThunkTarget::Invoke_0099b510()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01229ad0()
{
    g_AutoTinyThunkObject_013bad98.Invoke_0099b510();
}
int main()
{
    GlobalMethodTailThunk_01229ad0();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01229ad0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229ad0_TEST FAIL\n");
    return 1;
}