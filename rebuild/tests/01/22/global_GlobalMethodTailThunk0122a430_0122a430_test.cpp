#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bb72c;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a430()
{
    g_AutoTinyThunkObject_013bb72c.Invoke();
}
int main()
{
    GlobalMethodTailThunk_0122a430();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a430_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a430_TEST FAIL\n");
    return 1;
}