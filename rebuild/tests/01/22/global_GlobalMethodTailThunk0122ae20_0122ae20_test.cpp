#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c3cb30();
};
AutoTinyThunkTarget g_AutoTinyThunkObject;
void AutoTinyThunkTarget::Invoke_00c3cb30()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122ae20()
{
    g_AutoTinyThunkObject.Invoke_00c3cb30();
}
int main()
{
    GlobalMethodTailThunk_0122ae20();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122ae20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ae20_TEST FAIL\n");
    return 1;
}