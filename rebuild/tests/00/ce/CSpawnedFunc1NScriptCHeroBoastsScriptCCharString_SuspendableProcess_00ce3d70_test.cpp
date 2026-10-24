// behaviour test for 00ce3d70: SuspendableProcess calls the bound callback with a copy of Par1 and sets StopRunning
#include <cstdio>
static int g_copies = 0;
struct SStr { const char* p; SStr() : p(0) { } SStr(const SStr& o) : p(o.p) { ++g_copies; } };
struct SScript { const char* got; void Boast(SStr text) { got = text.p; } };
struct SFunc1 {
    bool Running, StopRunning; void (SScript::*Func)(SStr); SScript* PClass; SStr Par1;
    void SuspendableProcess() { (PClass->*Func)(Par1); StopRunning = true; }
};
int main()
{
    SScript s; s.got = 0;
    SFunc1 f; f.Running = true; f.StopRunning = false; f.Func = &SScript::Boast; f.PClass = &s; f.Par1.p = "I am the hero";
    g_copies = 0;
    f.SuspendableProcess();
    int ok = s.got == f.Par1.p && f.StopRunning && g_copies == 1;
    if (!ok) { std::printf("00ce3d70_TEST FAIL\n"); return 1; }
    std::printf("00ce3d70_TEST PASS\n");
    return 0;
}