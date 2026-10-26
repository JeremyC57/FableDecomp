// Behaviour test for 0x00cb7950 CScriptBase::UpdateSpawnedFunction
#include <cstdio>
#include <cstring>

static char g_log[32]; static int g_n = 0;
static void Log(char c) { if (g_n < 31) { g_log[g_n++] = c; g_log[g_n] = 0; } }

class CSpawnedFuncBase { public:
    bool Running, StopRunning, FuncPaused, Interrupted;
    int predicateOkCalls;          // TestPredicate returns true this many times, then false
    bool stopOnResume;
    virtual ~CSpawnedFuncBase() { }
    virtual void ResumeTimeslice() { Log('R'); if (stopOnResume) StopRunning = true; }
    virtual void Restart() { Log('S'); }
    virtual void OnPredicateFail() { Log('F'); }
    virtual void OnInterrupted() { Log('I'); }
    bool IsSuspendableProcessRunning() const { return Running; }
    bool IsSuspendableProcessStopping() const { return StopRunning; }
    void StopSuspendableProcess() { StopRunning = true; }
    void TerminateProcess() { Log('T'); Running = false; }
    bool IsFuncPaused() const { return FuncPaused; }
    bool IsFuncInterrupted() const { return Interrupted; }
    void SetFuncAsInterrupted(bool i) { Interrupted = i; }
    bool TestPredicate() { Log('P'); return predicateOkCalls-- > 0; }
    void RemovePredicate() { Log('X'); }
};
class CScriptBase { public:
    CSpawnedFuncBase* PActiveThread;
    bool UpdateSpawnedFunction(CSpawnedFuncBase* func)
    {
        PActiveThread = func;
        bool interrupted = false;
        if (!func->IsFuncPaused()) {
            if (func->TestPredicate()) {
                if (func->IsFuncInterrupted()) {
                    func->OnInterrupted();
                    func->TerminateProcess();
                    func->Restart();
                    func->SetFuncAsInterrupted(false);
                    interrupted = true;
                }
                else {
                    func->ResumeTimeslice();
                    if (!func->TestPredicate()) {
                        func->StopSuspendableProcess();
                        func->RemovePredicate();
                        func->OnPredicateFail();
                    }
                }
            }
            else {
                func->StopSuspendableProcess();
                func->RemovePredicate();
                func->OnPredicateFail();
            }
            if (!func->IsSuspendableProcessRunning() && !interrupted)
                return true;
            if (func->IsSuspendableProcessStopping()) {
                func->TerminateProcess();
                return true;
            }
        }
        PActiveThread = 0;
        return false;
    }
};

static CSpawnedFuncBase Make(bool running, bool paused, bool interrupted, int ok)
{
    CSpawnedFuncBase f; f.Running = running; f.StopRunning = false; f.FuncPaused = paused; f.Interrupted = interrupted;
    f.predicateOkCalls = ok; f.stopOnResume = false; return f;
}

int main()
{
    CScriptBase sb;
    // paused: no work, thread cleared, false
    CSpawnedFuncBase a = Make(true, true, false, 9); g_n = 0; g_log[0] = 0;
    if (sb.UpdateSpawnedFunction(&a) || sb.PActiveThread != 0 || g_n != 0) { std::printf("00cb7950_TEST FAIL\n"); return 1; }
    // normal frame, still running: resume, re-test, false
    CSpawnedFuncBase b = Make(true, false, false, 9); g_n = 0;
    if (sb.UpdateSpawnedFunction(&b) || std::strcmp(g_log, "PRP") != 0 || sb.PActiveThread != 0) { std::printf("00cb7950_TEST FAIL\n"); return 1; }
    // predicate fails after the frame: stop, remove, fail hook, terminate, true
    CSpawnedFuncBase c = Make(true, false, false, 1); g_n = 0;
    if (!sb.UpdateSpawnedFunction(&c) || std::strcmp(g_log, "PRPXFT") != 0 || sb.PActiveThread != &c) { std::printf("00cb7950_TEST FAIL\n"); return 1; }
    // interrupted (restarted but not running afterwards): interrupt path then 'running || interrupted' -> not stopping -> false
    CSpawnedFuncBase d = Make(true, false, true, 9); g_n = 0;
    if (sb.UpdateSpawnedFunction(&d) || std::strcmp(g_log, "PITS") != 0 || d.Interrupted) { std::printf("00cb7950_TEST FAIL\n"); return 1; }
    // finished thread (not running): true
    CSpawnedFuncBase e = Make(false, false, false, 9); g_n = 0;
    if (!sb.UpdateSpawnedFunction(&e) || std::strcmp(g_log, "PRP") != 0) { std::printf("00cb7950_TEST FAIL\n"); return 1; }
    std::printf("00cb7950_TEST PASS\n");
    return 0;
}