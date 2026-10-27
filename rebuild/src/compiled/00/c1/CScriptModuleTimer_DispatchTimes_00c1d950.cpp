// CScriptModuleTimer::DispatchTimes(void* argument) -- -1 when the owner has no dispatch target; otherwise the target dispatch 0x00C1DF70 with the module's remaining/total seconds, its current frame and the argument, mapped to 0 (handled) / -1
// Leaf: ?DispatchTimes@CScriptModuleTimer@@UAEJPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1D950, 76 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 2 of the CScriptModuleTimer vtable 0x012B32A0 (slot 0 the scalar deleting destructor
//   0x00C1DD30, slot 1 the landed UpdateState 0x00C1D8C0, whose header names this row as the sibling that "forwards the
//   same three timer values to 0x00C1DF70"). libpk has no PDB; CScriptModuleTimer is the landed descriptive name.
// Callees: CScriptModuleTimer::GetCurrentFrame @ 0x00C1D810, GetTotalTimeSeconds @ 0x00C1D850,
//   GetRemainingTimeSeconds @ 0x00C1D820 (bound spellings, thiscall, no arguments), and the landed
//   CAIStateGroup_00c1df70::InitializeAndDispatch(void*, void*, void*, void*) @ 0x00C1DF70 on a one-word local copy of
//   the target (its only member), homed in the `push ecx` frame slot.
// Shape: right-to-left argument evaluation (the argument pushed before any timer call); `neg al ; sbb ; neg ; dec` is
//   `handled ? 0 : -1`.
struct CDispatchTarget;

struct CAIStateGroup_00c1df70 {                 // the dispatcher: a one-word view of the target
    CDispatchTarget* m_pTarget;                 // +0
    bool InitializeAndDispatch(void* a, void* b, void* c, void* d);   // 0x00C1DF70
};

struct CScriptModuleOwner {
    void*            _pad00;
    CDispatchTarget* m_pTarget;                 // +0x04
};

struct CScriptModuleTimer {                     // vtable 0x012B32A0
    virtual ~CScriptModuleTimer();              // slot 0
    virtual long UpdateState(void* context, void* argument);   // slot 1 (0x00C1D8C0)
    virtual long DispatchTimes(void* argument); // slot 2
    CScriptModuleOwner* POwner;                 // +0x04
    unsigned long GetCurrentFrame();            // 0x00C1D810
    unsigned long GetRemainingTimeSeconds();    // 0x00C1D820
    unsigned long GetTotalTimeSeconds();        // 0x00C1D850
};

long CScriptModuleTimer::DispatchTimes(void* argument)
{
    CDispatchTarget* target = POwner->m_pTarget;
    if (!target)
        return -1;
    CAIStateGroup_00c1df70 group;
    group.m_pTarget = target;
    return group.InitializeAndDispatch((void*)GetRemainingTimeSeconds(), (void*)GetTotalTimeSeconds(),
                                       (void*)GetCurrentFrame(), argument) ? 0 : -1;
}