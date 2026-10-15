// CSuspendableProcessMicrothread::Restart() -- delete the current microthread (if any), create a fresh one running the process entry, clear StopRunning, throw "Failed to create fibre" when no microthread could be made
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A447D0, length 100 bytes (+12 int3 padding). Public virtual (`UAEXXZ`).
// Leaf: ?Restart@CSuspendableProcessMicrothread@@UAEXXZ
// Callees: DeleteMicroThread @ 0x009D8640 (`__fastcall`), CreateMicroThread(void (*)(void*), void*) @ 0x009D8710 (`__fastcall`:
// ecx the entry 0x00A446A0, edx this), the exception's constructor @ 0x004FAC70 (one empty byte, text ignored; catalog label
// "CFourierAnalysis" is a byte-twin), _CxxThrowException @ 0x00BFEB84 (throw info 0x013692F8, text 0x0129D46C).
// Shape: an UNOPTIMISED TU (`#pragma optimize("", off)`, as the landed ctor 0x00A44740 / dtor 0x00A44620): `this` spilled to
// [ebp-8] and reloaded for every use, the throw's temporary built at [ebp-2] and copied byte-wise into the throw object at [ebp-1].
// Model: the landed CSuspendableProcessMicrothread rows (engine/CASuspendableProcess.h member names, +0x10 PMicrothread).
#pragma optimize("", off)
struct MicroThreadInfo;
MicroThreadInfo* __fastcall CreateMicroThread(void (*entry)(void*), void* param);   // 0x009D8710
void __fastcall DeleteMicroThread(MicroThreadInfo* thread);                         // 0x009D8640
void MicrothreadEntry(void* param);        // 0x00A446A0: the fibre body that runs the process

struct CFibreException {                   // one byte, no state: the constructor @ 0x004FAC70 ignores its text
    CFibreException(const char* text);
};

class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();               // 0x00A44610
    bool  Running;                                 // +0x04
    bool  StopRunning;                             // +0x05
    bool  TerminateUsingExceptions;                // +0x06
    float TimeProcessSliceStarted;                 // +0x08
    float TimesliceLength;                         // +0x0c
};

class CSuspendableProcessMicrothread : public CASuspendableProcess {
public:
    MicroThreadInfo* PMicrothread;                 // +0x10
    virtual ~CSuspendableProcessMicrothread();
    virtual void Restart();
};

void CSuspendableProcessMicrothread::Restart()
{
    if (PMicrothread)
        DeleteMicroThread(PMicrothread);
    PMicrothread = CreateMicroThread(MicrothreadEntry, this);
    StopRunning = false;
    if (PMicrothread == 0)
        throw CFibreException("Failed to create fibre");
}