#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b80;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228f75()
{
    g_AutoTinyThunkObject_013b8b80.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01228f75();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228f75_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228f75_TEST FAIL\n");
    return 1;
}