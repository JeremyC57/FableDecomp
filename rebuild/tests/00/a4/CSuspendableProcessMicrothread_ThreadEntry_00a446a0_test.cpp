// Behaviour test for CSuspendableProcessMicrothread::ThreadEntry (0x00A446A0): the fibre body runs the process until it is told to
// stop (Running set around each pass, the exit exception swallowed, a suspend after each pass unless stopping), then clears
// Running and sleeps forever. The endless sleep is modelled by a stub that unwinds the test after three sleeps.
#include <stdio.h>
struct CExceptionExit { };
struct StopTest { };
static int g_sleeps = 0;
static void MicroThreadSleep() { if (++g_sleeps == 3) throw StopTest(); }
struct CProcess {
    bool Running, StopRunning; int runs, suspends, stopAfter; bool sawRunning; bool throwExit;
    CProcess() : Running(false), StopRunning(false), runs(0), suspends(0), stopAfter(3), sawRunning(true), throwExit(false) { }
    virtual ~CProcess() { }
    virtual bool SuspendSuspendableProcess() { ++suspends; return false; }
    virtual void SuspendableProcess()
    {
        if (!Running) sawRunning = false;
        if (++runs == stopAfter) StopRunning = true;
        if (throwExit) throw CExceptionExit();
    }
    static void ThreadEntry(void* param);
};
void CProcess::ThreadEntry(void* param)
{
    CProcess* process = (CProcess*)param;
    if (0) {} else for (; !process->StopRunning; ) {
        process->Running = true;
        try {
            process->SuspendableProcess();
        }
        catch (CExceptionExit& e) {
        }
        if (!process->StopRunning)
            process->SuspendSuspendableProcess();
    }
    process->Running = false;
    if (0) {} else for (;;)
        MicroThreadSleep();
}
static bool run(CProcess& p)
{
    g_sleeps = 0;
    try { CProcess::ThreadEntry(&p); } catch (StopTest&) { return true; }
    return false;
}
int main(void)
{
    CProcess a;
    bool ok = run(a) && a.runs == 3 && a.suspends == 2 && a.sawRunning && !a.Running && g_sleeps == 3;
    CProcess b; b.throwExit = true; b.stopAfter = 2;            // the exit exception does not escape
    ok = ok && run(b) && b.runs == 2 && b.suspends == 1 && !b.Running;
    CProcess c; c.StopRunning = true;                           // already stopping: never runs
    ok = ok && run(c) && c.runs == 0 && c.suspends == 0 && !c.Running;
    if (!ok) { printf("00a446a0_TEST FAIL\n"); return 1; }
    printf("00a446a0_TEST PASS\n");
    return 0;
}