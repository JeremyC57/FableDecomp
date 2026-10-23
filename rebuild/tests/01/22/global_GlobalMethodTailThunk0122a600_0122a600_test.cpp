#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_009f9f70();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013c5a80;
void AutoTinyThunkTarget::Invoke_009f9f70()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a600()
{
    g_AutoTinyThunkObject_013c5a80.Invoke_009f9f70();
}
int main()
{
    GlobalMethodTailThunk_0122a600();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a600_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a600_TEST FAIL\n");
    return 1;
}