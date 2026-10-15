// CSuspendableProcessMicrothread::SuspendSuspendableProcess() -- the cooperative suspend point: false at once when the process has been told to stop, else yield the microthread to the scheduler and, once resumed, false if a stop arrived meanwhile, true to carry on
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44840, length 56 bytes.
// Leaf: ?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ
// Callees: CASuspendableProcess::CheckTermination @ 0x00A4B220 (twice), YieldMicrothread @ 0x00A44690 (a free
// function: no `this` is set up for it).
// Shape: an UNOPTIMISED TU (`#pragma optimize("", off)`): `this` spilled to [ebp-4], each bool result widened
// with `movzx` and tested as an int, every `return false` its own `xor al,al ; jmp` to the shared epilogue,
// `mov al,1` last; a public virtual (`UAE_NXZ`).
#pragma optimize("", off)
struct MicroThreadInfo;
void __fastcall DeleteMicroThread(MicroThreadInfo* thread);   // 0x009D8640 (`if (thread) delete thread;` through its virtual slot 2)
void YieldMicrothread();                           // 0x00A44690 (an unoptimised free wrapper around the scheduler switch 0x009D8650)
extern long g_NumMicrothreads;                     // 0x013D283C

// CASuspendableProcess (engine/CASuspendableProcess.h: 0x10 bytes, vptr, Running, StopRunning,
// TerminateUsingExceptions, TimeProcessSliceStarted, TimesliceLength). The header models the vptr as a data
// member, which cannot express the derived destructor's vtable store or a virtual override, so the class is
// restated here with its virtual destructor and the same member names and offsets.
class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();               // 0x00A44610 (tail-jumps to CBase::~CBase 0x0099A300)
    bool  Running;                                 // +0x04
    bool  StopRunning;                             // +0x05
    bool  TerminateUsingExceptions;                // +0x06
    float TimeProcessSliceStarted;                 // +0x08
    float TimesliceLength;                         // +0x0c
    void TerminateProcess();                       // 0x00A4B200
    bool CheckTermination() const;                 // 0x00A4B220 (`StopRunning != 0`)
};

class CSuspendableProcessMicrothread : public CASuspendableProcess {
public:
    MicroThreadInfo* PMicrothread;                 // +0x10
    virtual ~CSuspendableProcessMicrothread();
    virtual bool SuspendSuspendableProcess();
};

bool CSuspendableProcessMicrothread::SuspendSuspendableProcess()
{
    if (CheckTermination())
        return false;
    YieldMicrothread();
    if (CheckTermination())
        return false;
    return true;
}
