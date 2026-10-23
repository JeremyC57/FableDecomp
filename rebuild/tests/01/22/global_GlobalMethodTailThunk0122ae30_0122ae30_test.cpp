#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c56370();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_0143bdb0;
void AutoTinyThunkTarget::Invoke_00c56370()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122ae30()
{
    g_AutoTinyThunkObject_0143bdb0.Invoke_00c56370();
}
int main()
{
    GlobalMethodTailThunk_0122ae30();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122ae30_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ae30_TEST FAIL\n");
    return 1;
}