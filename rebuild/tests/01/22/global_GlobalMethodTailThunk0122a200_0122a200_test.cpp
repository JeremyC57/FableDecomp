#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_008f9f50();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bb000;
void AutoTinyThunkTarget::Invoke_008f9f50()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a200()
{
    g_AutoTinyThunkObject_013bb000.Invoke_008f9f50();
}
int main()
{
    GlobalMethodTailThunk_0122a200();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a200_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a200_TEST FAIL\n");
    return 1;
}