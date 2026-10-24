// Behaviour test for 0x00A44660: CSuspendableProcessMicrothread::InternalSwitchToProcess (running process set around the microthread switch)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

namespace MicroThread {
class MicroThreadInfo {
public:
    int Id;
};
}
class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess() { }
    bool  Running;
    bool  StopRunning;
    bool  TerminateUsingExceptions;
    float TimeProcessSliceStarted;
    float TimesliceLength;
};
class CSuspendableProcessMicrothread : public CASuspendableProcess {
public:
    MicroThread::MicroThreadInfo* PMicrothread;
    virtual ~CSuspendableProcessMicrothread() { }
    static void __fastcall InternalSwitchToProcess(CSuspendableProcessMicrothread& process);
};

CSuspendableProcessMicrothread* g_pScriptRunner = 0;
static CSuspendableProcessMicrothread* g_seenRunner = 0;
static MicroThread::MicroThreadInfo* g_seenInfo = 0;
static int g_switches = 0;
namespace MicroThread {
bool __fastcall SwitchToMicroThread(MicroThreadInfo* info)
{
    ++g_switches;
    g_seenRunner = g_pScriptRunner;
    g_seenInfo = info;
    return true;
}
}

void __fastcall CSuspendableProcessMicrothread::InternalSwitchToProcess(CSuspendableProcessMicrothread& process)
{
    g_pScriptRunner = &process;
    MicroThread::SwitchToMicroThread(process.PMicrothread);
    g_pScriptRunner = 0;
}

int main()
{
    MicroThread::MicroThreadInfo info;
    info.Id = 3;
    CSuspendableProcessMicrothread p;
    p.PMicrothread = &info;
    CSuspendableProcessMicrothread::InternalSwitchToProcess(p);
    Check(g_switches == 1 && g_seenRunner == &p && g_seenInfo == &info && g_pScriptRunner == 0);
    if (g_fail) {
        printf("00a44660_TEST FAIL\n");
        return 1;
    }
    printf("00a44660_TEST PASS\n");
    return 0;
}