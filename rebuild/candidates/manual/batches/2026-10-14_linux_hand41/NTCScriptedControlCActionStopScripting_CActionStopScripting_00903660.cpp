// NTCScriptedControl::CActionStopScripting::CActionStopScripting() -- the inlined CTCScriptedControl::CActionBase constructor (CBase root + three zeroed members) with only the derived vtable stored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00903660, length 32 bytes.
// Callees: CBase::CBase @ 0x0099A2F0. Vtable 0x0128D788 (NTCScriptedControl::CActionStopScripting, rtti-vtables.tsv).
// Manifest label "CGameJoystickManager::CGameJoystickManager" is propagated; the vtable names the class.
// Shape: same stores as CActionBase::CActionBase (0x00711FA0) but a different vtable: the base constructor is inline here and
// its own vtable store is dead (overwritten by the derived one), so only the derived store survives.
// Leaf: ??0CActionStopScripting
struct CBase {
    virtual ~CBase();
    CBase();                                        // 0x0099A2F0
};

namespace CTCScriptedControl {
struct CActionBase : CBase {
    long m_actionState;                             // +0x04
    bool m_started;                                 // +0x08
    bool m_finished;                                // +0x09
    CActionBase() : m_actionState(0), m_started(false), m_finished(false) {}
};
}

namespace NTCScriptedControl {
struct CActionStopScripting : CTCScriptedControl::CActionBase {
    CActionStopScripting();
};

CActionStopScripting::CActionStopScripting()
{
}
}
