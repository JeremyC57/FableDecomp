#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_009a5800();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013ca618;
void AutoTinyThunkTarget::Invoke_009a5800()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a680()
{
    g_AutoTinyThunkObject_013ca618.Invoke_009a5800();
}
int main()
{
    GlobalMethodTailThunk_0122a680();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a680_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a680_TEST FAIL\n");
    return 1;
}