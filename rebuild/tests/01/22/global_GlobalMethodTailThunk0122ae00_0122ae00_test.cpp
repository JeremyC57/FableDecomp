#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c0ba40();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_0143bb3c;
void AutoTinyThunkTarget::Invoke_00c0ba40()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122ae00()
{
    g_AutoTinyThunkObject_0143bb3c.Invoke_00c0ba40();
}
int main()
{
    GlobalMethodTailThunk_0122ae00();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122ae00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ae00_TEST FAIL\n");
    return 1;
}