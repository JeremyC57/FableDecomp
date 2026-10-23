#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0090b750();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013bb664;
void AutoTinyThunkTarget::Invoke_0090b750()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_0122a230()
{
    g_AutoTinyThunkObject_013bb664.Invoke_0090b750();
}
int main()
{
    GlobalMethodTailThunk_0122a230();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_0122a230_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122a230_TEST FAIL\n");
    return 1;
}