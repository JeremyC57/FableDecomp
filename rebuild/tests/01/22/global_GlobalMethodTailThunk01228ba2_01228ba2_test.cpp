#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0044f3de();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b879c;
void AutoTinyThunkTarget::Invoke_0044f3de()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228ba2()
{
    g_AutoTinyThunkObject_013b879c.Invoke_0044f3de();
}
int main()
{
    GlobalMethodTailThunk_01228ba2();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228ba2_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228ba2_TEST FAIL\n");
    return 1;
}