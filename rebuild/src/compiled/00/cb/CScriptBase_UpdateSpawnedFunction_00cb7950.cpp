// CScriptBase::UpdateSpawnedFunction(CSpawnedFuncBase* func) -- run one script frame of a spawned script thread: honour pause, interrupt and predicate state, and report whether the thread is still alive
// Leaf: UpdateSpawnedFunction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7950, length 169 bytes (`ret 4`, bool).
// PDB: ?UpdateSpawnedFunction@CScriptBase@@IAE_NPAVCSpawnedFuncBase@@@Z.
// Callees: CSpawnedFuncBase::TestPredicate() const @ 0x00F35A00 (true without a predicate), CSpawnedFuncBase::RemovePredicate
// @ 0x00F35A30 (manifest label "_Dest_val<..CFaction..>" is propagated), CASuspendableProcess::TerminateProcess @ 0x00A4B200;
// virtuals: slot 1 (+0x04) and slot 3 (+0x0c) are pure in CASuspendableProcess (vtable 0x0129D428: dtor, three
// _purecall, SuspendableProcess) -- slot 1 resumes the thread for one timeslice, slot 3 restarts it after an interrupt
// (descriptive names); CSpawnedFuncBase slot 5 OnPredicateFail (+0x14), slot 6 OnInterrupted (+0x18).
// Layout: retail CScriptBase sits 0x10 below Ego_r.pdb after its containers (Threads list +4 is one word, the two
// CVectorMaps +8/+0x18 are 16 bytes), so PActiveThread is +0x2c; CSpawnedFuncBase is the PDB layout over the
// CASuspendableProcess base (Running +4, StopRunning +5).
// Shape: the inline accessors of the PDB (IsFuncPaused, IsFuncInterrupted, StopSuspendableProcess, ...) over the
// bytes; the interrupt flag local lives in bl and doubles as the zero stored by SetFuncAsInterrupted(false); the
// paused and still-running-but-not-stopping exits share the `PActiveThread = 0; return false` tail.
class CASuspendableProcess {
public:
    virtual ~CASuspendableProcess();
    virtual void ResumeTimeslice() = 0;            // slot 1 (+0x04)
    virtual void v02() = 0;
    virtual void Restart() = 0;                    // slot 3 (+0x0c)
    virtual void SuspendableProcess();             // slot 4 (+0x10)
    bool  Running;                                 // +0x04
    bool  StopRunning;                             // +0x05
    bool  TerminateUsingExceptions;                // +0x06
    float TimeProcessSliceStarted;                 // +0x08
    float TimesliceLength;                         // +0x0c
    unsigned long _pad10;                          // +0x10
    bool IsSuspendableProcessRunning() const { return Running; }
    bool IsSuspendableProcessStopping() const { return StopRunning; }
    void StopSuspendableProcess() { StopRunning = true; }
    void TerminateProcess();                       // 0x00A4B200
};

struct CCharString { void* PStringData; };

class CSpawnedFuncBase : public CASuspendableProcess {
public:
    virtual void OnPredicateFail();                // slot 5 (+0x14)
    virtual void OnInterrupted();                  // slot 6 (+0x18)
    unsigned long ID;                              // +0x14
    long          LastScriptFrame;                 // +0x18
    long          CurrentScriptFrame;              // +0x1c
    CCharString   Name;                            // +0x20
    CCharString   AssociatedRegionName;            // +0x24
    bool          FuncPaused;                      // +0x28
    bool          Interrupted;                     // +0x29
    bool IsFuncPaused() const { return FuncPaused; }
    bool IsFuncInterrupted() const { return Interrupted; }
    void SetFuncAsInterrupted(bool interrupted) { Interrupted = interrupted; }
    bool TestPredicate() const;                    // 0x00F35A00
    void RemovePredicate();                        // 0x00F35A30
};

class CScriptBase {
public:
    void*             __vftable;                   // +0x00
    unsigned char     _containers[0x28];           // +0x04 Threads, EntityScripts, EntityScriptBindings, LastWorldUpdateFrame
    CSpawnedFuncBase* PActiveThread;               // +0x2c
    bool UpdateSpawnedFunction(CSpawnedFuncBase* func);
};

bool CScriptBase::UpdateSpawnedFunction(CSpawnedFuncBase* func)
{
    PActiveThread = func;
    bool interrupted = false;
    if (!func->IsFuncPaused()) {
        if (func->TestPredicate()) {
            if (func->IsFuncInterrupted()) {
                func->OnInterrupted();
                func->TerminateProcess();
                func->Restart();
                func->SetFuncAsInterrupted(false);
                interrupted = true;
            }
            else {
                func->ResumeTimeslice();
                if (!func->TestPredicate()) {
                    func->StopSuspendableProcess();
                    func->RemovePredicate();
                    func->OnPredicateFail();
                }
            }
        }
        else {
            func->StopSuspendableProcess();
            func->RemovePredicate();
            func->OnPredicateFail();
        }
        if (!func->IsSuspendableProcessRunning() && !interrupted)
            return true;
        if (func->IsSuspendableProcessStopping()) {
            func->TerminateProcess();
            return true;
        }
    }
    PActiveThread = 0;
    return false;
}