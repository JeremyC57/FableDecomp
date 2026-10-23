#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00687180();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013ba89c;
void AutoTinyThunkTarget::Invoke_00687180()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01229270()
{
    g_AutoTinyThunkObject_013ba89c.Invoke_00687180();
}
int main()
{
    GlobalMethodTailThunk_01229270();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01229270_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01229270_TEST FAIL\n");
    return 1;
}