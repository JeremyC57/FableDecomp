// NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::CActionWaitForThingToFinishPerformingTasks(const CScriptThing& thing) -- inlined CActionBase constructor, then the thing member (+0xc) built in place from the script-thing manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00905130, length 48 bytes.
// Callees: CBase::CBase @ 0x0099A2F0, CScriptThing-returning helper @ 0x004C7E50 (`this` = the argument thing, hidden result =
// &m_thing; it forwards to CGameScriptThingManager::CreateScriptThing @ 0x00704860). Vtable 0x0128D7F0 (rtti-vtables.tsv).
// Manifest label "CGameJoystickManager::CGameJoystickManager" is propagated; the vtable names the class.
// Shape: the three base members are zeroed before the vtable store (inline base constructor, dead base vtable store), the
// member object is constructed after it through the hidden result pointer (`lea eax,[esi+0Ch] ; push eax`).
// Leaf: ??0CActionWaitForThingToFinishPerformingTasks
struct CBase {
    virtual ~CBase();
    CBase();                                        // 0x0099A2F0
};

class CScriptThing { public:
    void* m_pVtable;                                // +0 (CScriptThing is polymorphic, 0xc bytes)
    void* m_pImpData;                               // +4 CCountedPointer<CScriptThing>::Data
    void* m_pImpInfo;                               // +8 CCountedPointer<CScriptThing>::Info
    CScriptThing(const CScriptThing& other);        // out of line (not emitted here)
    CScriptThing MakeScriptThing() const;           // 0x004C7E50
};

namespace CTCScriptedControl {
class CActionBase : public CBase { public:
    long m_actionState;                             // +0x04
    bool m_started;                                 // +0x08
    bool m_finished;                                // +0x09
    CActionBase() : m_actionState(0), m_started(false), m_finished(false) {}
};
}

namespace NTCScriptedControl {
struct CActionWaitForThingToFinishPerformingTasks : CTCScriptedControl::CActionBase {
    CScriptThing m_thing;                           // +0x0c
    CActionWaitForThingToFinishPerformingTasks(const CScriptThing& thing);
};

CActionWaitForThingToFinishPerformingTasks::CActionWaitForThingToFinishPerformingTasks(const CScriptThing& thing)
    : m_thing(thing.MakeScriptThing())
{
}
}