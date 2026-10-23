#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b3c;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228ece()
{
    g_AutoTinyThunkObject_013b8b3c.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01228ece();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228ece_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228ece_TEST FAIL\n");
    return 1;
}