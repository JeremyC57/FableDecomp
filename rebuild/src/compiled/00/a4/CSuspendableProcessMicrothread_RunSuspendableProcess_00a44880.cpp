// CSuspendableProcessMicrothread::RunSuspendableProcess() -- run this process's microthread: from inside another process, stack both and yield to the scheduler; from the main thread, stack it and keep switching to the top process until the stack is empty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A44880, 172 bytes (+ int3 padding), public virtual (PDB
//   `?RunSuspendableProcess@CSuspendableProcessMicrothread@@UAEXXZ`).
// Leaf: ?RunSuspendableProcess@CSuspendableProcessMicrothread@@UAEXXZ
// Callees: vector<CASuspendableProcess*>::push_back @ 0x00A44C20 (landed as the generic PushBack_dword body), empty() @ 0x00A44930,
//   back() @ 0x00A44A70, pop_back() @ 0x00A44A80 (out-of-line COMDAT copies; manifest labels propagated), YieldMicrothread
//   @ 0x00A44690 (landed name; Ego_r.pdb `?InternalSuspend@CSuspendableProcessMicrothread@@KIXXZ`), GFGetTime @ 0x009E1BC0
//   (`__fastcall`, double), InternalSwitchToProcess(CSuspendableProcessMicrothread&) @ 0x00A44660 (static `__fastcall`).
// Globals: the running process @ 0x013D2838 (landed name g_pScriptRunner), the process stack @ 0x013D2828 (12-byte vector).
// Shape: an UNOPTIMISED TU (`#pragma optimize("", off)`, as the landed ctor 0x00A44740 / Restart 0x00A447D0 /
//   SuspendSuspendableProcess 0x00A44840): `this` spilled to [ebp-48h], every inline vector member called out of line, each
//   pointer handed to push_back through its own conversion temporary, the empty() bool widened with `movzx`, the slice start
//   time stored straight from the x87 double.
#pragma optimize("", off)
struct MicroThreadInfo;
double __fastcall GFGetTime();                     // 0x009E1BC0
void YieldMicrothread();                           // 0x00A44690

class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();               // 0x00A44610
    bool  Running;                                 // +0x04
    bool  StopRunning;                             // +0x05
    bool  TerminateUsingExceptions;                // +0x06
    float TimeProcessSliceStarted;                 // +0x08
    float TimesliceLength;                         // +0x0c
};

template <class T> struct vector {                 // STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& x);                    // 0x00A44C20 (for T = CASuspendableProcess*)
    bool empty() const;                            // 0x00A44930
    T& back();                                     // 0x00A44A70
    void pop_back();                               // 0x00A44A80
};

class CSuspendableProcessMicrothread : public CASuspendableProcess {
public:
    MicroThreadInfo* PMicrothread;                 // +0x10
    virtual ~CSuspendableProcessMicrothread();
    virtual void RunSuspendableProcess();
    static void __fastcall InternalSwitchToProcess(CSuspendableProcessMicrothread& process);   // 0x00A44660
};

extern CSuspendableProcessMicrothread*  g_pScriptRunner;     // 0x013D2838
extern vector<CASuspendableProcess*>    g_ProcessStack;      // 0x013D2828
__forceinline void StartTimeslice(CASuspendableProcess* process)
{
    long reserved[12];     // never touched: models retail's 0x30 untouched inlinee frame bytes (contents unknown)
    process->TimeProcessSliceStarted = (float)GFGetTime();
}

void CSuspendableProcessMicrothread::RunSuspendableProcess()
{
    if (g_pScriptRunner != 0) {
        CSuspendableProcessMicrothread* previous = g_pScriptRunner;
        g_ProcessStack.push_back(g_pScriptRunner);
        g_ProcessStack.push_back(this);
        YieldMicrothread();
        g_pScriptRunner = previous;
    } else {
        g_ProcessStack.push_back(this);
        while (!g_ProcessStack.empty()) {
            CSuspendableProcessMicrothread* next = static_cast<CSuspendableProcessMicrothread*>(g_ProcessStack.back());
            g_ProcessStack.pop_back();
            StartTimeslice(this);
            InternalSwitchToProcess(*next);
        }
    }
}