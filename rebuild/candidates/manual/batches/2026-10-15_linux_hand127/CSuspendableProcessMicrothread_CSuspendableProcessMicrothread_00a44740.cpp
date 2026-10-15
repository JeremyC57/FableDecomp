// CSuspendableProcessMicrothread::CSuspendableProcessMicrothread(bool, long, long) -- base CASuspendableProcess(terminateUsingExceptions, unused, timesliceLength), own vtable (0x0129D440), PMicrothread cleared then set to a new microthread running the process entry (throws "Failed to create fibre" when none could be made), the global microthread count bumped
// Label: the manifest row says CCreatureAction_CastBattleCharge (propagated); the stored vtable 0x0129D440 is CSuspendableProcessMicrothread (rtti-vtables.tsv).
// Leaf: ??0CSuspendableProcessMicrothread
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A44740, length 130 bytes (+ int3 padding).
// Callees: CASuspendableProcess::CASuspendableProcess(bool, long, long) @ 0x00A445D0 (landed), CreateMicroThread(void (*)(void*), void*)
// @ 0x009D8710 (`__fastcall`: ecx the entry 0x00A446A0, edx this), the exception's constructor @ 0x004FAC70 (an empty one-byte object
// whose text argument is ignored: `mov eax,ecx ; ret 4`), _CxxThrowException @ 0x00BFEB84 (throw info 0x013692F8), the count @ 0x013D283C.
// Shape: an UNOPTIMISED TU (`#pragma optimize("", off)`, as the landed destructor 0x00A44620): `this` spilled to [ebp-8] and reloaded
// for every use, the three arguments pushed right to left from their slots (the bool through dl), the vtable store, the zero store
// and the assignment both after it, the throw's temporary built at [ebp-2] and copied byte-wise into the throw object at [ebp-1],
// `mov eax,[g] ; add eax,1 ; mov [g],eax` for the increment, `mov esp,ebp ; pop ebp ; ret 0Ch`.
#pragma optimize("", off)
struct MicroThreadInfo;
MicroThreadInfo* __fastcall CreateMicroThread(void (*entry)(void*), void* param);   // 0x009D8710
void MicrothreadEntry(void* param);        // 0x00A446A0: the fibre body that runs the process
extern long g_NumMicrothreads;             // 0x013D283C

struct CFibreException {                   // one byte, no state: the constructor @ 0x004FAC70 ignores its text
    CFibreException(const char* text);
};

struct CBase {
    virtual void v00();
};

struct CASuspendableProcess : CBase {       // vtable 0x0129D428; sizeof 0x10 (engine/CASuspendableProcess.h names)
    unsigned char Running;                  // +0x04
    unsigned char StopRunning;              // +0x05
    unsigned char TerminateUsingExceptions; // +0x06
    long          TimeProcessSliceStarted;  // +0x08
    long          TimesliceLength;          // +0x0c
    virtual void v00();
    CASuspendableProcess(bool terminateUsingExceptions, long unused, long timesliceLength);   // 0x00A445D0
};

struct CSuspendableProcessMicrothread : CASuspendableProcess {   // vtable 0x0129D440
    MicroThreadInfo* PMicrothread;          // +0x10
    virtual void v00();
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
