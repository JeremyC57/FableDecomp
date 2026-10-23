#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_009d4e90();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013ca828;
void AutoTinyThunkTarget::Invoke_009d4e90()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a780()
{
    g_AutoTinyThunkObject_013ca828.Invoke_009d4e90();
}
int main()
{
    GlobalMethodTailThunk_0122a780();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a780_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a780_TEST FAIL\n");
    return 1;
}