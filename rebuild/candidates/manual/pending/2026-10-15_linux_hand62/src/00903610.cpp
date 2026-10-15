// NTCScriptedControl::CActionWait::CActionWait(long) -- inlined CActionBase part (no script, not activated, not terminated), a zero update counter, own vtable, then the frame count to wait
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903610, length 41 bytes.
// Leaf: ??0CActionWait
// Callees: CBase::CBase @ 0x0099A2F0.
// Identity: the stored vtable 0x0128D760 is NTCScriptedControl::CActionWait (rtti-vtables.tsv; its slot 4
// is 0x00903640, right after this body); the manifest label "CPSCSpline::CPSCSpline" is propagated.
// Layout (Ego_r.pdb): CActionBase {PTCScript +4, Activated +8, Terminated +9}, CActionWait
// {NoFramesToWait +0xc, UpdateCounter +0x10}; the CActionBase constructor is inline (its vtable store is
// dead), as in the landed sibling CActionWaitForThingToFinishPerformingTasks (0x00905130).
// Shape: the argument is loaded first, the zero stores (base members, then UpdateCounter) precede the
// vtable store, the argument store follows it.
struct CBase {
    virtual ~CBase();
    CBase();                                // 0x0099A2F0
};

class CTCScriptedControl;

namespace NTCScriptedControl {
struct CActionBase : CBase {
    CTCScriptedControl* PTCScript;          // +0x04
    bool                Activated;          // +0x08
    bool                Terminated;         // +0x09
    CActionBase() : PTCScript(0), Activated(false), Terminated(false) { }
};

struct CActionWait : CActionBase {          // vtable 0x0128D760
    long NoFramesToWait;                    // +0x0c
    long UpdateCounter;                     // +0x10
    CActionWait(long noFramesToWait);
};

CActionWait::CActionWait(long noFramesToWait)
    : NoFramesToWait(noFramesToWait)
    , UpdateCounter(0)
{
}
}
