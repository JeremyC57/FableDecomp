#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b4c;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228eec()
{
    g_AutoTinyThunkObject_013b8b4c.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01228eec();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228eec_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228eec_TEST FAIL\n");
    return 1;
}