// CSuspendableProcessMicrothread::~CSuspendableProcessMicrothread() -- stop the process (TerminateProcess), destroy the microthread it ran on, drop the global microthread count, then the CASuspendableProcess base destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44620, length 62 bytes.
// Leaf: ??1CSuspendableProcessMicrothread
// Callees: CASuspendableProcess::TerminateProcess @ 0x00A4B200, DeleteMicroThread @ 0x009D8640 (`__fastcall`,
// the thread in ecx), CASuspendableProcess::~CASuspendableProcess @ 0x00A44610; vtable 0x0129D440; the count
// @ 0x013D283C.
// Shape: an UNOPTIMISED TU (`#pragma optimize("", off)`, as the landed scalar deleting destructor 0x00A44A40):
// `this` spilled to [ebp-4] and reloaded for every use, the vtable store first, `mov edx,[g] ; sub edx,1 ;
// mov [g],edx` for the decrement, the base destructor called explicitly, `mov esp,ebp ; pop ebp ; ret`.
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
    MicroThreadInfo* PMicrothread;                 protected: // +0x10
    virtual ~CSuspendableProcessMicrothread(); public:
    virtual bool SuspendSuspendableProcess();
};

CSuspendableProcessMicrothread::~CSuspendableProcessMicrothread()
{
    TerminateProcess();
    DeleteMicroThread(PMicrothread);
    --g_NumMicrothreads;
}