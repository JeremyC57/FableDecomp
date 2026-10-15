#include <cstdio>
static char g_log[16]; static int g_n = 0;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
static long g_NumMicrothreads = 3;
struct MicroThreadInfo { int id; };
static MicroThreadInfo* g_deleted = 0;
static void DeleteMicroThread(MicroThreadInfo* thread) { g_deleted = thread; Log('D'); }
class CASuspendableProcess {
public:
    bool Running, StopRunning, TerminateUsingExceptions; float TimeProcessSliceStarted, TimesliceLength;
    CASuspendableProcess() : Running(true), StopRunning(false), TerminateUsingExceptions(false), TimeProcessSliceStarted(0.0f), TimesliceLength(0.0f) {}
    virtual ~CASuspendableProcess() { Log('B'); }
    void TerminateProcess() { StopRunning = true; Running = false; Log('T'); }
};
class CSuspendableProcessMicrothread : public CASuspendableProcess {
public:
    MicroThreadInfo* PMicrothread;
    CSuspendableProcessMicrothread(MicroThreadInfo* t) : PMicrothread(t) {}
    virtual ~CSuspendableProcessMicrothread()
    {
        TerminateProcess();
        DeleteMicroThread(PMicrothread);
        --g_NumMicrothreads;
    }
};
int main()
{
    MicroThreadInfo t; t.id = 1;
    CASuspendableProcess* p = new CSuspendableProcessMicrothread(&t);
    delete p;
    if (g_log[0] != 'T' || g_log[1] != 'D' || g_log[2] != 'B' || g_log[3] != 0 || g_deleted != &t || g_NumMicrothreads != 2) { std::printf("00a44620_TEST FAIL\n"); return 1; }
    std::printf("00a44620_TEST PASS\n");
    return 0;
}