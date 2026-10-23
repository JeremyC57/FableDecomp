#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_0099b510();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8660;
void AutoTinyThunkTarget::Invoke_0099b510()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228950()
{
    g_AutoTinyThunkObject_013b8660.Invoke_0099b510();
}
int main()
{
    GlobalMethodTailThunk_01228950();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228950_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228950_TEST FAIL\n");
    return 1;
}