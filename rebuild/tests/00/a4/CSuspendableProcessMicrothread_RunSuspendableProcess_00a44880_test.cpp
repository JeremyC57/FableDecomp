#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
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

// behaviour-test stubs for the out-of-line vector members (specialised before first use)
template <> void vector<CASuspendableProcess*>::push_back(CASuspendableProcess* const& x) { *_M_finish++ = x; }
template <> bool vector<CASuspendableProcess*>::empty() const { return _M_start == _M_finish; }
template <> CASuspendableProcess*& vector<CASuspendableProcess*>::back() { return *(_M_finish - 1); }
template <> void vector<CASuspendableProcess*>::pop_back() { --_M_finish; }
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

// ---- behaviour-test stubs ----
static CASuspendableProcess* g_items[16];
vector<CASuspendableProcess*> g_ProcessStack;
CSuspendableProcessMicrothread* g_pScriptRunner;
CASuspendableProcess::~CASuspendableProcess() { }
CSuspendableProcessMicrothread::~CSuspendableProcessMicrothread() { }
static int g_yields, g_switches, g_time;
static CSuspendableProcessMicrothread* g_order[8];
static CSuspendableProcessMicrothread* g_spawnA;
static CSuspendableProcessMicrothread* g_spawnB;
void YieldMicrothread() { ++g_yields; g_pScriptRunner = 0; }
double __fastcall GFGetTime() { return (double)(++g_time) + 0.5; }
void __fastcall CSuspendableProcessMicrothread::InternalSwitchToProcess(CSuspendableProcessMicrothread& process)
{
    g_order[g_switches++] = &process;
    if (g_switches == 1) {            // the first process spawns two more
        g_ProcessStack.push_back(g_spawnA);
        g_ProcessStack.push_back(g_spawnB);
    }
}
int main()
{
    int ok = 1;
    CSuspendableProcessMicrothread self, other, a, b;
    g_spawnA = &a; g_spawnB = &b; a.TimeProcessSliceStarted = b.TimeProcessSliceStarted = -1.0f;
    g_ProcessStack._M_start = g_ProcessStack._M_finish = g_items; g_ProcessStack._M_end_of_storage = g_items + 16;
    // A: called from inside another process
    g_pScriptRunner = &other;
    self.RunSuspendableProcess();
    if (g_yields != 1 || g_pScriptRunner != &other || g_ProcessStack._M_finish - g_ProcessStack._M_start != 2 ||
        g_items[0] != &other || g_items[1] != &self) { ok = 0; printf("caseA\n"); }
    // B: from the main thread -> run until the stack is empty, newest first
    g_ProcessStack._M_finish = g_items; g_pScriptRunner = 0;
    self.RunSuspendableProcess();
    if (g_switches != 3 || g_order[0] != &self || g_order[1] != &b || g_order[2] != &a || !g_ProcessStack.empty() ||
        self.TimeProcessSliceStarted != 3.5f || b.TimeProcessSliceStarted != -1.0f || a.TimeProcessSliceStarted != -1.0f) { ok = 0; printf("caseB %d\n", g_switches); }
    if (ok) printf("00a44880_TEST PASS\n");
    return ok ? 0 : 1;
}