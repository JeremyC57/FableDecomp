// behaviour test for 00ce1c70: SuspendableProcess calls the bound callback with Par1 and a copy of Par2, then sets StopRunning
#include <cstdio>
static int g_copies = 0;
struct SStr { const char* p; SStr() : p(0) { } SStr(const SStr& o) : p(o.p) { ++g_copies; } };
struct SScript { long n; const char* got; void Boast(long count, SStr text) { n = count; got = text.p; } };
struct SFunc2 {
    bool Running, StopRunning; void (SScript::*Func)(long, SStr); SScript* PClass; long Par1; SStr Par2;
    void SuspendableProcess() { (PClass->*Func)(Par1, Par2); StopRunning = true; }
};
int main()
{
    SScript s; s.n = 0; s.got = 0;
    SFunc2 f; f.Running = true; f.StopRunning = false; f.Func = &SScript::Boast; f.PClass = &s; f.Par1 = 42; f.Par2.p = "boast";
    g_copies = 0;
    f.SuspendableProcess();
    int ok = s.n == 42 && s.got == f.Par2.p && f.StopRunning && g_copies == 1;
    if (!ok) { std::printf("00ce1c70_TEST FAIL\n"); return 1; }
    std::printf("00ce1c70_TEST PASS\n");
    return 0;
}