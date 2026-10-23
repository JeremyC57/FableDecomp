#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00433cc0();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8760;
void AutoTinyThunkTarget::Invoke_00433cc0()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228af1()
{
    g_AutoTinyThunkObject_013b8760.Invoke_00433cc0();
}
int main()
{
    GlobalMethodTailThunk_01228af1();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228af1_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228af1_TEST FAIL\n");
    return 1;
}