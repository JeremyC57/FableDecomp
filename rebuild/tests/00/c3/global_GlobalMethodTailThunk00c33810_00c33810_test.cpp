#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke_00c3dcd0();
};
AutoTinyThunkTarget g_AutoTinyThunkObject;
void AutoTinyThunkTarget::Invoke_00c3dcd0()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_00c33810()
{
    g_AutoTinyThunkObject.Invoke_00c3dcd0();
}
int main()
{
    GlobalMethodTailThunk_00c33810();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_00c33810_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00c33810_TEST FAIL\n");
    return 1;
}