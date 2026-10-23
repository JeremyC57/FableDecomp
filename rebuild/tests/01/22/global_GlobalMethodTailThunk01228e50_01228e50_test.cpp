#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0055c300();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8ad4;
void AutoTinyThunkTarget::Invoke_0055c300()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228e50()
{
    g_AutoTinyThunkObject_013b8ad4.Invoke_0055c300();
}
int main()
{
    GlobalMethodTailThunk_01228e50();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228e50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228e50_TEST FAIL\n");
    return 1;
}