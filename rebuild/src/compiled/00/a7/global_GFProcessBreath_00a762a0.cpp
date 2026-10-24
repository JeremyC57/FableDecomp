// _global::GFProcessBreath(CASuspendableProcess& process, double& lastBreathTime) -- give a suspendable process its breathing slot: read the game time and, when the stored breath time is at least 5 ms past it, call the process's slot-4 handler and store the new time
// Leaf: ?GFProcessBreath@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A762A0, 56 bytes (+ int3 padding to 64), __fastcall `ret` (process in ecx, the
//   time reference in edx). PDB: `?GFProcessBreath@@YIXAAVCASuspendableProcess@@AAN@Z` (void __fastcall(CASuspendableProcess&,
//   double&)); no direct caller (reached through a function pointer).
// Callees: GFGetTime 0x009E1BC0 (`?GFGetTime@@YINXZ`, double in st0), CASuspendableProcess vtable slot 4 (0x00A44600 in
//   the base, empty; CLevelLoader overrides it with 0x006C27D0), the 0.005 double at 0x0129DD30.
// Class: engine/CASuspendableProcess.h models the vptr as a data member, which cannot express the virtual call, so the
//   class is restated with its virtual table (the landed 0x00A44840 model) and the header's member names/offsets.
// Shape: the time is kept in a NAMED double local (`fst qword [esp+8]` then reloaded for the final store after the
//   virtual call); `fld [time] ; fsub st(1)` is `lastBreathTime - now` and `test ah,1 ; jne` the `>=` branch.
double __fastcall GFGetTime();                       // 0x009E1BC0

class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();                 // slot 0 (0x00A44610)
    virtual void RunSuspendableProcess() = 0;        // slot 1 (CLevelLoader: 0x00A44880)
    virtual bool SuspendSuspendableProcess() = 0;    // slot 2 (0x00A44840)
    virtual void Restart() = 0;                      // slot 3 (0x00A447D0)
    virtual void SuspendableProcess();               // slot 4 (0x00A44600; CLevelLoader 0x006C27D0)
    bool  Running;                                   // +0x04
    bool  StopRunning;                               // +0x05
    bool  TerminateUsingExceptions;                  // +0x06
    float TimeProcessSliceStarted;                   // +0x08
    float TimesliceLength;                           // +0x0c
};

void __fastcall GFProcessBreath(CASuspendableProcess& process, double& lastBreathTime)
{
    double now = GFGetTime();
    if (lastBreathTime - now >= 0.005) {
        process.SuspendableProcess();
        lastBreathTime = now;
    }
}