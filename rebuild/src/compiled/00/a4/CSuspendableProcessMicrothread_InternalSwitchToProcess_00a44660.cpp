// CSuspendableProcessMicrothread::InternalSwitchToProcess(CSuspendableProcessMicrothread& process) -- make the process the running one, switch to its microthread until it yields back, then clear the running process
// Leaf: ?InternalSwitchToProcess@CSuspendableProcessMicrothread@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44660, length 40 bytes (+8 int3 pad), protected static `__fastcall`
//   (process in ecx, plain `ret`).
// Name: Ego_r.pdb / FableWin.pdb `?InternalSwitchToProcess@CSuspendableProcessMicrothread@@KIXAAV1@@Z` (name set); the landed
//   caller CSuspendableProcessMicrothread::RunSuspendableProcess 0x00A44880 declares it the same way.
// Callees: MicroThread::SwitchToMicroThread(MicroThreadInfo*) 0x009D87F0 (`__fastcall`, bool, PDB
//   `?SwitchToMicroThread@MicroThread@@YI_NPAVMicroThreadInfo@1@@Z`). Global: the running process 0x013D2838 (the landed
//   neighbours call it g_pScriptRunner).
// Layout (PDB): CSuspendableProcessMicrothread is 0x14 bytes, PMicrothread (MicroThreadInfo*) +0x10.
// Shape: an UNOPTIMISED TU (`#pragma optimize("", off)`, as the landed ctor 0x00A44740, Restart 0x00A447D0,
//   SuspendSuspendableProcess 0x00A44840 and RunSuspendableProcess 0x00A44880): ebp frame, the reference spilled to [ebp-4]
//   and re-read at each use, the bool result of the switch ignored.
#pragma optimize("", off)
namespace MicroThread {
class MicroThreadInfo;
bool __fastcall SwitchToMicroThread(MicroThreadInfo* info);     // 0x009D87F0
}

class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();                 // 0x00A44610
    bool  Running;                                   // +0x04
    bool  StopRunning;                               // +0x05
    bool  TerminateUsingExceptions;                  // +0x06
    float TimeProcessSliceStarted;                   // +0x08
    float TimesliceLength;                           // +0x0c
};

class CSuspendableProcessMicrothread : public CASuspendableProcess {
public:
    MicroThread::MicroThreadInfo* PMicrothread;      // +0x10
    virtual ~CSuspendableProcessMicrothread();
protected:
    static void __fastcall InternalSwitchToProcess(CSuspendableProcessMicrothread& process);
};

extern CSuspendableProcessMicrothread* g_pScriptRunner;     // 0x013D2838

void __fastcall CSuspendableProcessMicrothread::InternalSwitchToProcess(CSuspendableProcessMicrothread& process)
{
    g_pScriptRunner = &process;
    MicroThread::SwitchToMicroThread(process.PMicrothread);
    g_pScriptRunner = 0;
}