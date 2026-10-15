#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
#define FASTCALL __fastcall
#else
#define FASTCALL
#endif
struct MicroThreadInfo { void (*entry)(void*); void* param; };
static MicroThreadInfo g_thread; static bool g_canCreate = true;
MicroThreadInfo* FASTCALL CreateMicroThread(void (*entry)(void*), void* param) { if (!g_canCreate) return 0; g_thread.entry = entry; g_thread.param = param; return &g_thread; }
static int g_entryRuns = 0;
void MicrothreadEntry(void* param) { (void)param; ++g_entryRuns; }
long g_NumMicrothreads = 0;
struct CFibreException { const char* m_text; CFibreException(const char* text) : m_text(text) { } };
struct CBase { virtual ~CBase() { } };
struct CASuspendableProcess : CBase {
    unsigned char Running, StopRunning, TerminateUsingExceptions; long TimeProcessSliceStarted; long TimesliceLength;
    CASuspendableProcess(bool terminateUsingExceptions, long unused, long timesliceLength)
        : Running(0), StopRunning(0), TerminateUsingExceptions(terminateUsingExceptions), TimeProcessSliceStarted(0), TimesliceLength(timesliceLength) { (void)unused; }
};
struct CSuspendableProcessMicrothread : CASuspendableProcess {
    MicroThreadInfo* PMicrothread;
    CSuspendableProcessMicrothread(bool terminateUsingExceptions, long unused, long timesliceLength);
};
CSuspendableProcessMicrothread::CSuspendableProcessMicrothread(bool terminateUsingExceptions, long unused, long timesliceLength)
    : CASuspendableProcess(terminateUsingExceptions, unused, timesliceLength)
{
    PMicrothread = 0;
    PMicrothread = CreateMicroThread(MicrothreadEntry, this);
    if (PMicrothread == 0)
        throw CFibreException("Failed to create fibre");
    g_NumMicrothreads = g_NumMicrothreads + 1;
}
int main()
{
    bool ok = false;
    {
        CSuspendableProcessMicrothread process(true, 5, 16);
        ok = process.PMicrothread == &g_thread && g_thread.param == &process && g_thread.entry == &MicrothreadEntry && g_NumMicrothreads == 1
            && process.TerminateUsingExceptions == 1 && process.TimesliceLength == 16 && process.Running == 0 && process.StopRunning == 0;
        g_thread.entry(g_thread.param);
        ok = ok && g_entryRuns == 1;
    }
    g_canCreate = false;
    bool threw = false;
    try { CSuspendableProcessMicrothread failing(false, 0, 0); } catch (CFibreException& e) { threw = std::strcmp(e.m_text, "Failed to create fibre") == 0; }
    ok = ok && threw && g_NumMicrothreads == 1;
    if (!ok) { std::printf("00a44740_TEST FAIL\n"); return 1; }
    std::printf("00a44740_TEST PASS\n");
    return 0;
}
