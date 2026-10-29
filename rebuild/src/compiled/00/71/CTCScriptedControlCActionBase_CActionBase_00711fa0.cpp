// CTCScriptedControl::CActionBase::CActionBase() -- CBase root, a long (+4) and two flags (+8, +9) zeroed before the vtable store
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00711FA0, length 29 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Vtable 0x01265504 (CTCScriptedControl::CActionBase, rtti-vtables.tsv).
// Manifest label "CGameJoystickManager::CGameJoystickManager" is propagated over this body shape; the vtable names the class.
// Shape: three zero stores from one `xor eax,eax` before the vtable store = initialiser-list members.
// Leaf: ??0CActionBase
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                                        // 0x0099A2F0
};

namespace CTCScriptedControl {
class CActionBase : public CBaseClassNonCopyable { public:
    long m_actionState;                             // +0x04
    bool m_started;                                 // +0x08
    bool m_finished;                                // +0x09
    CActionBase();
};

CActionBase::CActionBase()
    : m_actionState(0), m_started(false), m_finished(false)
{
}
}