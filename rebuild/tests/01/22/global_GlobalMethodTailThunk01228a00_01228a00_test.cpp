#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b86f8;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228a00()
{
    g_AutoTinyThunkObject_013b86f8.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01228a00();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228a00_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228a00_TEST FAIL\n");
    return 1;
}