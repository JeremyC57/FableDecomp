#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b9c;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228fbb()
{
    g_AutoTinyThunkObject_013b8b9c.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01228fbb();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228fbb_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228fbb_TEST FAIL\n");
    return 1;
}