#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bb6c4;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a310()
{
    g_AutoTinyThunkObject_013bb6c4.Invoke();
}
int main()
{
    GlobalMethodTailThunk_0122a310();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a310_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a310_TEST FAIL\n");
    return 1;
}