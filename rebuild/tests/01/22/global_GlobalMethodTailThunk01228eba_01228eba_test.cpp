#include <cstdio>
static int g_AutoTinyThunkCalls = 0;
struct AutoTinyThunkTarget
{
    void Invoke();
};
AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b0c;
void AutoTinyThunkTarget::Invoke()
{
    ++g_AutoTinyThunkCalls;
}
void __fastcall GlobalMethodTailThunk_01228eba()
{
    g_AutoTinyThunkObject_013b8b0c.Invoke();
}
int main()
{
    GlobalMethodTailThunk_01228eba();
    if (g_AutoTinyThunkCalls == 1) { std::printf("AUTO_TINY_01228eba_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228eba_TEST FAIL\n");
    return 1;
}